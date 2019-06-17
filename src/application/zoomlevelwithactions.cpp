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

#include "zoomlevelwithactions.hpp"

namespace Slicer {

ZoomLevelWithActions::ZoomLevelWithActions(const std::vector<int>& levels, Gio::ActionMap& actionMap)
    : Glib::ObjectBase{typeid(ZoomLevelWithActions)}
    , ZoomLevel{levels}
    , m_actionMap{actionMap}
{
    m_zoomInAction = m_actionMap.add_action("zoom-in", sigc::mem_fun(*this, &ZoomLevelWithActions::onZoomInAction));
    m_zoomOutAction = m_actionMap.add_action("zoom-out", sigc::mem_fun(*this, &ZoomLevelWithActions::onZoomOutAction));

    m_zoomInAction->set_enabled(false);
    m_zoomOutAction->set_enabled(false);
}

ZoomLevelWithActions::~ZoomLevelWithActions()
{
    m_actionMap.remove_action("zoom-in");
    m_actionMap.remove_action("zoom-out");
}

void ZoomLevelWithActions::enable()
{
    if (currentLevel() == maxLevel())
        m_zoomInAction->set_enabled(false);
    else
        m_zoomInAction->set_enabled();

    if (currentLevel() == minLevel())
        m_zoomOutAction->set_enabled(false);
    else
        m_zoomOutAction->set_enabled();
}

void ZoomLevelWithActions::disable()
{
    m_zoomInAction->set_enabled(false);
    m_zoomOutAction->set_enabled(false);
}

void ZoomLevelWithActions::onZoomInAction()
{
    operator++();

    if (currentLevel() == maxLevel())
        m_zoomInAction->set_enabled(false);

    m_zoomOutAction->set_enabled();
}

void ZoomLevelWithActions::onZoomOutAction()
{
    operator--();

    if (currentLevel() == minLevel())
        m_zoomOutAction->set_enabled(false);

    m_zoomInAction->set_enabled();
}

} // namespace Slicer
