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

#include "tempfilepath.hpp"
#include <glibmm/fileutils.h>

#ifdef __linux__
#include <unistd.h>
#else
#include <glibmm/miscutils.h>
#endif

namespace Slicer {

TempFilePath::TempFilePath()
{
#ifdef __linux__
    m_fileHandle = Glib::file_open_tmp(m_path, "pdfslicer");
#else
    const std::string tempDirectory = Glib::get_tmp_dir();
    const std::string tempFileName = "pdfslicer-temp.pdf";
    const std::vector<std::string> pathParts = {tempDirectory, tempFileName};
    const std::string tempFilePath = Glib::build_filename(pathParts);

    m_path = tempFilePath;
#endif
}

TempFilePath::~TempFilePath()
{
#ifdef __linux__
    close(m_fileHandle);
#endif
}

const std::string& TempFilePath::get() const
{
    return m_path;
}

} // namespace Slicer
