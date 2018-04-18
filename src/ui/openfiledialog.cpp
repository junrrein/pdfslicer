#include "openfiledialog.hpp"
#include "utils.hpp"

namespace Slicer {

OpenFileDialog::OpenFileDialog(Gtk::Window& parent)
	: Gtk::FileChooserDialog{parent, "Open Document", Gtk::FILE_CHOOSER_ACTION_OPEN}
{
	set_transient_for(parent);
	set_select_multiple(false);
	set_filter(pdfFilter());

	add_button("Cancel", Gtk::RESPONSE_CANCEL);
	add_button("Open", Gtk::RESPONSE_OK);
}

} // namespace Slicer
