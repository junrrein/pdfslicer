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

#include "unsavedchangesdialog.hpp"
#include <glibmm/i18n.h>

namespace Slicer {

UnsavedChangesDialog::UnsavedChangesDialog(Gtk::Window& parentWindow)
    : Gtk::MessageDialog{parentWindow,
                         _("Save modified document before closing?"),
                         false,
                         Gtk::MESSAGE_WARNING,
                         Gtk::BUTTONS_NONE,
                         true}
{
    set_secondary_text("If you don't save, the changes will be permanently lost.");

    Gtk::Button* closeButton = add_button(_("Close without Saving"), Gtk::RESPONSE_CLOSE);
    add_button(_("Cancel"), Gtk::RESPONSE_CANCEL);
    add_button(_("Save As…"), Gtk::RESPONSE_YES);

    closeButton->get_style_context()->add_class("destructive-action");

    set_default_response(Gtk::RESPONSE_CANCEL);
}

}
