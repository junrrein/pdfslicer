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
#include <poppler/cpp/poppler-page-renderer.h>

namespace Slicer {

PageRenderer::PageRenderer(const Page& page)
    : m_page{page}
{
}

Glib::RefPtr<Gdk::Pixbuf> PageRenderer::render(int targetSize) const
{
    const Page::Size outputSize = m_page.scaledRotatedSize(targetSize);

    poppler::page_renderer renderer;
    renderer.set_image_format(poppler::image::format_argb32);
    renderer.set_render_hint(poppler::page_renderer::antialiasing);
    renderer.set_render_hint(poppler::page_renderer::text_antialiasing);
    renderer.set_render_hint(poppler::page_renderer::text_hinting);

    poppler::image image = renderer.render_page(m_page.m_ppage.get(),
                                                72.0,
                                                72.0,
                                                -1,
                                                -1,
                                                outputSize.width,
                                                outputSize.height);

    int stride = Cairo::ImageSurface::format_stride_for_width(Cairo::FORMAT_ARGB32, outputSize.width);
    auto surface = Cairo::ImageSurface::create(reinterpret_cast<unsigned char*>(image.data()),
                                               Cairo::FORMAT_ARGB32,
                                               outputSize.width,
                                               outputSize.height,
                                               stride);
    auto cr = Cairo::Context::create(surface);

    // Paint a black outline
    cr->set_line_width(1);
    cr->set_source_rgb(0, 0, 0);
    cr->rectangle(0, 0, outputSize.width, outputSize.height);
    cr->stroke();

    return Gdk::Pixbuf::create(surface, 0, 0, outputSize.width, outputSize.height);
}

} // namespace Slicer
