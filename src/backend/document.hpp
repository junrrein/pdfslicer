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

#ifndef DOCUMENT_HPP
#define DOCUMENT_HPP

#include "page.hpp"
#include <giomm/file.h>
#include <giomm/liststore.h>
#include <poppler/cpp/poppler-document.h>
#include <qpdf/QPDFPageDocumentHelper.hh>

namespace Slicer {

class Document {
public:
    Document(const Glib::RefPtr<Gio::File>& sourceFile);

    Glib::RefPtr<Page> removePage(unsigned int index);
    std::vector<Glib::RefPtr<Page>> removePages(const std::vector<unsigned int>& indexes);
    std::vector<Glib::RefPtr<Page>> removePageRange(unsigned int first, unsigned int last);

    void insertPage(const Glib::RefPtr<Page>& page);
    void insertPages(const std::vector<Glib::RefPtr<Page>>& pages);
    void insertPageRange(const std::vector<Glib::RefPtr<Page>>& pages, unsigned int position);

    void movePage(unsigned int indexToMove, unsigned int indexDestination);
    void movePageRange(unsigned int indexFirst,
                       unsigned int indexLast,
                       unsigned int indexDestination);

    void rotatePagesRight(const std::vector<unsigned int>& pageNumbers);
    void rotatePagesLeft(const std::vector<unsigned int>& pageNumbers);

    Glib::RefPtr<Page> getPage(unsigned int index) const;
    const Glib::RefPtr<Gio::ListStore<Page>>& pages() const;
    std::string basename() const;
    std::string filePath() const;
    unsigned int numberOfPages() const;

    sigc::signal<void, std::vector<unsigned int>> pagesRotated;
    sigc::signal<void, std::vector<unsigned int>> pagesReordered;

private:
    struct FileData {
        Glib::RefPtr<Gio::File> originalFile;
        Glib::RefPtr<Gio::File> tempFile;
        std::unique_ptr<poppler::document> popplerDocument;
        std::unique_ptr<QPDF> qpdfDocument;
        std::unique_ptr<QPDFPageDocumentHelper> qpdfDocumentHelper;
    };

    FileData m_fileData;
    Glib::RefPtr<Gio::ListStore<Page>> m_pages;

    FileData loadFile(const Glib::RefPtr<Gio::File>& sourceFile);
    std::vector<Glib::RefPtr<Page>> loadPages(const FileData& fileData);
};
}

#endif // DOCUMENT_HPP
