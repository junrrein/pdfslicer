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

#include "savefiledialog.hpp"
#include "utils.hpp"
#include <glibmm/i18n.h>

namespace Slicer {

SaveFileDialog::SaveFileDialog(Gtk::Window& parent)
    : FileChooserNative{parent,
                        _("Save document as"),
                        GTK_FILE_CHOOSER_ACTION_SAVE,
                        _("Save"),
                        _("Cancel")}
{
    set_current_name(_("Untitled document") + Glib::ustring{".pdf"});
    add_filter(pdfFilter());
    set_do_overwrite_confirmation(true);
}

} // namespace Slicer
