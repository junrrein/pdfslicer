#include "aboutdialog.hpp"
#include <config.hpp>

namespace Slicer {

AboutDialog::AboutDialog(Gtk::Window& parent)
{
    set_program_name("PDF Slicer");
    set_logo_icon_name("edit-cut-symbolic");
    set_version(config::VERSION);
    set_copyright("Copyright © 2017 Julián Unrrein");
    set_license_type(Gtk::LICENSE_GPL_3_0_ONLY);

    signal_hide().connect([this]() {
        delete this;
    });

    set_transient_for(parent);
    set_modal();
    present();
}

} // namespace Slicer
