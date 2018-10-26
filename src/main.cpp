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
#include <logger.hpp>
#include <config.hpp>
#include <gtkmm/main.h>

using namespace Slicer;

int main(int num_args, char* args_array[])
{
    config::setupLocalization();
    config::createConfigDirIfNotExistent();
    Logger::setupLogger();

    Logger::logInfo("Welcome to PDF Slicer");
    Logger::logInfo("Logging to file: " + Logger::getPathToLogFile());

    auto app = Application::create();

    return app->run(num_args, args_array);
}
