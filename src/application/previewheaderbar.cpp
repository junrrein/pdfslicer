// PDF Slicer
// Copyright (C) 2017-2020 Julián Unrrein

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

#include "previewheaderbar.hpp"
#include <glibmm/i18n.h>
#include <gtkmm/box.h>

namespace Slicer {

PreviewHeaderBar::PreviewHeaderBar()
{
    set_title("Page preview");
    set_show_close_button();

    setupWidgets();
}

void PreviewHeaderBar::setupWidgets()
{
    m_buttonZoomOut.set_image_from_icon_name("zoom-out-symbolic");
    m_buttonZoomOut.set_tooltip_text(_("Zoom out"));
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonZoomOut.gobj()), "preview.zoom-out"); // NOLINT

    m_buttonZoomIn.set_image_from_icon_name("zoom-in-symbolic");
    m_buttonZoomIn.set_tooltip_text(_("Zoom in"));
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonZoomIn.gobj()), "preview.zoom-in"); // NOLINT

    auto zoomBox = Gtk::manage(new Gtk::Box); //NOLINT
    zoomBox->get_style_context()->add_class("linked");
    zoomBox->pack_start(m_buttonZoomOut);
    zoomBox->pack_start(m_buttonZoomIn);
    pack_start(*zoomBox);
}

} // namespace Slicer
