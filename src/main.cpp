// PDF Slicer
// Copyright (C) 2017-2018 Julián Unrrein

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

#include "application.hpp"
#include <glibmm/i18n.h>
#include <config.hpp>

int main(int num_args, char* args_array[])
{
    bindtextdomain(Slicer::config::GETEXT_PACKAGE.c_str(),
                   Slicer::config::LOCALE_DIR.c_str());
    bind_textdomain_codeset(Slicer::config::GETEXT_PACKAGE.c_str(),
                            "UTF-8");
    textdomain(Slicer::config::GETEXT_PACKAGE.c_str());

    auto app = Slicer::Application::create();

    return app->run(num_args, args_array);
}
