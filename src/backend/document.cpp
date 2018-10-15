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

namespace Slicer {

Document::Document(const Glib::RefPtr<Gio::File>& sourceFile)
    : m_sourceFile{sourceFile}
{
    m_popplerDocument = poppler_document_new_from_file(m_sourceFile->get_uri().c_str(),
                                                       nullptr,
                                                       nullptr);

    if (m_popplerDocument == nullptr)
        throw std::runtime_error("Couldn't load file: " + m_sourceFile->get_path());

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

void Document::reload()
{
    m_pages->remove_all();
    g_object_unref(m_popplerDocument);
    m_popplerDocument = nullptr;
    m_popplerDocument = poppler_document_new_from_file(m_sourceFile->get_uri().c_str(),
                                                       nullptr,
                                                       nullptr);

    if (m_popplerDocument == nullptr)
        throw std::runtime_error("Couldn't load file: " + m_sourceFile->get_path());

    const int num_pages = poppler_document_get_n_pages(m_popplerDocument);

    for (int i = 0; i < num_pages; ++i) {
        PopplerPage* popplerPage = poppler_document_get_page(m_popplerDocument, i);
        auto page = Glib::RefPtr<Page>{new Page{popplerPage}};
        m_pages->append(page);
    }

    m_commandManager.reset();
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
}
