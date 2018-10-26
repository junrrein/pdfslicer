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

#include "logger.hpp"
#include <glibmm/miscutils.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <config.hpp>
#include <iostream>

namespace Slicer::Logger {

void setupLogger()
{
    static const int logFileSize = 1024 * 1024 * 2; // 2 MB
    static const int numberOfLogFiles = 3;

    try {
        auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto fileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(getPathToLogFile(),
                                                                               logFileSize,
                                                                               numberOfLogFiles);
        auto logger = std::make_shared<spdlog::logger>("default",
                                                       spdlog::sinks_init_list{consoleSink, fileSink});

        spdlog::register_logger(logger);
    }
    catch (const spdlog::spdlog_ex& e) {
        std::cerr << "Couldn't initialize logger with error:" << '\n'
                  << e.what() << std::endl;
    }
}

std::string getPathToLogFile()
{
    return Glib::build_filename(config::getConfigDirPath(), "log.txt");
}

void logInfo(std::string str)
{
    if (auto logger = spdlog::get("default"); logger != nullptr)
        logger->info(str);
}

void logWarning(std::string str)
{
    if (auto logger = spdlog::get("default"); logger != nullptr)
        logger->warn(str);
}

void logError(std::string str)
{
    if (auto logger = spdlog::get("default"); logger != nullptr)
        logger->error(str);
}
}
