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

Glib::RefPtr<Page> Document::removePage(unsigned int index)
{
    Glib::RefPtr<Page> removedPage = m_pages->get_item(index);
    m_pages->remove(index);

    for (unsigned int i = index; i < numberOfPages(); ++i)
        m_pages->get_item(i)->setDocumentIndex(i);

    return removedPage;
}

std::vector<Glib::RefPtr<Page>> Document::removePages(const std::vector<unsigned int>& indexes)
{
    std::vector<Glib::RefPtr<Page>> removedPages;

    for (unsigned int position : indexes) {
        auto page = m_pages->get_item(position);
        removedPages.push_back(page);
    }

    // Removing disjoint pages is complicated.
    // The list positions are the positions in the list of the pages we
    // want to remove.
    // The problem is that, everytime a page is removed, all positions are invalidated.
    // After each page removal, the remaining positions must be decremented by one.
    for (unsigned int i = 0; i < indexes.size(); ++i) {
        const unsigned int actualPosition = indexes.at(i) - i;
        m_pages->remove(actualPosition);
    }

    for (unsigned int i = indexes.front(); i < numberOfPages(); ++i)
        m_pages->get_item(i)->setDocumentIndex(i);

    return removedPages;
}

std::vector<Glib::RefPtr<Page>> Document::removePageRange(unsigned int first, unsigned int last)
{
    std::vector<Glib::RefPtr<Page>> removedPages;

    for (unsigned int i = first; i <= last; ++i)
        removedPages.push_back(m_pages->get_item(i));

    const unsigned int nElem = last - first + 1;
    m_pages->splice(first, nElem, {});

    for (unsigned int i = first; i < numberOfPages(); ++i)
        m_pages->get_item(i)->setDocumentIndex(i);

    return removedPages;
}

void Document::insertPage(const Glib::RefPtr<Page>& page)
{
    for (unsigned int i = page->getDocumentIndex(); i < numberOfPages(); ++i)
        m_pages->get_item(i)->setDocumentIndex(i + 1);

    m_pages->insert_sorted(page, pageComparator{});
}

void Document::insertPages(const std::vector<Glib::RefPtr<Page>>& pages)
{
    for (const auto& page : pages)
        insertPage(page);
}

void Document::insertPageRange(const std::vector<Glib::RefPtr<Page>>& pages, unsigned int position)
{
    for (unsigned int i = position; i < numberOfPages(); ++i)
        m_pages->get_item(i)->setDocumentIndex(i + static_cast<unsigned>(pages.size()));

    m_pages->splice(position, 0, pages);
}

void Document::movePage(unsigned int indexToMove, unsigned int indexDestination)
{
    if (indexToMove < indexDestination) {
        for (unsigned int i = indexToMove + 1; i <= indexDestination; ++i)
            m_pages->get_item(i)->setDocumentIndex(i - 1);
    }
    else if (indexToMove > indexDestination) {
        for (unsigned int i = indexDestination; i < indexToMove; ++i)
            m_pages->get_item(i)->setDocumentIndex(i + 1);
    }

    m_pages->get_item(indexToMove)->setDocumentIndex(indexDestination);
    m_pages->sort(pageComparator{});
}

void Document::rotatePagesRight(const std::vector<unsigned int>& pageNumbers)
{
    for (unsigned int pageNumber : pageNumbers)
        m_pages->get_item(pageNumber)->rotateRight();

    pagesRotated.emit(pageNumbers);
}

void Document::rotatePagesLeft(const std::vector<unsigned int>& pageNumbers)
{
    for (unsigned int pageNumber : pageNumbers)
        m_pages->get_item(pageNumber)->rotateLeft();

    pagesRotated.emit(pageNumbers);
}

Glib::RefPtr<const Page> Document::getPage(unsigned int index) const
{
    return m_pages->get_item(index);
}

const Glib::RefPtr<Gio::ListStore<Page>>& Document::pages() const
{
    return m_pages;
}

std::string Document::basename() const
{
    return m_basename;
}

std::string Document::filePath() const
{
    return m_sourceFile->get_path();
}

unsigned int Slicer::Document::numberOfPages() const
{
    return m_pages->get_n_items();
}

void Document::loadDocument()
{
    PopplerDocumentPointer tempDocument = {poppler_document_new_from_file(m_sourceFile->get_uri().c_str(),
                                                                          nullptr,
                                                                          nullptr),
                                           &g_object_unref};

    if (tempDocument == nullptr)
        throw std::runtime_error("Couldn't load file: " + m_sourceFile->get_path());

    m_basename = m_sourceFile->get_basename();
    Glib::RefPtr<Gio::File> tempFile = TempFile::generate();
    m_sourceFile->copy(tempFile, Gio::FILE_COPY_OVERWRITE);
    m_sourceFile = tempFile;

    m_popplerDocument.reset(poppler_document_new_from_file(m_sourceFile->get_uri().c_str(),
                                                           nullptr,
                                                           nullptr));

    const int num_pages = poppler_document_get_n_pages(m_popplerDocument.get());

    for (int i = 0; i < num_pages; ++i) {
        auto page = Glib::RefPtr<Page>{new Page{m_popplerDocument.get(), i}};
        m_pages->append(page);
    }
}
}
