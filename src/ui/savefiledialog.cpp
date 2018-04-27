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
    : Gtk::FileChooserDialog{parent,
                             _("Save document as"),
                             Gtk::FILE_CHOOSER_ACTION_SAVE}
{
	set_transient_for(parent);
    set_current_name(_("Untitled document") + Glib::ustring{".pdf"});
	set_filter(pdfFilter());
    set_do_overwrite_confirmation();

    add_button(_("Cancel"), Gtk::RESPONSE_CANCEL);
    add_button(_("Save"), Gtk::RESPONSE_OK);
}

std::string SaveFileDialog::getSavePath() const
{
	std::string filePath = get_filename();
	const Glib::ustring baseName = get_file()->get_basename();

	if (baseName.find(".pdf") != baseName.size() - 4)
		filePath += ".pdf";

	return filePath;
}

} // namespace Slicer
