#include "logger.hpp"
#include <glibmm/miscutils.h>
#include <config.hpp>

namespace Slicer::Logger {

Slicer::LoggerInstance setupLogger()
{
    auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(getPathToLogFile(), true);
    auto logger = std::make_shared<spdlog::logger>("default",
                                                   spdlog::sinks_init_list{consoleSink, fileSink});

    spdlog::register_logger(logger);

    return {consoleSink, fileSink, logger};
}

std::string getPathToLogFile()
{
    return Glib::build_filename(std::vector<std::string>{Glib::get_user_config_dir(),
                                                         Slicer::config::APPLICATION_ID,
                                                         "log.txt"});
}

void logInfo(std::string str)
{
    spdlog::get("default")->info(str);
}

void logWarning(std::string str)
{
    spdlog::get("default")->warn(str);
}

void logError(std::string str)
{
    spdlog::get("default")->error(str);
}
}
