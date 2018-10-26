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

#include "tempfile.hpp"
#include <glibmm/miscutils.h>
#include <uuid.h>

namespace Slicer {

Glib::RefPtr<Gio::File> generateTempFile()
{
    const std::string path = Glib::build_filename(Glib::get_tmp_dir(),
                                                  uuids::to_string(uuids::uuid_system_generator{}()));

    return Gio::File::create_for_path(path);
}
}
