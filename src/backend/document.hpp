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

    Glib::RefPtr<const Page> getPage(unsigned int index) const;
    const Glib::RefPtr<Gio::ListStore<Page>>& pages() const;
    std::string basename() const;
    std::string filePath() const;
    std::string originalParentPath() const;
    unsigned int numberOfPages() const;

    sigc::signal<void, std::vector<unsigned int>> pagesRotated;
    sigc::signal<void, std::vector<unsigned int>> pagesReordered;

private:
    using PopplerDocumentPointer = std::unique_ptr<PopplerDocument, decltype(&g_object_unref)>;

    // Data belonging to a specific PDF file
    struct FileData {
        PopplerDocumentPointer popplerDocument;
        Glib::RefPtr<Gio::File> originalFile;
        Glib::RefPtr<Gio::File> tempFile;

        FileData();
        FileData(PopplerDocumentPointer&& t_popplerDocument,
                 const Glib::RefPtr<Gio::File>& t_originalFile,
                 const Glib::RefPtr<Gio::File>& t_tempFile);
        FileData& operator=(FileData&&) = default;
    };

    FileData m_fileData;
    Glib::RefPtr<Gio::ListStore<Page>> m_pages;

    void loadDocument();
};
}

#endif // DOCUMENT_HPP
