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

#include "document.hpp"
#include <glibmm/convert.h>
#include <glibmm/miscutils.h>
#include <giomm/file.h>
#include <PDFWriter.h>
#include <PDFPage.h>
#include <PDFDocumentCopyingContext.h>

namespace Slicer {

Document::Document(std::string filePath)
    : m_sourcePath{std::move(filePath)}
{
    Glib::ustring uri = Glib::filename_to_uri(m_sourcePath);

    m_popplerDocument = poppler_document_new_from_file(uri.c_str(),
                                                       nullptr,
                                                       nullptr);

    if (m_popplerDocument == nullptr)
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

        int width, height;
        std::tie(width, height) = page->size();

        auto pdfPage = new PDFPage{};
        pdfPage->SetMediaBox(PDFRectangle(0, 0, width, height));

        copyingContext->MergePDFPageToPage(pdfPage, static_cast<unsigned>(page->number()));
        pdfWriter.WritePageAndRelease(pdfPage);
    }

    pdfWriter.EndPDF();
    delete copyingContext;
}

void Document::saveDocument(Glib::RefPtr<Gio::File> destinationFile) const
{
    const std::string tempFilePath = getTempFilePath();
    auto tempFile = Gio::File::create_for_path(tempFilePath);

    makePDFCopy(m_pages, m_sourcePath, tempFilePath);
    tempFile->move(destinationFile, Gio::FILE_COPY_OVERWRITE);
}

void Document::removePage(int pageNumber)
{
    auto command = std::make_shared<RemovePageCommand>(m_pages, pageNumber);
    m_commandManager.execute(command);
}

void Document::removePages(const std::vector<unsigned int>& positions)
{
    auto command = std::make_shared<RemovePagesCommand>(m_pages, positions);
    m_commandManager.execute(command);
}

void Document::removePageRange(int first, int last)
{
    auto command = std::make_shared<RemovePageRangeCommand>(m_pages, first, last);
    m_commandManager.execute(command);
}
}
