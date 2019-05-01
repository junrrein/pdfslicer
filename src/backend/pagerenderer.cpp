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

PageRenderer::PageRenderer(const Glib::RefPtr<const Page>& page)
    : m_page{page}
{
}

PageRenderer::RenderDimensions PageRenderer::getRenderDimensions(int targetSize) const
{
    const Page::Size outputSize = m_page->scaledRotatedSize(targetSize);
    const Page::Size rotatedSize = m_page->rotatedSize();
    double scale;

    if (rotatedSize.width >= rotatedSize.height)
        scale = static_cast<double>(outputSize.width) / rotatedSize.width;
    else
        scale = static_cast<double>(outputSize.height) / rotatedSize.height;

    poppler::rotation_enum rotation = poppler::rotate_0;
    switch (m_page->rotation()) {
    case 90:
        rotation = poppler::rotate_90;
        break;
    case 180:
        rotation = poppler::rotate_180;
        break;
    case 270:
        rotation = poppler::rotate_270;
    }

    return {outputSize, scale, rotation};
}

Glib::RefPtr<Gdk::Pixbuf> PageRenderer::render(int targetSize) const
{
    poppler::page_renderer renderer;
    renderer.set_render_hint(poppler::page_renderer::text_antialiasing);

    const auto [outputSize, scale, rotation] = getRenderDimensions(targetSize);

    poppler::image image = renderer.render_page(m_page->m_ppage.get(),
                                                standardDpi * scale,
                                                standardDpi * scale,
                                                -1,
                                                -1,
                                                outputSize.width,
                                                outputSize.height,
                                                rotation);

    int stride = Cairo::ImageSurface::format_stride_for_width(Cairo::FORMAT_ARGB32, outputSize.width);
    auto surface = Cairo::ImageSurface::create(reinterpret_cast<unsigned char*>(image.data()), //NOLINT
                                               Cairo::FORMAT_ARGB32,
                                               outputSize.width,
                                               outputSize.height,
                                               stride);

    // Paint a black outline
    auto cr = Cairo::Context::create(surface);
    cr->set_line_width(1);
    cr->set_source_rgb(0, 0, 0);
    cr->rectangle(0, 0, outputSize.width, outputSize.height);
    cr->stroke();

    return Gdk::Pixbuf::create(surface, 0, 0, outputSize.width, outputSize.height);
}

} // namespace Slicer
