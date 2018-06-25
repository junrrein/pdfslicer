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
#include <range/v3/all.hpp>

namespace Slicer {

View::View(BackgroundThread& backgroundThread)
    : m_backgroundThread{backgroundThread}
{
    set_column_spacing(10);
    set_row_spacing(20);
    set_selection_mode(Gtk::SELECTION_NONE);

    m_dispatcher.connect(sigc::mem_fun(*this, &View::onDispatcherCalled));

    signal_child_activated().connect([this](Gtk::FlowBoxChild* widget) {
        auto pageWidget = dynamic_cast<PageWidget*>(widget->get_child());
        pageWidget->setChecked(!pageWidget->getChecked());
        selectedPagesChanged.emit();
    });
}

View::~View()
{
    for (sigc::connection& connection : m_documentConnections)
        connection.disconnect();

    m_backgroundThread.killRemainingTasks();
}

void View::setDocument(Document& document, int targetWidgetSize)
{
    for (Gtk::Widget* child : get_children())
        remove(*child);

    m_pageWidgets = {};

    for (sigc::connection& connection : m_documentConnections)
        connection.disconnect();

    m_document = &document;
    m_pageWidgetSize = targetWidgetSize;

    for (unsigned int i = 0; i < m_document->pages()->get_n_items(); ++i) {
        auto page = m_document->pages()->get_item(i);
        auto pageWidget = std::make_shared<PageWidget>(page, m_pageWidgetSize);
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

void View::clearSelection()
{
    for (Gtk::Widget* child : get_children()) {
        auto fwchild = dynamic_cast<Gtk::FlowBoxChild*>(child);
        auto pageWidget = dynamic_cast<PageWidget*>(fwchild->get_child());
        pageWidget->setChecked(false);
    }

    selectedPagesChanged.emit();
}

int View::getSelectedChildIndex() const
{
    return static_cast<int>(getSelectedChildrenIndexes().front());
}

std::vector<unsigned int> View::getSelectedChildrenIndexes() const
{
    std::vector<const Gtk::FlowBoxChild*> children;
    ranges::transform(get_children(),
                      ranges::back_inserter(children),
                      [](const Gtk::Widget* child) {
                          return dynamic_cast<const Gtk::FlowBoxChild*>(child);
                      });

    std::vector<unsigned int> result;
    for (const Gtk::FlowBoxChild* fwchild : children) {
        auto pageWidget = dynamic_cast<const PageWidget*>(fwchild->get_child());

        if (pageWidget->getChecked())
            result.push_back(static_cast<unsigned int>(fwchild->get_index()));
    }

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

void View::onDispatcherCalled()
{
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

    while (!m_toRenderQueue.empty()) {
        std::weak_ptr<PageWidget> weakWidget = m_toRenderQueue.front();

        m_backgroundThread.push([this, weakWidget]() {
            std::shared_ptr<PageWidget> pageWidget = weakWidget.lock();

            if (pageWidget != nullptr) {
                pageWidget->renderPage();
                std::lock_guard<std::mutex> lock{m_renderedQueueMutex};
                m_renderedQueue.push(pageWidget);
                m_dispatcher.emit();
            }
        });

        m_toRenderQueue.pop();
    }
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
        auto pageWidget = std::make_shared<PageWidget>(page, m_pageWidgetSize);

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
}
