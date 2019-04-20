// PDF Slicer
// Copyright (C) 2018 Julián Unrrein

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

#include "config.hpp"
#include <glibmm/i18n.h>
#include <giomm/file.h>
#include <glibmm/miscutils.h>
#include <gtkmm/main.h>
#include <iostream>

namespace Slicer::config {

std::string getPathToLocaleDir()
{
    return config::LINUX_LOCALE_DIR;
}

void setupLocalization()
{
    bindtextdomain(GETEXT_PACKAGE.c_str(),
                   getPathToLocaleDir().c_str());
    bind_textdomain_codeset(GETEXT_PACKAGE.c_str(),
                            "UTF-8");
    textdomain(GETEXT_PACKAGE.c_str());
}

std::string getConfigDirPath()
{
    return Glib::build_filename(Glib::get_user_config_dir(),
                                APPLICATION_ID);
}

std::string getTempDirPath()
{
    return Glib::build_filename(Glib::get_tmp_dir(),
                                APPLICATION_ID);
}

void createSlicerDirsIfNotExistent()
{
    try {
        Gtk::Main::init_gtkmm_internals();

        auto settingsDirectory = Gio::File::create_for_path(getConfigDirPath());
        auto tempDirectory = Gio::File::create_for_path(getTempDirPath());

        if (!settingsDirectory->query_exists())
            settingsDirectory->make_directory_with_parents();

        if (!tempDirectory->query_exists())
            tempDirectory->make_directory_with_parents();
    }
    catch (const Glib::Error& e) {
        std::cerr << "Couldn't create config dir or temp dir with error: " << '\n'
                  << e.what() << std::endl;
    }
}

}
