#include "document.hpp"
#include <glibmm/convert.h>
#include <PDFWriter.h>
#include <PDFPage.h>
#include <PDFDocumentCopyingContext.h>

namespace Slicer {

Document::Document(std::string filePath)
    : m_sourcePath{filePath}
{
    Glib::ustring uri = Glib::filename_to_uri(m_sourcePath);

    PopplerDocument* popplerDocument = poppler_document_new_from_file(uri.c_str(),
                                                                      nullptr,
                                                                      nullptr);

    if (!popplerDocument)
        throw std::runtime_error("Couldn't load file: " + m_sourcePath);

    const int num_pages = poppler_document_get_n_pages(popplerDocument);
    if (num_pages == 0)
        throw std::runtime_error("The file has zero pages");

    m_pages = Gio::ListStore<Page>::create();

    for (int i = 0; i < num_pages; ++i) {
        PopplerPage* popplerPage = poppler_document_get_page(popplerDocument, i);
        auto page = Glib::RefPtr<Page>{new Page{popplerPage}};
        m_pages->append(page);
    }

    g_object_unref(popplerDocument);
}

void Document::saveDocument(std::string filePath) const
{
    PDFWriter pdfWriter;
    pdfWriter.StartPDF(filePath, ePDFVersion13);
    PDFDocumentCopyingContext* copyingContext = pdfWriter.CreatePDFCopyingContext(m_sourcePath);

    for (unsigned int i = 0; i < m_pages->get_n_items(); ++i) {
        auto page = m_pages->get_item(i);

        double width, height;
        std::tie(width, height) = page->size();

        PDFPage* pdfPage = new PDFPage();
        pdfPage->SetMediaBox(PDFRectangle(0, 0, width, height));

        copyingContext->MergePDFPageToPage(pdfPage, page->number());
        pdfWriter.WritePageAndRelease(pdfPage);
    }

    pdfWriter.EndPDF();
    delete copyingContext;
}

void Document::removePage(int pageNumber)
{
    auto command = std::make_shared<RemovePageCommand>(m_pages, pageNumber);
    m_commandManager.execute(command);
}

void Document::removePageRange(int first, int last)
{
    auto command = std::make_shared<RemovePageRangeCommand>(m_pages, first, last);
    m_commandManager.execute(command);
}
}
