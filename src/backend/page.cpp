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

PageSize Page::size() const
{
    double width = 0, height = 0;
    poppler_page_get_size(m_ppage, &width, &height);

    return {static_cast<int>(width), static_cast<int>(height)};
}

PageSize Page::scaledSize(int targetSize) const
{
    const PageSize realSize = size();
    PageSize scaledSize;

    if (realSize.height > realSize.width) {
        scaledSize.height = targetSize;
        const double ratioFactor = static_cast<double>(realSize.width) / realSize.height;
        scaledSize.width = static_cast<int>(floor(targetSize * ratioFactor));
    }
    else {
        scaledSize.width = targetSize;
        const double ratioFactor = static_cast<double>(realSize.height) / realSize.width;
        scaledSize.height = static_cast<int>(floor(targetSize * ratioFactor));
    }

    return scaledSize;
}

Glib::RefPtr<Gdk::Pixbuf> Page::renderPage(int targetSize) const
{
    const PageSize scaledSize = this->scaledSize(targetSize);
    auto surface = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32,
                                               scaledSize.width,
                                               scaledSize.height);
    auto cr = Cairo::Context::create(surface);

    // Paint a white background
    cr->set_source_rgb(255, 255, 255);
    cr->rectangle(0, 0, scaledSize.width, scaledSize.height);
    cr->fill();

    // Scale Context to match the ImageSurface's area.
    // Otherwise the page would get rendered at (realWidth x realHeight).
    const PageSize realSize = size();

    double scale;
    if (realSize.width >= realSize.height)
        scale = static_cast<double>(scaledSize.width) / realSize.width;
    else
        scale = static_cast<double>(scaledSize.height) / realSize.height;
    cr->scale(scale, scale);

    // Render page
    poppler_page_render(m_ppage, cr->cobj());

    // Scale back and paint a black outline
    cr->scale(1 / scale, 1 / scale);
    cr->set_line_width(1);
    cr->set_source_rgb(0, 0, 0);
    cr->rectangle(0, 0, scaledSize.width, scaledSize.height);
    cr->stroke();

    // Convert rendered page to a pixbuf
    auto pixbuf = Gdk::Pixbuf::create(surface, 0, 0, scaledSize.width, scaledSize.height);

    return pixbuf;
}

int pageComparator::operator()(const Glib::RefPtr<const Page>& a,
                               const Glib::RefPtr<const Page>& b)
{
    const int aPosition = a->number();
    const int bPosition = b->number();

    if (aPosition < bPosition)
        return -1;

    if (aPosition == bPosition)
        return 0;

    return 1;
}

} // namespace Slicer
