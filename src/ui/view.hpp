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
#include "zoomlevelwithactions.hpp"
#include <gtkmm/flowbox.h>
#include <ctpl_stl.h>

namespace Slicer {

class View : public Gtk::FlowBox {
public:
    View(Gio::ActionMap& actionMap);
    virtual ~View();

    void setDocument(Document& document);
    void waitForRenderCompletion();

private:
    Document* m_document;
    std::unique_ptr<ctpl::thread_pool> m_pageRendererPool;
    static const int numRendererThreads;
    Gio::ActionMap& m_actionMap;

    ZoomLevelWithActions m_zoomLevel;
    static const std::set<int> zoomLevels;

    Glib::RefPtr<Gio::SimpleAction> m_removeSelectedAction;
    Glib::RefPtr<Gio::SimpleAction> m_removeUnselectedAction;
    Glib::RefPtr<Gio::SimpleAction> m_removePreviousAction;
    Glib::RefPtr<Gio::SimpleAction> m_removeNextAction;
    Glib::RefPtr<Gio::SimpleAction> m_previewPageAction;
    Glib::RefPtr<Gio::SimpleAction> m_cancelSelectionAction;

    void stopRendering();
    void startGeneratingThumbnails(int targetThumbnailSize);
    void removeSelectedPages();
    void removeUnselectedPages();
    void removePreviousPages();
    void removeNextPages();
    void previewPage();
    void addActions();
    void setupSignalHandlers();
    void manageActionsEnabledStates();
    void onCancelSelection();
    std::vector<unsigned int> getSelectedChildrenIndexes();
    std::vector<unsigned int> getUnselectedChildrenIndexes();
};
}

#endif // SLICERVIEW_HPP
