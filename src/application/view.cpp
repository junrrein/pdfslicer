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

namespace rsv = ranges::view;

View::View(BackgroundThread& backgroundThread)
    : m_backgroundThread{backgroundThread}
{
    set_row_spacing(5);
    set_selection_mode(Gtk::SELECTION_NONE);
    set_sort_func(&sortFunction);

    m_dispatcher.connect(sigc::mem_fun(*this, &View::onDispatcherCalled));
}

View::~View()
{
    for (sigc::connection& connection : m_documentConnections)
        connection.disconnect();

    m_backgroundThread.killRemainingTasks();
}

std::shared_ptr<InteractivePageWidget> View::createPageWidget(const Glib::RefPtr<const Page>& page)
{
    auto pageWidget = std::make_shared<InteractivePageWidget>(page, m_pageWidgetSize, m_showFileNames);

    pageWidget->selectedChanged.connect(sigc::mem_fun(*this, &View::onPageSelection));
    pageWidget->shiftSelected.connect(sigc::mem_fun(*this, &View::onShiftSelection));
    pageWidget->previewRequested.connect(sigc::mem_fun(*this, &View::onPreviewRequested));

    return pageWidget;
}

void View::clearState()
{
    killStillRenderingPages();
    clearSelection();

    for (Gtk::Widget* child : get_children())
        remove(*child);

    m_pageWidgets = {};

    for (sigc::connection& connection : m_documentConnections)
        connection.disconnect();
}

void View::setDocument(Document& document, int targetWidgetSize)
{
    clearState();

    m_document = &document;
    m_pageWidgetSize = targetWidgetSize;

    for (unsigned int i = 0; i < m_document->numberOfPages(); ++i) {
        auto page = m_document->getPage(i);
        std::shared_ptr<InteractivePageWidget> pageWidget = createPageWidget(page);
        m_pageWidgets.push_back(pageWidget);
        add(*pageWidget);
        renderPage(pageWidget);
    }

    m_documentConnections.emplace_back(
        m_document->pages()->signal_items_changed().connect(sigc::mem_fun(*this, &View::onModelItemsChanged)));
    m_documentConnections.emplace_back(
        m_document->pagesRotated.connect(sigc::mem_fun(*this, &View::onModelPagesRotated)));
    m_documentConnections.emplace_back(
        m_document->pagesReordered.connect(sigc::mem_fun(*this, &View::onModelPagesReordered)));
    selectedPagesChanged.emit();
}

void View::changePageSize(int targetWidgetSize)
{
    killStillRenderingPages();

    for (auto& pageWidget : m_pageWidgets) {
        pageWidget->changeSize(targetWidgetSize);
        pageWidget->showSpinner();
        renderPage(pageWidget);
    }
}

void View::setShowFileNames(bool showFileNames)
{
    if (m_showFileNames == showFileNames)
        return;

    m_showFileNames = showFileNames;

    for (auto& pageWidget : m_pageWidgets)
        pageWidget->setShowFilename(showFileNames);
}

void View::selectPageRange(unsigned int first, unsigned int last)
{
    if (first > last || last > m_document->numberOfPages() - 1)
        throw std::runtime_error("Incorrect parameters");

    clearSelection();

    for (auto& widget : m_pageWidgets | rsv::drop(first) | rsv::take(last - first + 1))
        widget->setSelected(true);

    selectedPagesChanged.emit();
}

void View::clearSelection()
{
    for (auto& widget : m_pageWidgets)
        widget->setSelected(false);

    m_lastPageSelected = nullptr;

    selectedPagesChanged.emit();
}

unsigned int View::getSelectedChildIndex() const
{
    const std::vector<unsigned int> selected = getSelectedChildrenIndexes();

    if (selected.size() != 1)
        throw std::runtime_error("More than one child was actually selected");

    return selected.front();
}

