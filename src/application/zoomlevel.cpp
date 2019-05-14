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

#include "zoomlevel.hpp"
#include <range/v3/algorithm.hpp>
#include <range/v3/action.hpp>

namespace Slicer {

bool isNonPositive(int number)
{
    return number <= 0;
}

ZoomLevel::ZoomLevel(const std::vector<int>& levels)
    : m_zoomLevelIndex{*this, "zoom-level-index", 0}
    , m_levels{levels}
{
    m_levels |= ranges::action::sort | ranges::action::unique;

    if (m_levels.empty())
        throw std::runtime_error("There has to be at least one zoom level");

    if (ranges::any_of(m_levels, isNonPositive))
        throw std::runtime_error("Zoom levels have to be greater than zero");
}

int ZoomLevel::currentLevel() const
{
    return m_levels.at(m_zoomLevelIndex.get_value());
}

int ZoomLevel::minLevel() const
{
    return m_levels.front();
}

int ZoomLevel::maxLevel() const
{
    return m_levels.back();
}

int ZoomLevel::operator++()
{
    if (currentLevel() != maxLevel())
        m_zoomLevelIndex.set_value(m_zoomLevelIndex.get_value() + 1);

    return currentLevel();
}

int ZoomLevel::operator--()
{
    if (currentLevel() != minLevel())
        m_zoomLevelIndex.set_value(m_zoomLevelIndex.get_value() - 1);

    return currentLevel();
}

Glib::PropertyProxy<unsigned> ZoomLevel::zoomLevelIndex()
{
    return m_zoomLevelIndex.get_proxy();
}

} // namespace Slicer
