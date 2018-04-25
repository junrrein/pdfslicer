#include "openfiledialog.hpp"
#include "utils.hpp"
#include <glibmm/i18n.h>

namespace Slicer {

OpenFileDialog::OpenFileDialog(Gtk::Window& parent)
    : Gtk::FileChooserDialog{parent, _("Open Document"), Gtk::FILE_CHOOSER_ACTION_OPEN}
{
	set_transient_for(parent);
	set_select_multiple(false);
	set_filter(pdfFilter());

    add_button(_("Cancel"), Gtk::RESPONSE_CANCEL);
    add_button(_("Open"), Gtk::RESPONSE_OK);
}

} // namespace Slicer
