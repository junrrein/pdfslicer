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

void HeaderBar::enableAddDocumentButton()
{
    m_buttonAddDocument.set_sensitive();
}

void HeaderBar::disableAddDocumentButton()
{
    m_buttonAddDocument.set_sensitive(false);
}

void HeaderBar::setupWidgets()
{
    m_buttonOpen.set_label(_("Open…"));
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonOpen.gobj()), "win.open-document"); // NOLINT

    Glib::RefPtr<Gio::Menu> addDocumentMenu = Gio::Menu::create();
    addDocumentMenu->append(_("At the beggining…"), "win.add-document-at-beginning");
    addDocumentMenu->append(_("At the end…"), "win.add-document-at-end");
    addDocumentMenu->append(_("After the selected page…"), "win.add-document-after-selected");
    m_buttonAddDocument.set_image_from_icon_name("list-add-symbolic");
    m_buttonAddDocument.set_tooltip_text(_("Add Document…"));
    m_buttonAddDocument.set_menu_model(addDocumentMenu);

    auto openBox = Gtk::manage(new Gtk::Box{Gtk::ORIENTATION_HORIZONTAL});
    openBox->pack_start(m_buttonOpen);
    openBox->pack_start(m_buttonAddDocument);
    openBox->get_style_context()->add_class("linked");
    pack_start(*openBox);

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

    Glib::RefPtr<Gio::Menu> appMenu = Gio::Menu::create();
    appMenu->append(_("Keyboard shortcuts"), "win.shortcuts");
    appMenu->append(_("About"), "win.about");
    m_buttonAppMenu.set_image_from_icon_name("open-menu-symbolic");
    m_buttonAppMenu.set_menu_model(appMenu);
    pack_end(m_buttonAppMenu);

    m_buttonSave.set_label(_("Save As…"));
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonSave.gobj()), "win.save-document"); // NOLINT
    pack_end(m_buttonSave);

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
