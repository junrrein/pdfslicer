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
#include <poppler.h>
#include <gdkmm/pixbuf.h>

namespace Slicer {

class Page : public Glib::Object {
public:
    struct Size {
        int width;
        int height;
    };

    Page(PopplerDocument* document, int pageNumber);

    int number() const;
    int rotation() const { return m_rotation; }
    Size size() const;
    Size rotatedSize() const;
    Size scaledSize(int targetSize) const;
    Size scaledRotatedSize(int targetSize) const;

    void rotateRight();
    void rotateLeft();

private:
    std::unique_ptr<PopplerPage, decltype(&g_object_unref)> m_ppage;
    int m_rotation = 0;

    friend class PageRenderer; // For access to m_ppage
};

struct pageComparator {
    int operator()(const Glib::RefPtr<const Page>& a,
                   const Glib::RefPtr<const Page>& b);
};

} // namespace Slicer

#endif // PAGE_HPP
