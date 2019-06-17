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
#include <gtkmm/box.h>

namespace Slicer {

HeaderBar::HeaderBar(const Glib::PropertyProxy<unsigned>& zoomIndexProperty)
    : m_appMenu{zoomIndexProperty}
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

void HeaderBar::enableZoomSlider()
{
    m_appMenu.enableZoomSlider();
}

void HeaderBar::disableZoomSlider()
{
    m_appMenu.disableZoomSlider();
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

    m_buttonAppMenu.set_popover(m_appMenu);
    m_buttonAppMenu.set_image_from_icon_name("open-menu-symbolic");
    pack_end(m_buttonAppMenu);

    m_buttonSave.set_label(_("Save As…"));
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonSave.gobj()), "win.save-document"); // NOLINT
    pack_end(m_buttonSave);
}

} // namespace Slicer
