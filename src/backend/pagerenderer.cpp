// PDF Slicer
// Copyright (C) 2018 Julián Unrrein

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

#include "pagerenderer.hpp"
#include <cairomm/context.h>

namespace Slicer {

PageRenderer::PageRenderer(const Page& page)
    : m_page{page}
{
}

Glib::RefPtr<Gdk::Pixbuf> PageRenderer::render(int targetSize) const
{
    const Page::Size outputSize = m_page.scaledRotatedSize(targetSize);
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
    if (m_page.rotation() == 90)
        cr->translate(outputSize.width, 0);
    else if (m_page.rotation() == 180)
        cr->translate(outputSize.width, outputSize.height);
    else if (m_page.rotation() == 270)
        cr->translate(0, outputSize.height);

    cr->rotate_degrees(m_page.rotation());

    // Scale Context to match the ImageSurface's area.
    // Otherwise the page would get rendered at (realWidth x realHeight).
    const Page::Size rotatedSize = m_page.rotatedSize();
    double scale;

    if (rotatedSize.width >= rotatedSize.height)
        scale = static_cast<double>(outputSize.width) / rotatedSize.width;
    else
        scale = static_cast<double>(outputSize.height) / rotatedSize.height;

    cr->scale(scale, scale);

    // Render page
    poppler_page_render(m_page.m_ppage.get(), cr->cobj());

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

} // namespace Slicer
