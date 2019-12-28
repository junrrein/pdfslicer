// PDF Slicer
// Copyright (C) 2018 Julián Unrrein

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

#ifndef SAVINGREVEALER_HPP
#define SAVINGREVEALER_HPP

#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/frame.h>
#include <gtkmm/label.h>
#include <gtkmm/revealer.h>
#include <gtkmm/spinner.h>

namespace Slicer {

class SavingRevealer : public Gtk::Revealer {
public:
    SavingRevealer();

    void saving();
    void saved();

private:
    Gtk::Frame m_outerFrame;

    Gtk::Box m_boxSaving;
    Gtk::Label m_labelSaving;
    Gtk::Spinner m_spinner;

    Gtk::Box m_boxDone;
    Gtk::Label m_labelDone;
    Gtk::Button m_closeButton;

    sigc::connection m_connectionSaved;
};

} // namespace Slicer

#endif // SAVINGREVEALER_HPP
