#include "document.hpp"
#include <cairomm/context.h>
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
        throw std::runtime_error("No se pudo cargar el PDF");

    const int num_pages = poppler_document_get_n_pages(popplerDocument);
    if (num_pages == 0)
        throw std::runtime_error("El PDF tiene 0 páginas?!");

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

bool Document::canUndo() const
{
    return m_commandManager.canUndo();
}

bool Document::canRedo() const
{
    return m_commandManager.canRedo();
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

void Document::undoCommand()
{
    m_commandManager.undo();
}

void Document::redoCommand()
{
    m_commandManager.redo();
}

Document::RemovePageCommand::RemovePageCommand(const Glib::RefPtr<Gio::ListStore<Page>> pages,
                                               int position)
    : m_pages{pages}
    , m_position{position}
{
    m_removedPage = m_pages->get_item(position);
}

void Document::RemovePageCommand::execute()
{
    m_pages->remove(m_position);
}

void Document::RemovePageCommand::undo()
{
    m_pages->insert(m_position, m_removedPage);
}

void Document::RemovePageCommand::redo()
{
    execute();
}

Document::RemovePageRangeCommand::RemovePageRangeCommand(const Glib::RefPtr<Gio::ListStore<Page>> pages,
                                                         int first,
                                                         int last)
    : m_pages{pages}
    , m_first{first}
    , m_last{last}
{
    // Store pages in reversed order, since Gio::ListStore::splice()
    // inserts them in reversed order.
    for (int i = last; i >= first; --i)
        m_removedPages.push_back(m_pages->get_item(i));
}

void Document::RemovePageRangeCommand::execute()
{
    const int nElem = m_last - m_first + 1;

    m_pages->splice(m_first, nElem, {});
}

void Document::RemovePageRangeCommand::undo()
{
    m_pages->splice(m_first, 0, m_removedPages);
}

void Document::RemovePageRangeCommand::redo()
{
    execute();
}
}
