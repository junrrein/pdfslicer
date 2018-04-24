#include "view.hpp"
#include "viewchild.hpp"
#include "previewwindow.hpp"
#include <algorithm>
#include <numeric>
#include <functional>

namespace Slicer {

const int View::numRendererThreads = 1; // Poppler can't handle more than one
const std::set<int> View::zoomLevels = {200, 300, 400};

View::View(Gio::ActionMap& actionMap)
    : m_document{nullptr}
    , m_actionMap{actionMap}
    , m_zoomLevel{zoomLevels, m_actionMap}
{
    set_column_spacing(10);
    set_row_spacing(20);

    set_selection_mode(Gtk::SELECTION_MULTIPLE);
    set_activate_on_single_click(false);

    addActions();
    setupSignalHandlers();
}

View::~View()
{
    if (m_pageRendererPool != nullptr)
        m_pageRendererPool->stop();

    m_actionMap.remove_action("remove-selected");
    m_actionMap.remove_action("remove-previous");
    m_actionMap.remove_action("remove-next");
    m_actionMap.remove_action("preview-selected");
}

void View::setDocument(Document& document)
{
    stopRendering();
    m_document = &document;
    startGeneratingThumbnails(m_zoomLevel.minLevel());
}

void View::addActions()
{
    m_removeSelectedAction = m_actionMap.add_action("remove-selected", sigc::mem_fun(*this, &View::removeSelectedPages));
    m_removeUnselectedAction = m_actionMap.add_action("remove-unselected", sigc::mem_fun(*this, &View::removeUnselectedPages));
    m_removePreviousAction = m_actionMap.add_action("remove-previous", sigc::mem_fun(*this, &View::removePreviousPages));
    m_removeNextAction = m_actionMap.add_action("remove-next", sigc::mem_fun(*this, &View::removeNextPages));
    m_previewPageAction = m_actionMap.add_action("preview-selected", sigc::mem_fun(*this, &View::previewPage));
    m_cancelSelectionAction = m_actionMap.add_action("cancel-selection", sigc::mem_fun(*this, &View::onCancelSelection));

    m_removeSelectedAction->set_enabled(false);
    m_removeUnselectedAction->set_enabled(false);
    m_removePreviousAction->set_enabled(false);
    m_removeNextAction->set_enabled(false);
    m_previewPageAction->set_enabled(false);
    m_cancelSelectionAction->set_enabled(false);
}

void View::setupSignalHandlers()
{
    m_zoomLevel.changed.connect([this](int targetThumbnailSize) {
        stopRendering();
        startGeneratingThumbnails(targetThumbnailSize);
    });

    signal_selected_children_changed().connect([this]() {
        manageActionsEnabledStates();
    });

    signal_child_activated().connect([this](Gtk::FlowBoxChild*) {
        m_previewPageAction->activate();
    });
}

void View::manageActionsEnabledStates()
{
    const unsigned long numSelected = get_selected_children().size();

    if (numSelected == 0) {
        m_removeSelectedAction->set_enabled(false);
        m_removeUnselectedAction->set_enabled(false);
        m_removePreviousAction->set_enabled(false);
        m_removeNextAction->set_enabled(false);
        m_cancelSelectionAction->set_enabled(false);
    }
    else {
        m_removeSelectedAction->set_enabled();
        m_removeUnselectedAction->set_enabled();
        m_cancelSelectionAction->set_enabled();
    }

    if (numSelected == 1) {
        m_previewPageAction->set_enabled();

        const int index = get_selected_children().at(0)->get_index();
        if (index == 0)
            m_removePreviousAction->set_enabled(false);
        else
            m_removePreviousAction->set_enabled();

        if (index == static_cast<int>(get_children().size()) - 1)
            m_removeNextAction->set_enabled(false);
        else
            m_removeNextAction->set_enabled();
    }
    else {
        m_previewPageAction->set_enabled(false);
    }

    if (numSelected > 1) {
        m_removePreviousAction->set_enabled(false);
        m_removeNextAction->set_enabled(false);
    }
}

void View::onCancelSelection()
{
    unselect_all();
}

std::vector<unsigned int> View::getSelectedChildrenIndexes()
{
    std::vector<Gtk::FlowBoxChild*> children = get_selected_children();
    std::vector<unsigned int> positions;
    std::transform(children.begin(),
                   children.end(),
                   std::back_inserter(positions),
                   std::mem_fn(&Gtk::FlowBoxChild::get_index));

    return positions;
}

std::vector<unsigned int> View::getUnselectedChildrenIndexes()
{
    std::vector<unsigned int> selectedIndexes = getSelectedChildrenIndexes();

    std::vector<unsigned int> unselectedIndexes(get_children().size());
    std::iota(unselectedIndexes.begin(), unselectedIndexes.end(), 0);

    auto newEndIt = std::remove_if(unselectedIndexes.begin(),
                                   unselectedIndexes.end(),
                                   [&](unsigned int position) {
                                       return std::any_of(selectedIndexes.begin(),
                                                          selectedIndexes.end(),
                                                          [&](unsigned int selectedIndex) {
                                                              return position == selectedIndex;
                                                          });
                                   });
    unselectedIndexes.erase(newEndIt, unselectedIndexes.end());

    return unselectedIndexes;
}

void View::waitForRenderCompletion()
{
    if (m_pageRendererPool != nullptr)
        m_pageRendererPool->stop(true);

    m_pageRendererPool = std::make_unique<ctpl::thread_pool>(numRendererThreads);
}

void View::stopRendering()
{
    if (m_pageRendererPool != nullptr)
        m_pageRendererPool->stop();

    m_pageRendererPool = std::make_unique<ctpl::thread_pool>(numRendererThreads);
}

void View::startGeneratingThumbnails(int targetThumbnailSize)
{
    bind_list_store(m_document->pages(), [this, targetThumbnailSize](const Glib::RefPtr<Page>& page) {
        return Gtk::manage(new Slicer::ViewChild{page,
                                                 targetThumbnailSize,
                                                 *m_pageRendererPool});
    });
}

void View::removeSelectedPages()
{
    waitForRenderCompletion();

    if (get_selected_children().size() == 1) {
        Gtk::FlowBoxChild* child = get_selected_children().at(0);
        const int index = child->get_index();
        m_document->removePage(index);
    }
    else {
        m_document->removePages(getSelectedChildrenIndexes());
    }
}

void View::removeUnselectedPages()
{
    waitForRenderCompletion();

    m_document->removePages(getUnselectedChildrenIndexes());
}

void View::removePreviousPages()
{
    waitForRenderCompletion();

    std::vector<Gtk::FlowBoxChild*> selected = get_selected_children();

    if (selected.size() != 1)
        throw std::runtime_error(
            "Tried to remove previous pages with more "
            "than one page selected. This should never happen!");

    const int index = selected.at(0)->get_index();

    m_document->removePageRange(0, index - 1);
}

void View::removeNextPages()
{
    waitForRenderCompletion();

    std::vector<Gtk::FlowBoxChild*> selected = get_selected_children();

    if (selected.size() != 1)
        throw std::runtime_error(
            "Tried to remove next pages with more "
            "than one page selected. This should never happen!");

    const int index = selected.at(0)->get_index();

    m_document->removePageRange(index + 1,
                                static_cast<int>(m_document->pages()->get_n_items()) - 1);
}

void View::previewPage()
{
    // We need to wait till the thumbnails finish rendering
    // before rendering a big preview, to prevent crashes.
    // Poppler isn't designed for rendering many pages of
    // the same document in different threads.
    waitForRenderCompletion();

    const int pageNumber = get_selected_children().at(0)->get_index();

    Glib::RefPtr<Slicer::Page> page
        = m_document->pages()->get_item(static_cast<unsigned>(pageNumber));

    (new Slicer::PreviewWindow{page})->show();
}
}
