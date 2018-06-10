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

struct PageSize {
    int width;
    int height;
};

class Page : public Glib::Object {
public:
    Page(PopplerPage* ppage);
    virtual ~Page();

    int number() const;
    int rotation() const { return m_rotation; }
    PageSize size() const;
    PageSize scaledSize(int targetSize) const;

    Glib::RefPtr<Gdk::Pixbuf> renderPage(int targetSize) const;
    void rotateRight() { m_rotation += 90; }
    void rotateLeft() { m_rotation -= 90; }

private:
    PopplerPage* m_ppage;

    int m_rotation = 0;
};

struct pageComparator {
    int operator()(const Glib::RefPtr<const Page>& a,
                   const Glib::RefPtr<const Page>& b);
};

} // namespace Slicer

#endif // PAGE_HPP
