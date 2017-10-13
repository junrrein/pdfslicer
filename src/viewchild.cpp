#include "viewchild.hpp"

namespace Slicer {

ViewChild::ViewChild(const Slicer::Document& document,
                     const Glib::RefPtr<GPopplerPage>& gPage,
                     Glib::Dispatcher& uiDispatcher)
    : m_document{document}
    , m_uiDispatcher{uiDispatcher}
    , m_gPage{gPage}
    , m_isRendered{false}
    , m_isShown{false}
{
    set_size_request(200, 200);
    m_spinner.start();

    pack_start(m_spinner);
}

void ViewChild::renderPage()
{
    auto pixbuf = m_document.renderPage(m_gPage->page, 200);
    m_thumbnail.set(pixbuf);

    m_isRendered = true;
    m_uiDispatcher.emit();
}

void ViewChild::showPage()
{
    if (m_isRendered && !m_isShown) {
        m_spinner.stop();
        pack_start(m_thumbnail);
        m_thumbnail.show();
        m_spinner.hide();
        m_isShown = true;
    }
}

} // namespace Slicer
