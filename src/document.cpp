#include "document.hpp"
#include <cairomm/context.h>
#include <glibmm/convert.h>
#include "../PDF-Writer/PDFWriter/PDFWriter.h"
#include "../PDF-Writer/PDFWriter/PDFPage.h"
#include "../PDF-Writer/PDFWriter/PDFDocumentCopyingContext.h"

namespace Slicer {

Document::Document(std::string filePath)
    : m_sourcePath{filePath}
{
    Glib::ustring uri = Glib::filename_to_uri(m_sourcePath);

    PopplerDocument* popplerDocument = poppler_document_new_from_file(uri.c_str(),
                                                                      nullptr,
                                                                      nullptr);

    if (!popplerDocument)
        throw std::runtime_error("No se pudo cargar el PDF");

    const int num_pages = poppler_document_get_n_pages(popplerDocument);
    if (num_pages == 0)
        throw std::runtime_error("El PDF tiene 0 páginas?!");

    m_pages = Gio::ListStore<GPopplerPage>::create();

    for (int i = 0; i < num_pages; ++i) {
        PopplerPage* page = poppler_document_get_page(popplerDocument, i);
        auto gPage = new GPopplerPage{page};

        // Disgusting hack to get a RefPtr from our GPopplerPage.
        // Gio::ListStore needs a RefPtr.
        Glib::RefPtr<Glib::Object> glibRefPtr = Glib::wrap(gPage->gobj());
        auto slicerPageRefPtr = Glib::RefPtr<GPopplerPage>::cast_dynamic(glibRefPtr);

        m_pages->append(slicerPageRefPtr);
    }

    g_object_unref(popplerDocument);
}

Glib::RefPtr<Gdk::Pixbuf> Document::renderPage(PopplerPage* page,
                                               int targetSize) const
{
    double realWidth = 0, realHeight = 0;
    poppler_page_get_size(page, &realWidth, &realHeight);

    int width, height;

    if (realHeight > realWidth) {
        height = targetSize;
        width = floor(targetSize * (realWidth / realHeight));
    }
    else {
        width = targetSize;
        height = floor(targetSize * (realHeight / realWidth));
    }

    auto surface = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, width, height);
    auto cr = Cairo::Context::create(surface);

    // Paint a white background
    cr->set_source_rgb(255, 255, 255);
    cr->rectangle(0, 0, width, height);
    cr->fill();

    // Scale Context to match the ImageSurface's area.
    // Otherwise the page would get rendered at (realWidth x realHeight).
    double scale;
    if (realWidth >= realHeight)
        scale = double(width) / realWidth;
    else
        scale = double(height) / realHeight;
    cr->scale(scale, scale);

    // Render page
    poppler_page_render(page, cr->cobj());

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

Glib::RefPtr<Gdk::Pixbuf> Document::renderPage(int pageNumber,
                                               int targetSize) const
{
    PopplerPage* page = m_pages->get_item(pageNumber)->page;

    return renderPage(page, targetSize);
}

void Document::saveDocument(std::string filePath) const
{
    PDFWriter pdfWriter;
    pdfWriter.StartPDF(filePath, ePDFVersion13);
    PDFDocumentCopyingContext* copyingContext = pdfWriter.CreatePDFCopyingContext(m_sourcePath);

    for (unsigned int i = 0; i < m_pages->get_n_items(); ++i) {
        PopplerPage* page = m_pages->get_item(i)->page;
        const int pageNumber = poppler_page_get_index(page);
        double width, height;
        poppler_page_get_size(page, &width, &height);

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
    m_pages->remove(pageNumber);
}

void Document::removePageRange(int first, int last)
{
    const int nElem = last - first + 1;

    m_pages->splice(first, nElem, {});
}
}
