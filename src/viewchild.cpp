#include "viewchild.hpp"

namespace Slicer {

ViewChild::ViewChild(const Glib::RefPtr<Page>& page,
                     int targetSize)
    : m_page{page}
    , m_targetSize{targetSize}
    , m_isRendered{false}
    , m_isShown{false}
{
    int width, height;
    std::tie(width, height) = m_page->scaledSize(m_targetSize);
    set_size_request(width, height);

    m_spinner.set_size_request(38, 38);
    m_spinner.start();
    pack_start(m_spinner, true, false);
}

void ViewChild::renderPage()
{
    auto pixbuf = m_page->renderPage(m_targetSize);
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
