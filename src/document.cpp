#include "document.hpp"
#include <glibmm/convert.h>
#include <glibmm/fileutils.h>
#include <glibmm/miscutils.h>
#include <giomm/file.h>
#include <PDFWriter.h>
#include <PDFPage.h>
#include <PDFDocumentCopyingContext.h>

namespace Slicer {

Document::Document(std::string filePath)
    : m_sourcePath{filePath}
{
    Glib::ustring uri = Glib::filename_to_uri(m_sourcePath);

    m_popplerDocument = poppler_document_new_from_file(uri.c_str(),
                                                       nullptr,
                                                       nullptr);

    if (!m_popplerDocument)
        throw std::runtime_error("Couldn't load file: " + m_sourcePath);

    const int num_pages = poppler_document_get_n_pages(m_popplerDocument);
    if (num_pages == 0)
        throw std::runtime_error("The file has zero pages");

    m_pages = Gio::ListStore<Page>::create();

    for (int i = 0; i < num_pages; ++i) {
        PopplerPage* popplerPage = poppler_document_get_page(m_popplerDocument, i);
        auto page = Glib::RefPtr<Page>{new Page{popplerPage}};
        m_pages->append(page);
    }
}

Document::~Document()
{
    g_object_unref(m_popplerDocument);
}

std::string getTempFilePath()
{
    const std::string tempDirectory = Glib::get_tmp_dir();
    const std::string tempFileName = "pdfslicer-temp.pdf";
    const std::vector<std::string> pathParts = {tempDirectory, tempFileName};
    const std::string tempFilePath = Glib::build_filename(pathParts);

    return tempFilePath;
}

void makePDFCopy(const Glib::RefPtr<Gio::ListStore<Page>>& pages,
                 const std::string& sourcePath,
                 const std::string& destinationPath)
{
    PDFWriter pdfWriter;
    pdfWriter.StartPDF(destinationPath, ePDFVersionMax);
    PDFDocumentCopyingContext* copyingContext = pdfWriter.CreatePDFCopyingContext(sourcePath);

    for (unsigned int i = 0; i < pages->get_n_items(); ++i) {
        Glib::RefPtr<Slicer::Page> page = pages->get_item(i);

        double width, height;
        std::tie(width, height) = page->size();

        PDFPage* pdfPage = new PDFPage();
        pdfPage->SetMediaBox(PDFRectangle(0, 0, width, height));

        copyingContext->MergePDFPageToPage(pdfPage, static_cast<unsigned>(page->number()));
        pdfWriter.WritePageAndRelease(pdfPage);
    }

    pdfWriter.EndPDF();
    delete copyingContext;
}

void Document::saveDocument(std::string destinationPath) const
{
    const std::string tempFilePath = getTempFilePath();

    makePDFCopy(m_pages, m_sourcePath, getTempFilePath());

    auto oldFile = Gio::File::create_for_path(destinationPath);
    auto newFile = Gio::File::create_for_path(tempFilePath);
    newFile->move(oldFile, Gio::FILE_COPY_OVERWRITE);
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
