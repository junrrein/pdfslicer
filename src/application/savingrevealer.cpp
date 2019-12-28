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

#include "savingrevealer.hpp"
#include <glibmm/main.h>
#include <glibmm/i18n.h>

namespace Slicer {

SavingRevealer::SavingRevealer()
{
    m_labelSaving.set_label(_("Saving document…"));
    m_labelSaving.set_padding(10, -1);
    m_spinner.set_size_request(22, 22);
    m_spinner.set_margin_right(3);
    m_boxSaving.pack_start(m_labelSaving);
    m_boxSaving.pack_start(m_spinner);
    m_boxSaving.set_size_request(-1, 34);

    m_labelDone.set_label(_("Document succesfully saved"));
    m_labelDone.set_padding(10, -1);
    m_closeButton.set_image_from_icon_name("window-close-symbolic");
    m_closeButton.get_style_context()->add_class("flat");
    m_boxDone.pack_start(m_labelDone);
    m_boxDone.pack_start(m_closeButton);

    m_outerFrame.get_style_context()->add_class("app-notification");
    add(m_outerFrame);

    set_halign(Gtk::ALIGN_CENTER);
    set_valign(Gtk::ALIGN_START);

    m_closeButton.signal_clicked().connect([this]() {
        set_reveal_child(false);
    });
}

void SavingRevealer::saving()
{
    m_outerFrame.remove();
    m_outerFrame.add(m_boxSaving);
    m_boxSaving.show_all();
    m_spinner.start();

    // Prevent any previous previous saving operation started
    // from hiding the popup prematurely
    if (m_connectionSaved.connected())
        m_connectionSaved.disconnect();

    set_reveal_child(true);
}

void SavingRevealer::saved()
{
    m_outerFrame.remove();
    m_outerFrame.add(m_boxDone);
    m_boxDone.show_all();
    m_spinner.stop();

    // Prevent any previous previous saving operation started
    // from hiding the popup prematurely
    if (m_connectionSaved.connected())
        m_connectionSaved.disconnect();

    // Automatically hide the notification after a while
    m_connectionSaved = Glib::signal_timeout().connect([this]() {
        set_reveal_child(false);
        return false;
    },
                                                       5000);
}

} // namespace Slicer
