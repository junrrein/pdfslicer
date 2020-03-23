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
#include <glibmm/main.h>
#include <range/v3/view.hpp>
#include <range/v3/range/conversion.hpp>

namespace Slicer {

namespace rsv = ranges::views;

View::View(TaskRunner& taskRunner,
           std::function<void()> onMouseWheelUp,
           std::function<void()> onMouseWheelDown)
    : m_taskRunner{taskRunner}
{
    setupFlowbox();
    setupSignalHandlers(onMouseWheelUp, onMouseWheelDown);
}

void View::setupFlowbox()
{
    m_flowBox.set_row_spacing(5);
    m_flowBox.set_selection_mode(Gtk::SELECTION_NONE);
    m_flowBox.set_sort_func(&sortFunction);

    add(m_flowBox);
}

void View::setupSignalHandlers(const std::function<void()>& onMouseWheelUp,
                               const std::function<void()>& onMouseWheelDown)
{
    add_events(Gdk::SCROLL_MASK | Gdk::SMOOTH_SCROLL_MASK);

    signal_scroll_event().connect([onMouseWheelUp, onMouseWheelDown](GdkEventScroll* event) {
        if ((event->state & Gdk::CONTROL_MASK) != 0) {
            if (event->delta_y < 0) {
                onMouseWheelUp();
                return true;
            }

            if (event->delta_y > 0) {
                onMouseWheelDown();
                return true;
            }
        }

        return false;
    });
}

View::~View()
{
    for (sigc::connection& connection : m_documentConnections)
        connection.disconnect();

    cancelRenderingTasks();
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
    cancelRenderingTasks();
    clearSelection();

    for (Gtk::Widget* child : m_flowBox.get_children())
        m_flowBox.remove(*child);

    m_pageWidgets.clear();

    for (sigc::connection& connection : m_documentConnections)
        connection.disconnect();

    m_documentConnections.clear();
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
        m_flowBox.add(*pageWidget);
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
    cancelRenderingTasks();

    m_pageWidgetSize = targetWidgetSize;

    for (auto& pageWidget : m_pageWidgets) {
        pageWidget->changeSize(m_pageWidgetSize);
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

void View::selectAllPages()
{
    for (auto& widget : m_pageWidgets)
        widget->setSelected(true);

    m_lastPageSelected = nullptr;

    selectedPagesChanged.emit();
}

void View::selectOddPages()
{
    for (auto [i, widget] : rsv::enumerate(m_pageWidgets)) {
        if (i % 2 == 0)
            widget->setSelected(true);
        else
            widget->setSelected(false);
    }

    m_lastPageSelected = nullptr;

    selectedPagesChanged.emit();
}

void View::selectEvenPages()
{
    for (auto [i, widget] : rsv::enumerate(m_pageWidgets)) {
        if (i % 2 == 1)
            widget->setSelected(true);
        else
            widget->setSelected(false);
    }

    m_lastPageSelected = nullptr;

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
    return m_pageWidgets
           | rsv::filter(std::mem_fn(&InteractivePageWidget::getSelected))
           | rsv::transform(std::mem_fn(&InteractivePageWidget::get_index))
           | ranges::to<std::vector<unsigned int>>;
}

std::vector<unsigned int> View::getUnselectedChildrenIndexes() const
{
    return m_pageWidgets
           | rsv::remove_if(std::mem_fn(&InteractivePageWidget::getSelected))
           | rsv::transform(std::mem_fn(&InteractivePageWidget::get_index))
           | ranges::to<std::vector<unsigned int>>;
}

int View::sortFunction(Gtk::FlowBoxChild* a, Gtk::FlowBoxChild* b)
{
    const auto widgetA = dynamic_cast<InteractivePageWidget*>(a);
    const auto widgetB = dynamic_cast<InteractivePageWidget*>(b);

    return InteractivePageWidget::sortFunction(*widgetA, *widgetB);
}

void View::renderPage(const std::shared_ptr<InteractivePageWidget>& pageWidget)
{
    std::weak_ptr<InteractivePageWidget> weakWidget = pageWidget;

    auto funcExecute = [weakWidget]() {
        if (auto widget = weakWidget.lock(); widget != nullptr)
            widget->renderPage();
    };

    auto funcPostExecute = [weakWidget]() {
        if (auto widget = weakWidget.lock(); widget != nullptr)
            widget->showPage();
    };

    auto task = std::make_shared<Task>(funcExecute, funcPostExecute);
    pageWidget->setRenderingTask(task);
    m_taskRunner.queueBack(task);
}

void View::cancelRenderingTasks()
{
    for (auto& pageWidget : m_pageWidgets)
        pageWidget->cancelRendering();
}

void View::onModelItemsChanged(guint position, guint removed, guint added)
{
    auto it = m_pageWidgets.begin();
    std::advance(it, position);

    for (; removed != 0; --removed) {
        (*it)->cancelRendering();
        m_flowBox.remove(*(*it));
        it = m_pageWidgets.erase(it);
    }

    for (guint i = 0; i != added; ++i) {
        auto page = m_document->getPage(position + i);
        std::shared_ptr<InteractivePageWidget> pageWidget = createPageWidget(page);

        m_pageWidgets.insert(it, pageWidget);
        m_flowBox.add(*pageWidget);
        renderPage(pageWidget);
    }

    selectedPagesChanged.emit();
}

void View::onModelPagesRotated(const std::vector<unsigned int>& positions)
{
    for (auto& pageWidget : m_pageWidgets) {
        for (unsigned int position : positions) {
            if (position == static_cast<unsigned>(pageWidget->get_index())) {
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
            if (position == static_cast<unsigned>(pageWidget->get_index())) {
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
    (new PreviewWindow{page, m_taskRunner})->show();
}
}
