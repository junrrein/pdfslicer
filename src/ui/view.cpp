// PDF Slicer
// Copyright (C) 2017-2018 Julián Unrrein

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "view.hpp"
#include "previewwindow.hpp"
#include <range/v3/all.hpp>

namespace Slicer {

const int View::numRendererThreads = 1; // Poppler can't handle more than one
const std::set<int> View::zoomLevels = {200, 300, 400};

View::View(Gio::ActionMap& actionMap,
           ActionBar& actionBar)
    : m_document{nullptr}
    , m_actionMap{actionMap}
    , m_zoomLevel{zoomLevels, m_actionMap}
    , m_actionBar{actionBar}
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
    if (m_backgroundThread != nullptr) {
        m_backgroundThread->stop();
        m_pagesRotatedConnection.disconnect();
    }

    m_actionMap.remove_action("remove-selected");
    m_actionMap.remove_action("remove-previous");
    m_actionMap.remove_action("remove-next");
    m_actionMap.remove_action("rotate-right");
    m_actionMap.remove_action("rotate-left");
    m_actionMap.remove_action("preview-selected");
}

void View::setDocument(Document& document)
{
    stopRendering();

    if (m_document == nullptr)
        m_zoomLevel.enable();
    else
        m_pagesRotatedConnection.disconnect();

    m_document = &document;

    m_document->commandExecuted().connect([this]() {
        m_actionBar.set_sensitive(true);
    });

    startGeneratingThumbnails(m_zoomLevel.minLevel());
    m_pagesRotatedConnection = m_document->pagesRotated.connect(sigc::mem_fun(*this, &View::onPagesRotated));
}

void View::addActions()
{
    m_removeSelectedAction = m_actionMap.add_action("remove-selected", sigc::mem_fun(*this, &View::removeSelectedPages));
    m_removeUnselectedAction = m_actionMap.add_action("remove-unselected", sigc::mem_fun(*this, &View::removeUnselectedPages));
    m_removePreviousAction = m_actionMap.add_action("remove-previous", sigc::mem_fun(*this, &View::removePreviousPages));
    m_removeNextAction = m_actionMap.add_action("remove-next", sigc::mem_fun(*this, &View::removeNextPages));
    m_rotateRightAction = m_actionMap.add_action("rotate-right", sigc::mem_fun(*this, &View::rotatePagesRight));
    m_rotateLeftAction = m_actionMap.add_action("rotate-left", sigc::mem_fun(*this, &View::rotatePagesLeft));
    m_previewPageAction = m_actionMap.add_action("preview-selected", sigc::mem_fun(*this, &View::previewPage));
    m_cancelSelectionAction = m_actionMap.add_action("cancel-selection", sigc::mem_fun(*this, &View::onCancelSelection));

    m_removeSelectedAction->set_enabled(false);
    m_removeUnselectedAction->set_enabled(false);
    m_removePreviousAction->set_enabled(false);
    m_removeNextAction->set_enabled(false);
    m_rotateRightAction->set_enabled(false);
    m_rotateLeftAction->set_enabled(false);
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

    m_thumbnailRendered.connect([this]() {
        if (m_childQueue.empty())
            return;

        ViewChild* child = m_childQueue.front();
        child->showPage();
        m_childQueue.pop();
    });

    m_executeCommand.connect([this]() {
        if (m_commandToExecute == nullptr)
            throw std::runtime_error("No command to execute");

        m_commandToExecute();
        m_commandToExecute = nullptr;
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
        m_rotateRightAction->set_enabled(false);
        m_rotateLeftAction->set_enabled(false);
        m_cancelSelectionAction->set_enabled(false);
    }
    else {
        m_removeSelectedAction->set_enabled();
        m_rotateRightAction->set_enabled();
        m_rotateLeftAction->set_enabled();
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
    const std::vector<Gtk::FlowBoxChild*> children = get_selected_children();
    std::vector<unsigned int> selectedIndexes;
    ranges::transform(children,
                      ranges::back_inserter(selectedIndexes),
                      std::mem_fn(&Gtk::FlowBoxChild::get_index));

    return selectedIndexes;
}

std::vector<unsigned int> View::getUnselectedChildrenIndexes()
{
    const std::vector<unsigned int> selectedIndexes = getSelectedChildrenIndexes();
    std::vector<unsigned int> unselectedIndexes;
    ranges::set_difference(ranges::view::iota(0, get_children().size()),
                           selectedIndexes,
                           ranges::back_inserter(unselectedIndexes));

    return unselectedIndexes;
}

void View::onPagesRotated(const std::vector<unsigned int> pageNumbers)
{
    for (unsigned int pageNumber : pageNumbers) {
        Gtk::FlowBoxChild* fwChild = get_child_at_index(static_cast<int>(pageNumber));
        auto child = dynamic_cast<ViewChild*>(fwChild->get_child());
        renderChild(child);
    }
}

void View::waitForRenderCompletion()
{
    if (m_backgroundThread != nullptr)
        m_backgroundThread->stop(true);

    while (!m_childQueue.empty()) {
        ViewChild* child = m_childQueue.front();
        child->showPage();
        m_childQueue.pop();
    }

    m_backgroundThread = std::make_unique<ctpl::thread_pool>(numRendererThreads);
}

void View::stopRendering()
{
    if (m_backgroundThread != nullptr)
        m_backgroundThread->stop();

    m_backgroundThread = std::make_unique<ctpl::thread_pool>(numRendererThreads);
    m_childQueue = {};
}

void View::startGeneratingThumbnails(int targetThumbnailSize)
{
    bind_list_store(m_document->pages(), [this, targetThumbnailSize](const Glib::RefPtr<Page>& page) {
        auto child = Gtk::manage(new Slicer::ViewChild{page, //NOLINT
                                                       targetThumbnailSize});
        renderChild(child);

        return child;
    });
}

void View::renderChild(ViewChild* child)
{
    child->showSpinner();

    m_backgroundThread->push([this, child](int) {
        child->renderPage();
        m_childQueue.push(child);
        m_thumbnailRendered.emit();
    });
}

void View::removeSelectedPages()
{
    const std::vector<unsigned int> indexes = getSelectedChildrenIndexes();

    queuePageRemoval([=]() {
        m_document->removePages(indexes);
    });
}

void View::removeUnselectedPages()
{
    const std::vector<unsigned int> indexes = getUnselectedChildrenIndexes();

    queuePageRemoval([=]() {
        m_document->removePages(getUnselectedChildrenIndexes());
    });
}

void View::removePreviousPages()
{
    const int index = get_selected_children().at(0)->get_index();

    queuePageRemoval([=]() {
        m_document->removePageRange(0, index - 1);
    });
}

void View::removeNextPages()
{
    const int index = get_selected_children().at(0)->get_index();

    queuePageRemoval([=]() {
        m_document->removePageRange(index + 1,
                                    static_cast<int>(m_document->pages()->get_n_items()) - 1);
    });
}

void View::queuePageRemoval(const std::function<void()>& command)
{
    if (m_commandToExecute != nullptr)
        throw std::runtime_error("There already exists a queued command");

    m_commandToExecute = command;

    m_backgroundThread->push([this](int) {
        m_executeCommand.emit();
    });

    m_actionBar.set_sensitive(false);
}

void View::rotatePagesRight()
{
    m_document->rotatePagesRight(getSelectedChildrenIndexes());
}

void View::rotatePagesLeft()
{
    m_document->rotatePagesLeft(getSelectedChildrenIndexes());
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
