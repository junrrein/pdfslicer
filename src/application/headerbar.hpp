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

#include "appmenu.hpp"
#include <gtkmm/headerbar.h>
#include <gtkmm/menubutton.h>

namespace Slicer {

class HeaderBar : public Gtk::HeaderBar {
public:
    HeaderBar(const Glib::PropertyProxy<unsigned>& zoomIndexProperty);

    void enableAddDocumentButton();
    void disableAddDocumentButton();

    void enableZoomSlider();
    void disableZoomSlider();

private:
    Gtk::Button m_buttonOpen;
    Gtk::MenuButton m_buttonAddDocument;
    Gtk::Button m_buttonUndo;
    Gtk::Button m_buttonRedo;
    Gtk::Button m_buttonSave;
    Gtk::MenuButton m_buttonAppMenu;
    AppMenu m_appMenu;

    void setupWidgets();
};

} // namespace Slicer

#endif // HEADERBAR_HPP
