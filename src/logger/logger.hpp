#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace Slicer {

struct LoggerInstance {
    std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> consoleSink;
    std::shared_ptr<spdlog::sinks::basic_file_sink_mt> fileSink;
    std::shared_ptr<spdlog::logger> logger;
};

namespace Logger {

    LoggerInstance setupLogger();
    void logInfo(std::string str);
    void logWarning(std::string str);
    void logError(std::string str);

    std::string getPathToLogFile();
}
}

#endif // LOGGER_HPP
