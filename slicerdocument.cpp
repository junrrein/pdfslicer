#include "slicerdocument.hpp"
#include <cairomm/context.h>
#include <glibmm/convert.h>

namespace Slicer {

Document::Document(std::string filePath)
{
    Glib::ustring uri = Glib::filename_to_uri(filePath);

    m_popplerDocument = poppler_document_new_from_file(uri.c_str(),
                                                       nullptr,
                                                       nullptr);

    if (!m_popplerDocument)
        throw std::runtime_error("No se pudo cargar el PDF");

    const int num_pages = poppler_document_get_n_pages(m_popplerDocument);
    if (num_pages == 0)
        throw std::runtime_error("El PDF tiene 0 páginas?!");

    for (int i = 0; i < num_pages; ++i) {
        PopplerPage* page = poppler_document_get_page(m_popplerDocument, i);
        m_pages.push_back(page);
    }
}

Document::~Document()
{
    // Hay que destruir manualmente todo lo que fue creado usando Poppler
    for (PopplerPage* p : m_pages)
        g_object_unref(p);

    g_object_unref(m_popplerDocument);
}

Glib::RefPtr<Gdk::Pixbuf> Document::renderPage(int pageNumber,
                                               int width,
                                               int height)
{
    auto surface = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, width, height);
    auto cr = Cairo::Context::create(surface);

    // Paint a white background
    cr->set_source_rgb(255, 255, 255);
    cr->rectangle(0, 0, width, height);
    cr->fill();

    // Paint a black outline
    cr->set_line_width(1);
    cr->set_source_rgb(0, 0, 0);
    cr->rectangle(0, 0, width, height);
    cr->stroke();

    // Scale Context to match the ImageSurface's area.
    // Otherwise the page would get rendered at (realWidth x realHeight).
    PopplerPage* page = m_pages.at(pageNumber);
    double realWidth = 0, realHeight = 0;
    poppler_page_get_size(page, &realWidth, &realHeight);

    double scale;
    if (realWidth >= realHeight)
        scale = double(width) / realWidth;
    else
        scale = double(height) / realHeight;
    cr->scale(scale, scale);

    // Translate Context so the page gets rendered at the center
    // of the ImageSurface.
    double renderedWidth = realWidth * scale,
           renderedHeight = realHeight * scale;

    // I have no idea why the renderedDimension scaling factor needs to be 2.5,
    // when it should be 2.
    if (renderedWidth >= renderedHeight)
        cr->translate(0, double(height) / 2 - renderedHeight / 2);
    else
        cr->translate(double(width) / 2 - renderedWidth / 2, 0);

    // Render and convert to Pixbuf
    poppler_page_render(page, cr->cobj());

    auto pixbuf = Gdk::Pixbuf::create(surface, 0, 0, width, height);

    return pixbuf;
}
}
