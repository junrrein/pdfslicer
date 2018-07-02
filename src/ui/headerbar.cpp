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

#include "headerbar.hpp"
#include <glibmm/i18n.h>
#include <giomm/menu.h>

namespace Slicer {

HeaderBar::HeaderBar()
{
    set_title("PDF Slicer");
    set_show_close_button();

    setupWidgets();
}

void HeaderBar::setupWidgets()
{
    m_buttonOpen.set_image_from_icon_name("document-open-symbolic");
    m_buttonOpen.set_tooltip_text(_("Open document…"));
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonOpen.gobj()), "win.open-document"); // NOLINT
    pack_start(m_buttonOpen);

    m_buttonSave.set_image_from_icon_name("document-save-symbolic");
    m_buttonSave.set_tooltip_text(_("Save as…"));
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonSave.gobj()), "win.save-document"); // NOLINT
    pack_start(m_buttonSave);

    m_buttonUndo.set_image_from_icon_name("edit-undo-symbolic");
    m_buttonUndo.set_tooltip_text(_("Undo"));
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonUndo.gobj()), "win.undo"); // NOLINT
    m_buttonRedo.set_image_from_icon_name("edit-redo-symbolic");
    m_buttonRedo.set_tooltip_text(_("Redo"));
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonRedo.gobj()), "win.redo"); // NOLINT

    auto undoBox = Gtk::manage(new Gtk::Box); //NOLINT
    undoBox->get_style_context()->add_class("linked");
    undoBox->pack_start(m_buttonUndo);
    undoBox->pack_start(m_buttonRedo);
    pack_start(*undoBox);

    Glib::RefPtr<Gio::Menu> menu = Gio::Menu::create();
    menu->append(_("About"), "win.about");
    m_buttonAppMenu.set_image_from_icon_name("open-menu-symbolic");
    m_buttonAppMenu.set_menu_model(menu);
    pack_end(m_buttonAppMenu);

    m_buttonPreviewPage.set_image_from_icon_name("document-print-preview-symbolic");
    m_buttonPreviewPage.set_tooltip_text(_("Preview the selected page"));
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonPreviewPage.gobj()), "win.preview-selected"); // NOLINT
    pack_end(m_buttonPreviewPage);

    m_buttonZoomOut.set_image_from_icon_name("zoom-out-symbolic");
    m_buttonZoomOut.set_tooltip_text(_("Zoom out"));
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonZoomOut.gobj()), "win.zoom-out"); // NOLINT
    m_buttonZoomIn.set_image_from_icon_name("zoom-in-symbolic");
    m_buttonZoomIn.set_tooltip_text(_("Zoom in"));
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonZoomIn.gobj()), "win.zoom-in"); // NOLINT
    m_boxZoom.pack_start(m_buttonZoomOut);
    m_boxZoom.pack_start(m_buttonZoomIn);
    m_boxZoom.get_style_context()->add_class("linked");
    pack_end(m_boxZoom);
}

} // namespace Slicer
