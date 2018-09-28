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

View::View(BackgroundThread& backgroundThread)
    : m_backgroundThread{backgroundThread}
{
    set_column_spacing(10);
    set_row_spacing(20);
    set_selection_mode(Gtk::SELECTION_NONE);

    m_dispatcher.connect(sigc::mem_fun(*this, &View::onDispatcherCalled));
}

View::~View()
{
    for (sigc::connection& connection : m_documentConnections)
        connection.disconnect();

    m_backgroundThread.killRemainingTasks();
}

std::shared_ptr<InteractivePageWidget> View::createPageWidget(const Glib::RefPtr<Page>& page)
{
    auto pageWidget = std::make_shared<InteractivePageWidget>(page, m_pageWidgetSize);

    pageWidget->selectedChanged.connect(sigc::mem_fun(*this, &View::onPageSelection));
    pageWidget->shiftSelected.connect(sigc::mem_fun(*this, &View::onShiftSelection));
    pageWidget->previewRequested.connect(sigc::mem_fun(*this, &View::onPreviewRequested));

    return pageWidget;
}

void View::setDocument(Document& document, int targetWidgetSize)
{
    killQueuedPages();

    for (Gtk::Widget* child : get_children())
        remove(*child);

    m_pageWidgets = {};

    for (sigc::connection& connection : m_documentConnections)
        connection.disconnect();

    m_document = &document;
    m_pageWidgetSize = targetWidgetSize;

    for (unsigned int i = 0; i < m_document->pages()->get_n_items(); ++i) {
        auto page = m_document->pages()->get_item(i);
        std::shared_ptr<InteractivePageWidget> pageWidget = createPageWidget(page);
        m_pageWidgets.push_back(pageWidget);
        insert(*m_pageWidgets.back().get(), -1);
        m_toRenderQueue.push(pageWidget);
        m_dispatcher.emit();
    }

    m_documentConnections.push_back(
        m_document->pages()->signal_items_changed().connect(sigc::mem_fun(*this, &View::onModelItemsChanged)));
    m_documentConnections.push_back(
        m_document->pagesRotated.connect(sigc::mem_fun(*this, &View::onModelPagesRotated)));
    selectedPagesChanged.emit();
}

void View::changePageSize(int targetWidgetSize)
{
    killQueuedPages();

    std::lock_guard<std::mutex> lock(m_toRenderQueueMutex);

    for (auto pageWidget : m_pageWidgets) {
        pageWidget->changeSize(targetWidgetSize);
        pageWidget->showSpinner();
        m_toRenderQueue.push(pageWidget);
        m_dispatcher.emit();
    }
}

void View::clearSelection()
{
    for (Gtk::Widget* child : get_children())
        dynamic_cast<InteractivePageWidget*>(child)->setChecked(false);

    selectedPagesChanged.emit();
}

int View::getSelectedChildIndex() const
{
    return static_cast<int>(getSelectedChildrenIndexes().front());
}

std::vector<unsigned int> View::getSelectedChildrenIndexes() const
{
    using namespace ranges;

    const std::vector<const Gtk::Widget*> children = get_children();

    const std::vector<unsigned int> result
        = children
          | view::transform([](const Gtk::Widget* child) {
                return dynamic_cast<const InteractivePageWidget*>(child);
            })
          | view::filter([](const InteractivePageWidget* pageWidget) {
                return pageWidget->getChecked();
            })
          | view::transform([](const InteractivePageWidget* pageWidget) {
                return static_cast<unsigned int>(pageWidget->get_index());
            });

    return result;
}

std::vector<unsigned int> View::getUnselectedChildrenIndexes() const
{
    std::vector<unsigned int> unselectedIndexes;
    ranges::set_difference(ranges::view::iota(0, m_pageWidgets.size()),
                           getSelectedChildrenIndexes(),
                           ranges::back_inserter(unselectedIndexes));

    return unselectedIndexes;
}

void View::displayRenderedPages()
{
    std::lock_guard<std::mutex> lock{m_renderedQueueMutex};

    while (!m_renderedQueue.empty()) {
        std::shared_ptr<PageWidget> pageWidget = m_renderedQueue.front().lock();
        m_renderedQueue.pop();

        if (pageWidget != nullptr) {
            pageWidget->showPage();
        }
    }
}

void View::renderQueuedPages()
{
    std::lock_guard<std::mutex> lock1(m_toRenderQueueMutex);

    while (!m_toRenderQueue.empty()) {
        std::weak_ptr<InteractivePageWidget> weakWidget = m_toRenderQueue.front();

        m_backgroundThread.push([this, weakWidget]() {
            std::shared_ptr<InteractivePageWidget> pageWidget = weakWidget.lock();

            if (pageWidget != nullptr) {
                pageWidget->renderPage();
                std::lock_guard<std::mutex> lock2{m_renderedQueueMutex};
                m_renderedQueue.push(pageWidget);
                m_dispatcher.emit();
            }
        });

        m_toRenderQueue.pop();
    }
}

void View::killQueuedPages()
{
    m_backgroundThread.killRemainingTasks();

    std::lock_guard<std::mutex> lock1(m_toRenderQueueMutex);
    std::lock_guard<std::mutex> lock2(m_renderedQueueMutex);

    m_toRenderQueue = {};
    m_renderedQueue = {};
}

void View::onDispatcherCalled()
{
    displayRenderedPages();
    renderQueuedPages();
}

void View::onModelItemsChanged(guint position, guint removed, guint added)
{
    auto it = m_pageWidgets.begin();
    std::advance(it, position);

    for (; removed != 0; --removed) {
        remove(*get_child_at_index(static_cast<int>(position)));
        it = m_pageWidgets.erase(it);
    }

    for (guint i = 0; i != added; ++i) {
        auto page = m_document->pages()->get_item(position + i);
        std::shared_ptr<InteractivePageWidget> pageWidget = createPageWidget(page);

        it = m_pageWidgets.insert(it, pageWidget);
        insert(*pageWidget, static_cast<int>(position + i));
        m_toRenderQueue.push(pageWidget);
        m_dispatcher.emit();
    }

    selectedPagesChanged.emit();
}

void View::onModelPagesRotated(const std::vector<unsigned int>& positions)
{
    for (unsigned int position : positions) {
        auto it = m_pageWidgets.begin();
        std::advance(it, position);

        (*it)->showSpinner();
        m_toRenderQueue.push(*it);
        m_dispatcher.emit();
    }
}

void View::onPageSelection(InteractivePageWidget* pageWidget)
{
    if (pageWidget->getChecked())
        m_lastPageSelected = pageWidget;
    else
        m_lastPageSelected = nullptr;

    selectedPagesChanged.emit();
}

void View::onShiftSelection(InteractivePageWidget* pageWidget)
{
    if (m_lastPageSelected == nullptr) {
        m_lastPageSelected = pageWidget;
    }
    else {
        int first = m_lastPageSelected->get_index();
        int last = pageWidget->get_index();

        if (first > last)
            std::swap(first, last);

        auto it = m_pageWidgets.begin();
        std::advance(it, first);

        for (int i = first; i != last; ++i) {
            it->get()->setChecked(true);
            std::advance(it, 1);
        }
    }

    selectedPagesChanged.emit();
}

void View::onPreviewRequested(const Glib::RefPtr<Page>& page)
{
    (new PreviewWindow{page, m_backgroundThread})->show();
}
}
