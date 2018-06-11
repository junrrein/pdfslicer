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
#include <utility>

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

Page::Size Page::size() const
{
    double width = 0, height = 0;
    poppler_page_get_size(m_ppage, &width, &height);

    return {static_cast<int>(width), static_cast<int>(height)};
}

Page::Size Page::rotatedSize() const
{
    Size size = this->size();

    if (std::abs((m_rotation / 90) % 2) != 0)
        std::swap(size.width, size.height);

    return size;
}

Page::Size scaleSize(Page::Size sourceSize, int targetSize)
{
    Page::Size scaledSize;

    if (sourceSize.height > sourceSize.width) {
        scaledSize.height = targetSize;
        const double ratioFactor = static_cast<double>(sourceSize.width) / sourceSize.height;
        scaledSize.width = static_cast<int>(floor(targetSize * ratioFactor));
    }
    else {
        scaledSize.width = targetSize;
        const double ratioFactor = static_cast<double>(sourceSize.height) / sourceSize.width;
        scaledSize.height = static_cast<int>(floor(targetSize * ratioFactor));
    }

    return scaledSize;
}

Page::Size Page::scaledSize(int targetSize) const
{
    return scaleSize(size(), targetSize);
}

Page::Size Page::scaledRotatedSize(int targetSize) const
{
    return scaleSize(rotatedSize(), targetSize);
}

Glib::RefPtr<Gdk::Pixbuf> Page::renderPage(int targetSize) const
{
    const Size outputSize = scaledRotatedSize(targetSize);
    auto surface = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32,
                                               outputSize.width,
                                               outputSize.height);
    auto cr = Cairo::Context::create(surface);

    // Paint a white background
    cr->set_source_rgb(255, 255, 255);
    cr->rectangle(0, 0, outputSize.width, outputSize.height);
    cr->fill();

    cr->save();

    // Rotate Context to render the page
    if (m_rotation == 90)
        cr->translate(outputSize.width, 0);
    else if (m_rotation == 180)
        cr->translate(outputSize.width, outputSize.height);
    else if (m_rotation == 270)
        cr->translate(0, outputSize.height);

    cr->rotate_degrees(m_rotation);

    // Scale Context to match the ImageSurface's area.
    // Otherwise the page would get rendered at (realWidth x realHeight).
    const Size rotatedSize = this->rotatedSize();
    double scale;

    if (rotatedSize.width >= rotatedSize.height)
        scale = static_cast<double>(outputSize.width) / rotatedSize.width;
    else
        scale = static_cast<double>(outputSize.height) / rotatedSize.height;

    cr->scale(scale, scale);

    // Render page
    poppler_page_render(m_ppage, cr->cobj());

    // Scale and/or rotate back and paint a black outline
    cr->restore();
    cr->set_line_width(1);
    cr->set_source_rgb(0, 0, 0);
    cr->rectangle(0, 0, outputSize.width, outputSize.height);
    cr->stroke();

    // Convert rendered page to a pixbuf
    auto pixbuf = Gdk::Pixbuf::create(surface, 0, 0, outputSize.width, outputSize.height);

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

void Page::rotateRight()
{
    if (m_rotation == 270)
        m_rotation = 0;
    else
        m_rotation += 90;
}

void Page::rotateLeft()
{
    if (m_rotation == 0)
        m_rotation = 270;
    else
        m_rotation -= 90;
}

} // namespace Slicer
