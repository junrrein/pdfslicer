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
    Glib::RefPtr<Gio::SimpleAction> m_removePreviousAction;
    Glib::RefPtr<Gio::SimpleAction> m_removeNextAction;
    Glib::RefPtr<Gio::SimpleAction> m_previewPageAction;

    void stopRendering();
    void startGeneratingThumbnails(int targetThumbnailSize);
    void removeSelectedPage();
    void removePreviousPages();
    void removeNextPages();
    void previewPage();
    void addActions();
    void setupSignalHandlers();
    void manageActionsEnabledStates();
};
}

#endif // SLICERVIEW_HPP
