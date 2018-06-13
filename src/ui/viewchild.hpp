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

#ifndef VIEWCHILD_HPP
#define VIEWCHILD_HPP

#include "../backend/page.hpp"
#include <ctpl_stl.h>
#include <gtkmm/box.h>
#include <gtkmm/image.h>
#include <gtkmm/spinner.h>

namespace Slicer {

class ViewChild : public Gtk::Box {
public:
    ViewChild(const Glib::RefPtr<Page>& page,
              int targetSize);
    virtual ~ViewChild() = default;

    void renderPage();
    void showSpinner();
    void showPage();

private:
    const Glib::RefPtr<Slicer::Page> m_page;
    const int m_targetSize;
    Gtk::Image m_thumbnail;
    Gtk::Spinner m_spinner;

    bool isThumbnailVisible();
};

} // namespace Slicer

#endif // VIEWCHILD_HPP
