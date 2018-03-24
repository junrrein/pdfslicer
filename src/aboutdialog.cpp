#include "aboutdialog.hpp"

namespace Slicer {

AboutDialog::AboutDialog()
{
    set_program_name("PDF Slicer");
    set_logo_icon_name("edit-cut-symbolic");
    set_version("0.9");
    set_copyright("Copyright © 2017 Julián Unrrein");
    set_license_type(Gtk::LICENSE_GPL_3_0_ONLY);
}

AboutDialog::~AboutDialog()
{
}

} // namespace Slicer
