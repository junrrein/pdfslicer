// PDF Slicer
// Copyright (C) 2019 Julián Unrrein

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

#include "fileutils.hpp"

namespace Slicer {

Glib::ustring getDiplayNameWithoutExtension(const Glib::RefPtr<Gio::File>& file)
{
    const std::string attribute = G_FILE_ATTRIBUTE_STANDARD_DISPLAY_NAME;
    Glib::ustring result = file->query_info(attribute)->get_attribute_as_string(attribute);

    if (auto extensionIndex = result.rfind(".pdf"); extensionIndex != Glib::ustring::npos)
        result.erase(extensionIndex);

    return result;
}

}
