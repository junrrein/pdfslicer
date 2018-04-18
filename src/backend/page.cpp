#include "page.hpp"
#include <cairomm/context.h>

namespace Slicer {

Page::Page(PopplerPage* ppage)
    : m_ppage{ppage}
{
}

Page::~Page()
{
    g_object_unref(m_ppage);
}

int Page::number() const
{
    return poppler_page_get_index(m_ppage);
}

std::pair<int, int> Page::size() const
{
    double width = 0, height = 0;
    poppler_page_get_size(m_ppage, &width, &height);

    return {width, height};
}

std::pair<int, int> Page::scaledSize(int targetSize) const
{
    const auto[realWidth, realHeight] = size();

    int width, height;

    if (realHeight > realWidth) {
        height = targetSize;
        width = static_cast<int>(floor(targetSize * (static_cast<double>(realWidth) / realHeight)));
    }
    else {
        width = targetSize;
        height = static_cast<int>(floor(targetSize * (static_cast<double>(realHeight) / realWidth)));
    }

    return {width, height};
}

Glib::RefPtr<Gdk::Pixbuf> Page::renderPage(int targetSize) const
{
    const auto[width, height] = scaledSize(targetSize);

    auto surface = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, width, height);
    auto cr = Cairo::Context::create(surface);

    // Paint a white background
    cr->set_source_rgb(255, 255, 255);
    cr->rectangle(0, 0, width, height);
    cr->fill();

    // Scale Context to match the ImageSurface's area.
    // Otherwise the page would get rendered at (realWidth x realHeight).
    const auto[realWidth, realHeight] = size();

    double scale;
    if (realWidth >= realHeight)
        scale = static_cast<double>(width) / realWidth;
    else
        scale = static_cast<double>(height) / realHeight;
    cr->scale(scale, scale);

    // Render page
    poppler_page_render(m_ppage, cr->cobj());

    // Scale back and paint a black outline
    cr->scale(1 / scale, 1 / scale);
    cr->set_line_width(1);
    cr->set_source_rgb(0, 0, 0);
    cr->rectangle(0, 0, width, height);
    cr->stroke();

    // Convert rendered page to a pixbuf
    auto pixbuf = Gdk::Pixbuf::create(surface, 0, 0, width, height);

    return pixbuf;
}

} // namespace Slicer
