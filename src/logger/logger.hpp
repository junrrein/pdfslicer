#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>

namespace Slicer {

namespace Logger {

    void setupLogger();
    void logInfo(std::string str);
    void logWarning(std::string str);
    void logError(std::string str);

    std::string getPathToLogFile();
}
}

#endif // LOGGER_HPP
