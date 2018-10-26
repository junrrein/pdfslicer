#include "logger.hpp"
#include <glibmm/miscutils.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <config.hpp>
#include <iostream>

namespace Slicer::Logger {

void setupLogger()
{
    try {
        auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(getPathToLogFile(), true);
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
