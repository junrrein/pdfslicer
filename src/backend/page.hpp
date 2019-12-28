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

#ifndef PAGE_HPP
#define PAGE_HPP

#include <glibmm/object.h>
#include <gdkmm/pixbuf.h>
#include <poppler/cpp/poppler-page.h>

namespace Slicer {

class Page : public Glib::Object {
public:
    struct Size {
        int width;
        int height;
    };

    Page(std::unique_ptr<poppler::page> ppage,
         const Glib::ustring& fileName,
         unsigned int fileNumber,
         unsigned int pageNumber);

    const Glib::ustring& fileName() const;
    unsigned int indexInFile() const;
    unsigned int getDocumentIndex() const;
    int sourceRotation() const { return m_sourceRotation; }
    int currentRotation() const { return m_currentRotation; }
    Size size() const;
    Size rotatedSize() const;
    Size scaledSize(int targetSize) const;
    Size scaledRotatedSize(int targetSize) const;

    void setDocumentIndex(unsigned int newIndex);
    void rotateRight();
    void rotateLeft();

    sigc::signal<void> indexChanged;
    const unsigned int m_fileNumber;

    static int sortFunction(const Page& a, const Page& b);
    static int sortFunction(const Glib::RefPtr<const Page>& a,
                            const Glib::RefPtr<const Page>& b);

private:
    std::unique_ptr<poppler::page> m_ppage;
    const Glib::ustring m_fileName;
    const unsigned int m_indexInFile;
    unsigned int m_indexInDocument;
    int m_sourceRotation;
    int m_currentRotation;

    friend class PageRenderer; // For access to m_ppage
};

struct pageComparator {
    int operator()(const Glib::RefPtr<const Page>& a,
                   const Glib::RefPtr<const Page>& b);
};

} // namespace Slicer

#endif // PAGE_HPP
