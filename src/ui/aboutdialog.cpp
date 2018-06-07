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

#include "aboutdialog.hpp"
#include <glibmm/i18n.h>
#include <config.hpp>

namespace Slicer {

AboutDialog::AboutDialog(Gtk::Window& parent)
{
    set_program_name(config::APPLICATION_NAME);
    set_comments(_("A simple application to extract pages of a PDF document"));
    set_logo_icon_name(config::APPLICATION_ID);
    set_version(config::VERSION);
    set_copyright("Copyright © 2017-2018 Julián Unrrein");
    set_license_type(Gtk::LICENSE_GPL_3_0);
    set_website(config::APPLICATION_WEBSITE);
    add_credit_section(_("Third-party libraries"),
                       {"CTPL https://github.com/vit-vit/CTPL",
                        "PDFWriter https://github.com/galkahana/PDF-Writer",
                        "Poppler https://poppler.freedesktop.org/",
                        "range-v3 https://github.com/ericniebler/range-v3"});

    signal_hide().connect([this]() {
        delete this;
    });

    set_transient_for(parent);
    set_modal();
    present();
}

} // namespace Slicer
