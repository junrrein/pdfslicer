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

#include "editor.hpp"

namespace Slicer {

Editor::Editor(Gio::ActionMap& actionMap)
    : Gtk::Box{Gtk::ORIENTATION_VERTICAL}
    , m_view{actionMap, m_actionBar}
{
    m_scroller.add(m_view);
    pack_start(m_scroller);
    pack_start(m_actionBar, Gtk::PACK_SHRINK);
}

void Editor::setDocument(Document& document)
{
    m_view.setDocument(document);
}

void Editor::waitForRenderCompletion()
{
    m_view.waitForRenderCompletion();
}

} // namespace Slicer
