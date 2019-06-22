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

#ifndef ACTIONBAR_HPP
#define ACTIONBAR_HPP

#include <gtkmm/actionbar.h>
#include <gtkmm/button.h>
#include <gtkmm/menubutton.h>

namespace Slicer {

class ActionBar : public Gtk::ActionBar {
public:
    ActionBar();

    void enableButtonRemovePagesMore();
    void disableButtonRemovePagesMore();

private:
    Gtk::Button m_buttonRotateLeft;
    Gtk::Button m_buttonRotateRight;

    Gtk::Button m_buttonMoveLeft;
    Gtk::Button m_buttonMoveRight;

    Gtk::Button m_buttonRemovePages;
    Gtk::MenuButton m_buttonRemovePagesMore;

    Gtk::Button m_buttonSelectAll;
    Gtk::Button m_buttonCancelSelection;
};

} // namespace Slicer

#endif // ACTIONBAR_HPP
