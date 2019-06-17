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
#include <fileutils.hpp>
#include <glibmm/i18n.h>
#include <fmt/format.h>
#include <fmt/ostream.h>

using namespace fmt::literals;

namespace Slicer {

GuiAddFileCommand::GuiAddFileCommand(Document& document,
                                     const Glib::RefPtr<Gio::File>& file,
                                     unsigned int position,
                                     HeaderBar& headerBar,
                                     View& view)
    : AddFileCommand{document, file, position}
    , m_headerBar{headerBar}
    , m_view{view}
    , m_fileName{getDisplayName(file)}
    , m_oldSubtitle{headerBar.get_subtitle()}
{
}

void GuiAddFileCommand::execute()
{
    AddFileCommand::execute();
    executeInternal();
}

void GuiAddFileCommand::undo()
{
    AddFileCommand::undo();
    m_headerBar.set_subtitle(m_oldSubtitle);

    if (m_oldSubtitle == "")
        m_view.setShowFileNames(false);
}

void GuiAddFileCommand::redo()
{
    AddFileCommand::redo();
    executeInternal();
}

void GuiAddFileCommand::executeInternal()
{
    setSubtitle();
    m_view.setShowFileNames(true);
    m_view.selectPageRange(m_position, m_position + m_numberOfAddedPages - 1);
}

void GuiAddFileCommand::setSubtitle()
{
    if (m_oldSubtitle == "")
        m_headerBar.set_subtitle(fmt::format(_("Added file {fileName}"),
                                             "fileName"_a = m_fileName)); //NOLINT
    else
        m_headerBar.set_subtitle(_("Multiple files added"));
}

}
