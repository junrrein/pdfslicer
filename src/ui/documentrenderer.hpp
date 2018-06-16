#ifndef DOCUMENTRENDERER_HPP
#define DOCUMENTRENDERER_HPP

#include "pagewidget.hpp"
#include "view.hpp"
#include "../backend/document.hpp"
#include "../application/backgroundthread.hpp"
#include <glibmm/dispatcher.h>
#include <concurrentqueue.h>

namespace Slicer {

class DocumentRenderer {

public:
    DocumentRenderer(View& view, BackgroundThread& backgroundThread);
    ~DocumentRenderer();

    void setDocument(Document& document, int targetWidgetSize);

private:
    std::list<std::shared_ptr<PageWidget>> m_pageWidgetList;
    int m_pageWidgetSize;
    View& m_view;
    Document* m_document = nullptr;
    std::vector<sigc::connection> m_documentConnections;
    std::queue<std::weak_ptr<PageWidget>> m_toRenderQueue;
    moodycamel::ConcurrentQueue<std::weak_ptr<PageWidget>> m_renderedQueue;
    Glib::Dispatcher m_dispatcher;
    BackgroundThread& m_backgroundThread;

    void onDispatcherCalled();
    void onModelItemsChanged(guint position, guint removed, guint added);
    void onModelPagesRotated(const std::vector<unsigned int>& positions);
};

} // namespace Slicer

#endif // DOCUMENTRENDERER_HPP
