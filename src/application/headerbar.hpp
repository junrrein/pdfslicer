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

#ifndef HEADERBAR_HPP
#define HEADERBAR_HPP

#include <gtkmm/headerbar.h>
#include <giomm/actiongroup.h>
#include <gtkmm/menubutton.h>
#include <gtkmm/box.h>

namespace Slicer {

class HeaderBar : public Gtk::HeaderBar {
public:
    HeaderBar();

private:
    Gtk::Button m_buttonOpen;
    Gtk::Button m_buttonAddFile;
    Gtk::Button m_buttonSave;
    Gtk::Button m_buttonUndo;
    Gtk::Button m_buttonRedo;
    Gtk::Button m_buttonZoomOut;
    Gtk::Button m_buttonZoomIn;
    Gtk::Box m_boxZoom;
    Gtk::MenuButton m_buttonAppMenu;

    void setupWidgets();
};

} // namespace Slicer

#endif // HEADERBAR_HPP
