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

#ifndef APPMENU_HPP
#define APPMENU_HPP

#include <glibmm/binding.h>
#include <gtkmm/adjustment.h>
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/modelbutton.h>
#include <gtkmm/scale.h>
#include <gtkmm/separator.h>
#include <gtkmm/popovermenu.h>

namespace Slicer {

class AppMenu : public Gtk::PopoverMenu {
public:
	AppMenu(const Glib::PropertyProxy<unsigned>& zoomIndexProperty);

	void enableZoomSlider();
	void disableZoomSlider();

private:
    Gtk::Box m_contentBox;

	Gtk::Box m_zoomBox;
	Gtk::Box m_zoomSeparatorBox;
	Gtk::Label m_zoomSeparatorLabel;
	Gtk::Separator m_zoomSeparatorLeft;
	Gtk::Separator m_zoomSeparatorRight;
	Gtk::Label m_zoomLabel;
	Gtk::Scale m_zoomSlider;
	Glib::RefPtr<Gtk::Adjustment> m_zoomAdjustment;
	Glib::RefPtr<Glib::Binding> m_zoomBinding;

    Gtk::Box m_appBox;
    Gtk::Separator m_appSeparator;
    Gtk::ModelButton m_shortcutsButton;
    Gtk::ModelButton m_aboutButton;

	void setupWidgets();
};

} // namespace Slicer

#endif // APPMENU_HPP