std::vector<unsigned int> View::getSelectedChildrenIndexes() const
{
    const std::vector<unsigned int> result
        = m_pageWidgets
          | rsv::filter(std::mem_fn(&InteractivePageWidget::getSelected))
          | rsv::transform(std::mem_fn(&InteractivePageWidget::get_index));

    return result;
}

std::vector<unsigned int> View::getUnselectedChildrenIndexes() const
{
    const std::vector<unsigned int> result
        = m_pageWidgets
          | rsv::remove_if(std::mem_fn(&InteractivePageWidget::getSelected))
          | rsv::transform(std::mem_fn(&InteractivePageWidget::get_index));

    return result;
}

int View::sortFunction(Gtk::FlowBoxChild* a, Gtk::FlowBoxChild* b)
{
    const auto widgetA = dynamic_cast<InteractivePageWidget*>(a);
    const auto widgetB = dynamic_cast<InteractivePageWidget*>(b);

    return InteractivePageWidget::sortFunction(*widgetA, *widgetB);
}

void View::displayRenderedPages()
{
    safe::WriteAccess<LockableQueue> queue{m_renderedQueue};

    while (!queue->empty()) {
        std::shared_ptr<InteractivePageWidget> pageWidget = queue->front().lock();
        queue->pop();

        if (pageWidget != nullptr) {
            pageWidget->showPage();
        }
    }
}

void View::renderPage(const std::shared_ptr<InteractivePageWidget>& pageWidget)
{
    pageWidget->enableRendering();

    m_backgroundThread.pushBack([this, pageWidget]() {
        if (pageWidget->isRenderingCancelled())
            return;

        pageWidget->renderPage();

        {
            safe::WriteAccess<LockableQueue> queue{m_renderedQueue};
            queue->push(pageWidget);
        }

        m_dispatcher.emit();
    });
}

void View::killStillRenderingPages()
{
    m_backgroundThread.killRemainingTasks();

    safe::WriteAccess<LockableQueue> queue{m_renderedQueue};
    *queue = {};
}

void View::onDispatcherCalled()
{
    displayRenderedPages();
}

void View::onModelItemsChanged(guint position, guint removed, guint added)
{
    auto it = m_pageWidgets.begin();
    std::advance(it, position);

    for (; removed != 0; --removed) {
        (*it)->cancelRendering();
        remove(*(*it));
        it = m_pageWidgets.erase(it);
    }

    for (guint i = 0; i != added; ++i) {
        auto page = m_document->getPage(position + i);
        std::shared_ptr<InteractivePageWidget> pageWidget = createPageWidget(page);

        m_pageWidgets.insert(it, pageWidget);
        add(*pageWidget);
        renderPage(pageWidget);
    }

    selectedPagesChanged.emit();
}

void View::onModelPagesRotated(const std::vector<unsigned int>& positions)
{
    for (auto& pageWidget : m_pageWidgets) {
        for (unsigned int position : positions) {
            if (position == pageWidget->documentIndex()) {
                pageWidget->showSpinner();
                pageWidget->changeSize(m_pageWidgetSize);
                renderPage(pageWidget);

                break;
            }
        }
    }
}

void View::onModelPagesReordered(const std::vector<unsigned int>& positions)
{
    for (auto& pageWidget : m_pageWidgets) {
        for (unsigned int position : positions) {
            if (position == pageWidget->documentIndex()) {
                pageWidget->setSelected(true);

                break;
            }
        }
    }

    selectedPagesChanged.emit();
}

void View::onPageSelection(InteractivePageWidget* pageWidget)
{
    if (pageWidget->getSelected())
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

        if (first != -1 && last != -1) {
            for (auto& widget : m_pageWidgets)
                widget->setSelected(false);

            if (first > last)
                std::swap(first, last);

            for (auto& widget : m_pageWidgets | rsv::drop(first) | rsv::take(last - first + 1))
                widget->setSelected(true);
        }
    }

    selectedPagesChanged.emit();
}

void View::onPreviewRequested(const Glib::RefPtr<const Page>& page)
{
    (new PreviewWindow{page, m_backgroundThread})->show();
}
}
