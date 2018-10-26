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
