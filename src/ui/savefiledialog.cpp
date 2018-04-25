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
