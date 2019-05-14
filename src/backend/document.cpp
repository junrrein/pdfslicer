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
#include "fileutils.hpp"
#include <numeric>
#include <range/v3/view/enumerate.hpp>

namespace Slicer {

Document::Document(const Glib::RefPtr<Gio::File>& sourceFile)
    : m_pages{Gio::ListStore<Page>::create()}
{
    FileData fileData = loadFile(sourceFile);
    m_pages->splice(0, 0, loadPages(fileData));

    m_filesData.emplace_back(std::move(fileData));
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
    if (position > numberOfPages())
        throw std::runtime_error("The insertion position is greater than the number of pages");

    for (unsigned int i = position; i < numberOfPages(); ++i)
        m_pages->get_item(i)->setDocumentIndex(i + static_cast<unsigned>(pages.size()));

    m_pages->splice(position, 0, pages);
}

void Document::movePage(unsigned int indexToMove, unsigned int indexDestination)
{
    Glib::RefPtr<Page> pageToMove = removePage(indexToMove);
    pageToMove->setDocumentIndex(indexDestination);
    insertPage(pageToMove);

    pagesReordered.emit({indexDestination});
}

void Document::movePageRange(unsigned int indexFirst,
                             unsigned int indexLast,
                             unsigned int indexDestination)
{
    auto pagesToMove = removePageRange(indexFirst, indexLast);

    for (unsigned int i = 0; i < pagesToMove.size(); ++i)
        pagesToMove.at(i)->setDocumentIndex(indexDestination + i);

    insertPageRange(pagesToMove, indexDestination);

    const unsigned int numberOfPages = indexLast - indexFirst + 1;
    std::vector<unsigned int> reorderedIndexes(numberOfPages);
    std::iota(reorderedIndexes.begin(), reorderedIndexes.end(), indexDestination);

    pagesReordered.emit(reorderedIndexes);
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

unsigned int Document::addFile(const Glib::RefPtr<Gio::File>& file, unsigned int position)
{
    FileData fileData = loadFile(file);
    std::vector<Glib::RefPtr<Page>> pages = loadPages(fileData);

    for (auto [i, page] : ranges::view::enumerate(pages))
        page->setDocumentIndex(position + i);

    insertPageRange(pages, position);
    m_filesData.emplace_back(std::move(fileData));

    return pages.size();
}

Glib::RefPtr<Page> Document::getPage(unsigned int index) const
{
    return m_pages->get_item(index);
}

const Glib::RefPtr<Gio::ListStore<Page>>& Document::pages() const
{
    return m_pages;
}

unsigned int Document::numberOfPages() const
{
    return m_pages->get_n_items();
}

Document::FileData Document::loadFile(const Glib::RefPtr<Gio::File>& sourceFile)
{
    std::unique_ptr<poppler::document> tempDocument{poppler::document::load_from_file(sourceFile->get_path())};

    if (tempDocument == nullptr)
        throw std::runtime_error("Couldn't load file: " + sourceFile->get_path());

    Glib::RefPtr<Gio::File> tempFile = TempFile::generate();
    sourceFile->copy(tempFile, Gio::FILE_COPY_OVERWRITE);

    std::unique_ptr<poppler::document> document{poppler::document::load_from_file(tempFile->get_path())};
    auto qpdfDocument = std::make_unique<QPDF>();
    qpdfDocument->processFile(tempFile->get_path().c_str());
    auto qpdfDocumentHelper = std::make_unique<QPDFPageDocumentHelper>(*qpdfDocument);

    return FileData{sourceFile,
                    tempFile,
                    std::move(document),
                    std::move(qpdfDocument),
                    std::move(qpdfDocumentHelper)};
}

std::vector<Glib::RefPtr<Page>> Document::loadPages(const Document::FileData& fileData)
{
    const Glib::ustring basenameWihoutExtension = getDisplayNameWithoutExtension(fileData.originalFile);
    const std::vector<QPDFPageObjectHelper> pages = fileData.qpdfDocumentHelper->getAllPages();
    std::vector<Glib::RefPtr<Page>> result;
    result.reserve(pages.size());

    for (auto [i, qpdfPage] : ranges::view::enumerate(pages)) {
        std::unique_ptr<poppler::page> ppage{fileData.popplerDocument->create_page(i)};

        if (ppage == nullptr)
            throw std::runtime_error("Couldn't load page with number: " + std::to_string(i));

        auto page = Glib::RefPtr<Page>{new Page{std::move(ppage),
                                                qpdfPage,
                                                basenameWihoutExtension,
                                                static_cast<unsigned>(i)}};
        result.push_back(page);
    }

    return result;
}

}
