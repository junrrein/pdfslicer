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

#ifndef GUICOMMAND_HPP
#define GUICOMMAND_HPP

#include <command.hpp>
#include "headerbar.hpp"
#include "view.hpp"

namespace Slicer {

class GuiAddFileCommand : public AddFileCommand {
public:
	GuiAddFileCommand(Document& document,
					  const Glib::RefPtr<Gio::File>& file,
					  unsigned int position,
					  HeaderBar& headerBar,
					  View& view);

	virtual void execute() override;
	virtual void undo() override;
	virtual void redo() override;

private:
	HeaderBar& m_headerBar;
	View& m_view;
	const Glib::ustring m_fileName;
	const Glib::ustring m_oldSubtitle;

	void executeInternal();
	void setSubtitle();
};

}

#endif // GUICOMMAND_HPP
