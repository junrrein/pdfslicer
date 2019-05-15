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
#include <gtkmm/separator.h>
#include <gtkmm/stack.h>

namespace Slicer {

HeaderBar::HeaderBar(const Glib::PropertyProxy<unsigned>& zoomIndexProperty)
{
    set_title("PDF Slicer");
    set_show_close_button();

    setupWidgets();

    m_zoomBinding = Glib::Binding::bind_property(zoomIndexProperty,
                                                 m_zoomAdjustment->property_value(),
                                                 Glib::BINDING_BIDIRECTIONAL);
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
    m_zoomSlider.set_sensitive();
}

void HeaderBar::disableZoomSlider()
{
    m_zoomSlider.set_sensitive(false);
}

void HeaderBar::setupWidgets()
{
    m_buttonOpen.set_label(_("Open…"));
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonOpen.gobj()), "win.open-document"); // NOLINT
    pack_start(m_buttonOpen);

    Glib::RefPtr<Gio::Menu> addDocumentMenu = Gio::Menu::create();
    addDocumentMenu->append(_("At the beggining…"), "win.add-document-at-beginning");
    addDocumentMenu->append(_("At the end…"), "win.add-document-at-end");
    addDocumentMenu->append(_("After the selected page…"), "win.add-document-after-selected");
    m_buttonAddDocument.set_image_from_icon_name("list-add-symbolic");
    m_buttonAddDocument.set_tooltip_text(_("Add Document…"));
    m_buttonAddDocument.set_menu_model(addDocumentMenu);
    pack_start(m_buttonAddDocument);

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

    m_zoomAdjustment = Gtk::Adjustment::create(0, 0, 2, 1, 1);
    m_zoomSlider.set_adjustment(m_zoomAdjustment);
    m_zoomSlider.set_has_origin(false);
    m_zoomSlider.set_draw_value(false);
    m_zoomSlider.set_round_digits(0);
    m_zoomSlider.add_mark(0, Gtk::POS_BOTTOM, "");
    m_zoomSlider.add_mark(1, Gtk::POS_BOTTOM, "");
    m_zoomSlider.add_mark(2, Gtk::POS_BOTTOM, "");
    auto appMenuSeparator = Gtk::manage(new Gtk::Separator{});
    Gtk::Popover* appMenuPopover = m_buttonAppMenu.get_popover();
    auto appMenuStack = dynamic_cast<Gtk::Stack*>(appMenuPopover->get_child());
    auto appMenuBox = dynamic_cast<Gtk::Box*>(appMenuStack->get_visible_child());
    appMenuBox->pack_start(m_zoomSlider);
    appMenuBox->pack_start(*appMenuSeparator, Gtk::PACK_EXPAND_WIDGET, 5);
    appMenuBox->show_all();

    m_buttonSave.set_label(_("Save As…"));
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonSave.gobj()), "win.save-document"); // NOLINT
    pack_end(m_buttonSave);
}

} // namespace Slicer
