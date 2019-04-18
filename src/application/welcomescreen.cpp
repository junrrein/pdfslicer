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

#include "welcomescreen.hpp"
#include <glibmm/i18n.h>
#include <config.hpp>

namespace Slicer {

WelcomeScreen::WelcomeScreen()
    : Gtk::Box{Gtk::ORIENTATION_VERTICAL}
{
    m_icon.set_from_icon_name(config::APPLICATION_ID + "-symbolic", Gtk::ICON_SIZE_BUTTON);
    m_icon.set_pixel_size(370);

    m_label.set_label(_("Open a document to start slicing!"));
    m_label.get_style_context()->add_class("dim-label");
    m_label.get_style_context()->add_class("welcome-label");
    m_label.set_margin_top(25);

    m_box.set_orientation(Gtk::ORIENTATION_VERTICAL);
    m_box.set_valign(Gtk::ALIGN_CENTER);
    m_box.pack_start(m_icon, false, false);
    m_box.pack_start(m_label, false, false);

    pack_start(m_box); // NOLINT
}

} // namespace Slicer
