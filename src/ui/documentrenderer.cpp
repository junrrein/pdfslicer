#include "documentrenderer.hpp"
#include <glibmm/main.h>

namespace Slicer {

DocumentRenderer::DocumentRenderer(View& view, BackgroundThread& backgroundThread)
    : m_view{view}
    , m_backgroundThread{backgroundThread}
{
    m_dispatcher.connect(sigc::mem_fun(*this, &DocumentRenderer::onDispatcherCalled));
}

DocumentRenderer::~DocumentRenderer()
{
    for (sigc::connection& connection : m_documentConnections)
        connection.disconnect();

    m_backgroundThread.killRemainingTasks();
}

void DocumentRenderer::setDocument(Document& document, int targetWidgetSize)
{
    for (Gtk::Widget* child : m_view.get_children())
        m_view.remove(*child);

    m_pageWidgetList = {};

    for (sigc::connection& connection : m_documentConnections)
        connection.disconnect();

    m_document = &document;
    m_pageWidgetSize = targetWidgetSize;

    for (unsigned int i = 0; i < m_document->pages()->get_n_items(); ++i) {
        auto page = m_document->pages()->get_item(i);
        auto pageWidget = std::make_shared<PageWidget>(page, m_pageWidgetSize);
        m_pageWidgetList.push_back(pageWidget);
        m_view.insert(*m_pageWidgetList.back().get(), -1);
        m_toRenderQueue.push(pageWidget);
        m_dispatcher.emit();
    }

    m_documentConnections.push_back(
        m_document->pages()->signal_items_changed().connect(sigc::mem_fun(*this, &DocumentRenderer::onModelItemsChanged)));
    m_documentConnections.push_back(
        m_document->pagesRotated.connect(sigc::mem_fun(*this, &DocumentRenderer::onModelPagesRotated)));
}

void DocumentRenderer::onDispatcherCalled()
{
    {
        std::weak_ptr<PageWidget> weakWidget;

        while (m_renderedQueue.try_dequeue(weakWidget)) {
            std::shared_ptr<PageWidget> pageWidget = weakWidget.lock();

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
                m_renderedQueue.enqueue(pageWidget);
                m_dispatcher.emit();
            }
        });

        m_toRenderQueue.pop();
    }
}

void DocumentRenderer::onModelItemsChanged(guint position, guint removed, guint added)
{
    auto it = m_pageWidgetList.begin();
    std::advance(it, position);

    for (; removed != 0; --removed) {
        m_view.remove(*m_view.get_child_at_index(static_cast<int>(position)));
        it = m_pageWidgetList.erase(it);
    }

    for (guint i = 0; i != added; ++i) {
        auto page = m_document->pages()->get_item(position + i);
        auto pageWidget = std::make_shared<PageWidget>(page, m_pageWidgetSize);

        it = m_pageWidgetList.insert(it, pageWidget);
        m_view.insert(*pageWidget, static_cast<int>(position + i));
        m_toRenderQueue.push(pageWidget);
        m_dispatcher.emit();
    }
}

void DocumentRenderer::onModelPagesRotated(const std::vector<unsigned int>& positions)
{
    for (unsigned int position : positions) {
        auto it = m_pageWidgetList.begin();
        std::advance(it, position);

        (*it)->showSpinner();
        m_toRenderQueue.push(*it);
        m_dispatcher.emit();
    }
}

} // namespace Slicer
