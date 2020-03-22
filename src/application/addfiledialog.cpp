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

#include "addfiledialog.hpp"
#include "pdffilter.hpp"
#include <glibmm/i18n.h>

namespace Slicer {

AddFileDialog::AddFileDialog(Gtk::Window& parent,
                             std::optional<std::string> folderPath)
    : Gtk::FileChooserNative{_("Select documents to add"),
                             parent,
                             Gtk::FILE_CHOOSER_ACTION_OPEN,
                             _("Add"),
                             _("Cancel")}
{
    set_select_multiple(true);
    add_filter(pdfFilter());

    if (folderPath.has_value())
        set_current_folder(folderPath.value());
}

} // namespace Slicer
