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

#include "actionbar.hpp"
#include <gtkmm/box.h>
#include <giomm/menu.h>
#include <glibmm/i18n.h>

namespace Slicer {

ActionBar::ActionBar()
{
    auto rotateBox = Gtk::manage(new Gtk::Box{Gtk::ORIENTATION_HORIZONTAL}); // NOLINT
    rotateBox->get_style_context()->add_class("linked");

    m_buttonRotateLeft.set_image_from_icon_name("object-rotate-left-symbolic");
    m_buttonRotateLeft.set_tooltip_text(_("Rotate left"));
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonRotateLeft.gobj()), "win.rotate-left"); // NOLINT
    rotateBox->pack_start(m_buttonRotateLeft);

    m_buttonRotateRight.set_image_from_icon_name("object-rotate-right-symbolic");
    m_buttonRotateRight.set_tooltip_text(_("Rotate right"));
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonRotateRight.gobj()), "win.rotate-right"); // NOLINT
    rotateBox->pack_start(m_buttonRotateRight);

    auto moveBox = Gtk::manage(new Gtk::Box{Gtk::ORIENTATION_HORIZONTAL}); // NOLINT
    moveBox->get_style_context()->add_class("linked");

    m_buttonMoveLeft.set_image_from_icon_name("go-previous-symbolic");
    m_buttonMoveLeft.set_tooltip_text(_("Move left"));
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonMoveLeft.gobj()), "win.move-left"); //NOLINT
    moveBox->pack_start(m_buttonMoveLeft);

    m_buttonMoveRight.set_image_from_icon_name("go-next-symbolic");
    m_buttonMoveRight.set_tooltip_text(_("Move right"));
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonMoveRight.gobj()), "win.move-right"); //NOLINT
    moveBox->pack_start(m_buttonMoveRight);

    auto removeBox = Gtk::manage(new Gtk::Box{Gtk::ORIENTATION_HORIZONTAL}); // NOLINT
    removeBox->get_style_context()->add_class("linked");

    m_buttonRemovePages.set_image_from_icon_name("user-trash-symbolic");
    m_buttonRemovePages.set_tooltip_text(_("Remove selected pages"));
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonRemovePages.gobj()), "win.remove-selected"); // NOLINT
    removeBox->pack_start(m_buttonRemovePages);

    Glib::RefPtr<Gio::Menu> menu = Gio::Menu::create();
    menu->append(_("Remove unselected pages"), "win.remove-unselected");
    menu->append(_("Remove previous pages"), "win.remove-previous");
    menu->append(_("Remove next pages"), "win.remove-next");
    m_buttonRemovePagesMore.set_image_from_icon_name("pan-up-symbolic");
    m_buttonRemovePagesMore.set_tooltip_text(_("More page removing operations…"));
    m_buttonRemovePagesMore.get_style_context()->remove_class("image-button");
    m_buttonRemovePagesMore.get_style_context()->add_class("disclosure-button");
    m_buttonRemovePagesMore.get_style_context()->add_class("thin-button");
    m_buttonRemovePagesMore.set_menu_model(menu);
    removeBox->pack_start(m_buttonRemovePagesMore);

    m_buttonSelectAll.set_label(_("Select All"));
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonSelectAll.gobj()), "win.select-all"); // NOLINT

    m_buttonCancelSelection.set_label(_("Cancel Selection"));
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonCancelSelection.gobj()), "win.cancel-selection"); // NOLINT

    pack_start(*moveBox);
    pack_start(*rotateBox);
    pack_start(*removeBox);
    pack_end(m_buttonCancelSelection);
    pack_end(m_buttonSelectAll);

    // The following is to work around a bug with themes other than Adwaita.
    // Without this, the ActionBar is too narrow and the buttons don't fit.
    set_size_request(-1, 45);
}

void ActionBar::enableButtonRemovePagesMore()
{
    m_buttonRemovePagesMore.set_sensitive();
}

void ActionBar::disableButtonRemovePagesMore()
{
    m_buttonRemovePagesMore.set_sensitive(false);
}

} // namespace Slicer
