#include "slicerdocument.hpp"
#include <cairomm/context.h>
#include <glibmm/convert.h>
#include "PDF-Writer/PDFWriter/PDFWriter.h"
#include "PDF-Writer/PDFWriter/PDFPage.h"
#include "PDF-Writer/PDFWriter/PDFDocumentCopyingContext.h"
#include "config.hpp"

namespace Slicer {

Document::Document(std::string filePath)
    : m_sourcePath{filePath}
{
    Glib::ustring uri = Glib::filename_to_uri(m_sourcePath);

    m_popplerDocument = poppler_document_new_from_file(uri.c_str(),
                                                       nullptr,
                                                       nullptr);

    if (!m_popplerDocument)
        throw std::runtime_error("No se pudo cargar el PDF");

    const int num_pages = poppler_document_get_n_pages(const_cast<PopplerDocument*>(m_popplerDocument));
    if (num_pages == 0)
        throw std::runtime_error("El PDF tiene 0 páginas?!");

    for (int i = 0; i < num_pages; ++i) {
        const PopplerPage* page = poppler_document_get_page(const_cast<PopplerDocument*>(m_popplerDocument),
                                                            i);
        m_pages.push_back(page);
    }
}

Document::~Document()
{
    // Hay que destruir manualmente todo lo que fue creado usando Poppler
    for (const PopplerPage* p : m_pages)
        g_object_unref(const_cast<PopplerPage*>(p));

    g_object_unref(const_cast<PopplerDocument*>(m_popplerDocument));
}

Glib::RefPtr<Gdk::Pixbuf> Document::renderPage(int pageNumber,
                                               int width,
                                               int height) const
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

    const PopplerPage* page = m_pages.at(pageNumber);

    // Scale Context to match the ImageSurface's area.
    // Otherwise the page would get rendered at (realWidth x realHeight).
    double realWidth = 0, realHeight = 0;
    poppler_page_get_size(const_cast<PopplerPage*>(page), &realWidth, &realHeight);

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

    // The following should center the page in the surface,
    // but it doesn't always look OK, and I don't know why.
    if (renderedWidth >= renderedHeight)
        cr->translate(0, double(height) / 2 - renderedHeight / 2);
    else
        cr->translate(double(width) / 2 - renderedWidth / 2, 0);

    // Render page and convert to Pixbuf
    poppler_page_render(const_cast<PopplerPage*>(page), cr->cobj());
    auto pixbuf = Gdk::Pixbuf::create(surface, 0, 0, width, height);

    return pixbuf;
}

void Document::saveDocument(std::string filePath) const
{
    PDFWriter pdfWriter;
    pdfWriter.StartPDF(filePath, ePDFVersion13);
    PDFDocumentCopyingContext* copyingContext = pdfWriter.CreatePDFCopyingContext(m_sourcePath);

    for (const PopplerPage* page : m_pages) {
        const int pageNumber = poppler_page_get_index(const_cast<PopplerPage*>(page));
        double width, height;
        poppler_page_get_size(const_cast<PopplerPage*>(page), &width, &height);

        PDFPage* pdfPage = new PDFPage();
        pdfPage->SetMediaBox(PDFRectangle(0, 0, width, height));

        copyingContext->MergePDFPageToPage(pdfPage, pageNumber);
        pdfWriter.WritePageAndRelease(pdfPage);
    }

    pdfWriter.EndPDF();
    delete copyingContext;
}

void Document::removePage(int pageNumber)
{
    const PopplerPage* p = m_pages.at(pageNumber);

    m_pages.erase(begin(m_pages) + pageNumber);

    g_object_unref(const_cast<PopplerPage*>(p));
}
}
