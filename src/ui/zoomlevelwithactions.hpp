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

#ifndef ZOOMLEVELWITHACTIONS_HPP
#define ZOOMLEVELWITHACTIONS_HPP

#include "zoomlevel.hpp"
#include <giomm/actionmap.h>
#include <giomm/simpleaction.h>

namespace Slicer {

class ZoomLevelWithActions : public ZoomLevel {
public:
    ZoomLevelWithActions() = delete;
    ZoomLevelWithActions(const std::set<int>& levels,
                         Gio::ActionMap& actionMap);
    ~ZoomLevelWithActions();

    void enable();
    void disable();

private:
    Gio::ActionMap& m_actionMap;

    Glib::RefPtr<Gio::SimpleAction> m_zoomInAction;
    Glib::RefPtr<Gio::SimpleAction> m_zoomOutAction;

    void onZoomInAction();
    void onZoomOutAction();
};

} // namespace Slicer

#endif // ZOOMLEVELWITHACTIONS_HPP
