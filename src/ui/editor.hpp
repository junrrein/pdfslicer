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

#ifndef EDITOR_HPP
#define EDITOR_HPP

#include "../backend/document.hpp"
#include "view.hpp"
#include "actionbar.hpp"
#include <gtkmm/box.h>
#include <giomm/actionmap.h>
#include <gtkmm/scrolledwindow.h>

namespace Slicer {

class Editor : public Gtk::Box {
public:
    Editor(Gio::ActionMap& actionMap);

    void setDocument(Document& document);
    void waitForRenderCompletion();

private:
    View m_view;
    Gtk::ScrolledWindow m_scroller;
    ActionBar m_actionBar;
};

} // namespace Slicer

#endif // EDITOR_HPP
