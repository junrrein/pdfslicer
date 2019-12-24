// PDF Slicer
// Copyright (C) 2019 Julián Unrrein

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

#include "appmenu.hpp"
#include <glibmm/i18n.h>

namespace Slicer {

AppMenu::AppMenu(const Glib::PropertyProxy<unsigned>& zoomIndexProperty)
{
    setupWidgets();

    m_zoomBinding = Glib::Binding::bind_property(zoomIndexProperty,
                                                 m_zoomAdjustment->property_value(),
                                                 Glib::BINDING_BIDIRECTIONAL);
}

void AppMenu::enableZoomSlider()
{
    m_zoomSlider.set_sensitive();
}

void AppMenu::disableZoomSlider()
{
    m_zoomSlider.set_sensitive(false);
}

void AppMenu::setupWidgets()
{
    m_zoomSeparatorLeft.set_valign(Gtk::ALIGN_CENTER);
    m_zoomSeparatorRight.set_valign(Gtk::ALIGN_CENTER);
    m_zoomSeparatorLabel.set_label(_("Zoom"));
    m_zoomSeparatorLabel.get_style_context()->add_class("separator");
    m_zoomSeparatorBox.pack_start(m_zoomSeparatorLeft);
    m_zoomSeparatorBox.pack_start(m_zoomSeparatorLabel, Gtk::PACK_SHRINK, 5);
    m_zoomSeparatorBox.pack_start(m_zoomSeparatorRight);

    m_zoomAdjustment = Gtk::Adjustment::create(0, 0, 4, 1, 1);
    m_zoomSlider.set_adjustment(m_zoomAdjustment);
    m_zoomSlider.set_has_origin(false);
    m_zoomSlider.set_draw_value(false);
    m_zoomSlider.set_round_digits(0);
    m_zoomSlider.add_mark(0, Gtk::POS_BOTTOM, "");
    m_zoomSlider.add_mark(1, Gtk::POS_BOTTOM, "");
    m_zoomSlider.add_mark(2, Gtk::POS_BOTTOM, "");
    m_zoomSlider.add_mark(3, Gtk::POS_BOTTOM, "");
    m_zoomSlider.add_mark(4, Gtk::POS_BOTTOM, "");
    m_zoomSlider.set_size_request(150, -1);

    m_newWindowButton.set_label(_("New window"));
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_newWindowButton.gobj()), "app.new-window"); //NOLINT
    m_newWindowButton.get_style_context()->add_class("flat");
    m_newWindowButton.get_child()->set_halign(Gtk::ALIGN_START);
    m_shortcutsButton.set_label(_("Keyboard shortcuts"));
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_shortcutsButton.gobj()), "win.shortcuts"); //NOLINT
    m_shortcutsButton.get_style_context()->add_class("flat");
    m_shortcutsButton.get_child()->set_halign(Gtk::ALIGN_START);
    m_aboutButton.set_label(_("About PDF Slicer"));
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_aboutButton.gobj()), "win.about"); //NOLINT
    m_aboutButton.get_style_context()->add_class("flat");
    m_aboutButton.get_child()->set_halign(Gtk::ALIGN_START);

    m_contentBox.set_orientation(Gtk::ORIENTATION_VERTICAL);
    m_contentBox.property_margin().set_value(10);
    m_contentBox.pack_start(m_zoomSeparatorBox);
    m_contentBox.pack_start(m_zoomSlider);
    m_contentBox.pack_start(m_appSeparator, Gtk::PACK_EXPAND_PADDING, 5);
    m_contentBox.pack_start(m_newWindowButton);
    m_contentBox.pack_start(m_shortcutsButton);
    m_contentBox.pack_start(m_aboutButton);
    add(m_contentBox);
    show_all_children();
}

} // namespace Slicer
