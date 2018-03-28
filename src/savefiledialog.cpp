#include "savefiledialog.hpp"
#include "utils.hpp"

namespace Slicer {

SaveFileDialog::SaveFileDialog(Gtk::Window &parent)
	: Gtk::FileChooserDialog{parent,
							 "Save Document As",
							 Gtk::FILE_CHOOSER_ACTION_SAVE}
{
	set_transient_for(parent);
	set_current_name("Untitled document");
	set_filter(pdfFilter());

	add_button("Cancel", Gtk::RESPONSE_CANCEL);
	add_button("Save", Gtk::RESPONSE_OK);
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
