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

#ifndef SLICERVIEW_HPP
#define SLICERVIEW_HPP

#include "../backend/document.hpp"
#include "../application/backgroundthread.hpp"
#include "pagewidget.hpp"
#include <glibmm/dispatcher.h>
#include <gtkmm/flowbox.h>

namespace Slicer {

class View : public Gtk::FlowBox {

    using PageWidgetList = std::list<std::shared_ptr<PageWidget>>;
    using PageWidgetQueue = std::queue<std::weak_ptr<PageWidget>>;

public:
    View(BackgroundThread& backgroundThread);
    virtual ~View();

    void setDocument(Document& document, int targetWidgetSize);
    void changePageSize(int targetWidgetSize);
    void clearSelection();

    int getSelectedChildIndex() const;
    std::vector<unsigned int> getSelectedChildrenIndexes() const;
    std::vector<unsigned int> getUnselectedChildrenIndexes() const;

    sigc::signal<void> selectedPagesChanged;

private:
    PageWidgetList m_pageWidgets;
    int m_pageWidgetSize;
    Document* m_document = nullptr;
    std::vector<sigc::connection> m_documentConnections;
    PageWidgetQueue m_toRenderQueue;
    std::mutex m_toRenderQueueMutex;
    PageWidgetQueue m_renderedQueue;
    std::mutex m_renderedQueueMutex;
    Glib::Dispatcher m_dispatcher;
    BackgroundThread& m_backgroundThread;

    PageWidget* m_lastPageSelected = nullptr;

    std::shared_ptr<PageWidget> createPageWidget(const Glib::RefPtr<Page>& page);

    void onDispatcherCalled();
    void onModelItemsChanged(guint position, guint removed, guint added);
    void onModelPagesRotated(const std::vector<unsigned int>& positions);
    void onPageSelection(PageWidget* pageWidget);
    void onShiftSelection(PageWidget* pageWidget);
    void onPreviewRequested(const Glib::RefPtr<Page>& page);
    void displayRenderedPages();
    void renderQueuedPages();
    void killQueuedPages();
};
}

#endif // SLICERVIEW_HPP
