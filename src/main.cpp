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

#include "application/application.hpp"
#include "logger.hpp"
#include <glibmm/i18n.h>
#include <glibmm/miscutils.h>
#include <config.hpp>

void setupLocalization();
std::string getPathToLocaleDir();

int main(int num_args, char* args_array[])
{
    using namespace Slicer;

    Logger::setupLogger();
    setupLocalization();

    Logger::logInfo("Logging to file: " + Logger::getPathToLogFile());

    auto app = Application::create();

    return app->run(num_args, args_array);
}

void setupLocalization()
{
    bindtextdomain(Slicer::config::GETEXT_PACKAGE.c_str(),
                   getPathToLocaleDir().c_str());
    bind_textdomain_codeset(Slicer::config::GETEXT_PACKAGE.c_str(),
                            "UTF-8");
    textdomain(Slicer::config::GETEXT_PACKAGE.c_str());
}

std::string getPathToLocaleDir()
{
#ifdef __linux__
    return Slicer::config::LOCALE_DIR;
#else // We are in Windows
    const std::string pathToShareDir = Glib::get_system_data_dirs().at(2);
    const std::string pathToLocaleDir = pathToShareDir + +"\\locale\\";

    return pathToLocaleDir;
#endif
}
