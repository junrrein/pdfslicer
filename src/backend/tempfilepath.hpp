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

#ifndef TEMPFILE_HPP
#define TEMPFILE_HPP

#include <giomm/file.h>
#include <string>

namespace Slicer {

class TempFilePath {
public:
    TempFilePath();
    ~TempFilePath();

    const std::string& get() const;

private:
#ifdef __linux__
    int m_fileHandle;
#endif
    std::string m_path;
};

} // namespace Slicer

#endif // TEMPFILE_HPP
