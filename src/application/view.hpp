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

#include <document.hpp>
#include "../application/backgroundthread.hpp"
#include "interactivepagewidget.hpp"
#include <queue>
#include <glibmm/dispatcher.h>
#include <gtkmm/flowbox.h>

namespace Slicer {

class View : public Gtk::FlowBox {

    using PageWidgetList = std::list<std::shared_ptr<InteractivePageWidget>>;
    using PageWidgetQueue = std::queue<std::weak_ptr<InteractivePageWidget>>;

public:
    View(BackgroundThread& backgroundThread);
    virtual ~View();

    void setDocument(Document& document, int targetWidgetSize);
    void changePageSize(int targetWidgetSize);
    void clearSelection();

    unsigned int getSelectedChildIndex() const;
    std::vector<unsigned int> getSelectedChildrenIndexes() const;
    std::vector<unsigned int> getUnselectedChildrenIndexes() const;

    sigc::signal<void> selectedPagesChanged;

    static int sortFunction(Gtk::FlowBoxChild* a, Gtk::FlowBoxChild* b);

private:
    PageWidgetList m_pageWidgets;
    int m_pageWidgetSize = 0;
    Document* m_document = nullptr;
    std::vector<sigc::connection> m_documentConnections;
    PageWidgetQueue m_toRenderQueue;
    std::mutex m_toRenderQueueMutex;
    PageWidgetQueue m_renderedQueue;
    std::mutex m_renderedQueueMutex;
    Glib::Dispatcher m_dispatcher;
    BackgroundThread& m_backgroundThread;

    InteractivePageWidget* m_lastPageSelected = nullptr;

    std::shared_ptr<InteractivePageWidget> createPageWidget(const Glib::RefPtr<const Page>& page);

    void onDispatcherCalled();
    void onModelItemsChanged(guint position, guint removed, guint added);
    void onModelPagesRotated(const std::vector<unsigned int>& positions);
    void onPageSelection(InteractivePageWidget* pageWidget);
    void onShiftSelection(InteractivePageWidget* pageWidget);
    void onPreviewRequested(const Glib::RefPtr<const Page>& page);
    void displayRenderedPages();
    void renderQueuedPages();
    void killQueuedPages();
};
}

#endif // SLICERVIEW_HPP
