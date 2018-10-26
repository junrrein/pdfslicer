#include "config.hpp"
#include <glibmm/i18n.h>
#include <giomm/file.h>
#include <glibmm/miscutils.h>
#include <iostream>

namespace Slicer::config {

std::string getPathToLocaleDir()
{
#ifdef __linux__
    return config::LINUX_LOCALE_DIR;
#else // We are in Windows
    const std::string pathToShareDir = Glib::get_system_data_dirs().at(2);
    const std::string pathToLocaleDir = pathToShareDir + +"\\locale\\";

    return pathToLocaleDir;
#endif
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

void createConfigDirIfNotExistent()
{
    try {
        auto settingsDirectory = Gio::File::create_for_path(getConfigDirPath());

        if (!settingsDirectory->query_exists())
            settingsDirectory->make_directory_with_parents();
    }
    catch (...) {
        std::cerr << "Couldn't create the config dir at path: " << '\n'
                  << getConfigDirPath() << std::endl;
    }
}
}
