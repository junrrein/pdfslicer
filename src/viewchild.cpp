#include "viewchild.hpp"

namespace Slicer {

ViewChild::ViewChild(const Glib::RefPtr<GPopplerPage>& gPage)
    : m_gPage{gPage}
    , m_isRendered{false}
    , m_isShown{false}
{
    int width, height;
    std::tie(width, height) = getImageSize(gPage->page, 200);
    set_size_request(width, height);

    m_spinner.start();
    pack_start(m_spinner);
}

void ViewChild::renderPage()
{
    auto pixbuf = Slicer::renderPage(m_gPage->page, 200);
    m_thumbnail.set(pixbuf);

    m_isRendered = true;
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
