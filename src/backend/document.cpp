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
#include "tempfile.hpp"

namespace Slicer {

Document::Document(const Glib::RefPtr<Gio::File>& sourceFile)
    : m_popplerDocument{nullptr, &g_object_unref}
    , m_sourceFile{sourceFile}
    , m_pages{Gio::ListStore<Page>::create()}
{
    loadDocument();
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

void Document::rotatePagesRight(const std::vector<unsigned int>& pageNumbers)
{
    auto command = std::make_shared<RotatePagesRightCommand>(m_pages, pageNumbers, pagesRotated);
    m_commandManager.execute(command);
}

void Document::rotatePagesLeft(const std::vector<unsigned int>& pageNumbers)
{
    auto command = std::make_shared<RotatePagesLeftCommand>(m_pages, pageNumbers, pagesRotated);
    m_commandManager.execute(command);
}

void Document::loadDocument()
{
    PopplerDocument* document = poppler_document_new_from_file(m_sourceFile->get_uri().c_str(),
                                                               nullptr,
                                                               nullptr);

    if (document == nullptr)
        throw std::runtime_error("Couldn't load file: " + m_sourceFile->get_path());

    g_object_unref(document);
    m_basename = m_sourceFile->get_basename();
    Glib::RefPtr<Gio::File> tempFile = generateTempFile();
    m_sourceFile->copy(tempFile, Gio::FILE_COPY_OVERWRITE);
    m_sourceFile = tempFile;

    m_popplerDocument.reset(poppler_document_new_from_file(m_sourceFile->get_uri().c_str(),
                                                           nullptr,
                                                           nullptr));

    const int num_pages = poppler_document_get_n_pages(m_popplerDocument.get());

    for (int i = 0; i < num_pages; ++i) {
        PopplerPage* popplerPage = poppler_document_get_page(m_popplerDocument.get(), i);
        auto page = Glib::RefPtr<Page>{new Page{popplerPage}};
        m_pages->append(page);
    }
}
}
