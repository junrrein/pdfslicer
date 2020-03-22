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

#include "guicommand.hpp"
#include <glibmm/convert.h>
#include <glibmm/i18n.h>
#include <fmt/format.h>
#include <fmt/ostream.h>

using namespace fmt::literals;

namespace Slicer {

GuiAddFilesCommand::GuiAddFilesCommand(Document& document,
                                       const std::vector<Glib::RefPtr<Gio::File>>& files,
                                       unsigned int position,
                                       HeaderBar& headerBar,
                                       View& view)
    : AddFilesCommand{document, files, position}
    , m_headerBar{headerBar}
    , m_view{view}
    , m_oldSubtitle{headerBar.get_subtitle()}
{
}

void GuiAddFilesCommand::execute()
{
    AddFilesCommand::execute();
    executeInternal();
}

void GuiAddFilesCommand::undo()
{
    AddFilesCommand::undo();
    m_headerBar.set_subtitle(m_oldSubtitle);

    if (m_oldSubtitle.empty())
        m_view.setShowFileNames(false);
}

void GuiAddFilesCommand::redo()
{
    AddFilesCommand::redo();
    executeInternal();
}

void GuiAddFilesCommand::executeInternal()
{
    setSubtitle();
    m_view.setShowFileNames(true);
    m_view.selectPageRange(m_position, m_position + m_numberOfAddedPages - 1);
}

void GuiAddFilesCommand::setSubtitle()
{
    if (m_oldSubtitle.empty() && m_files.size() == 1) {
        Glib::ustring filename = Glib::filename_display_basename(m_files.at(0)->get_path());

        m_headerBar.set_subtitle(fmt::format(_("Added file {fileName}"),
                                             "fileName"_a = filename)); //NOLINT
    }
    else {
        m_headerBar.set_subtitle(_("Multiple files added"));
    }
}

}
