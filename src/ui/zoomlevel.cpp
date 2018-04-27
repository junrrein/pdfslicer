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

namespace Slicer {

bool isNonPositive(int number)
{
    return number <= 0;
}

ZoomLevel::ZoomLevel(const std::set<int>& levels)
    : m_levels{levels}
    , m_currentLevel{*levels.begin()}
{
    if (m_levels.empty())
        throw std::runtime_error("There has to be at least one zoom level");

    if (ranges::any_of(m_levels, isNonPositive))
        throw std::runtime_error("Zoom levels have to be greater than zero");
}

int ZoomLevel::currentLevel() const
{
    return m_currentLevel;
}

int ZoomLevel::minLevel() const
{
    return *m_levels.begin();
}

int ZoomLevel::maxLevel() const
{
    return *std::prev(m_levels.end());
}

int ZoomLevel::operator++()
{
    if (currentLevel() != maxLevel()) {
        auto it = m_levels.find(currentLevel());
        m_currentLevel = *std::next(it);
        changed.emit(currentLevel());
    }

    return currentLevel();
}

int ZoomLevel::operator--()
{
    if (currentLevel() != minLevel()) {
        auto it = m_levels.find(currentLevel());
        m_currentLevel = *std::prev(it);
        changed.emit(currentLevel());
    }

    return currentLevel();
}

} // namespace Slicer
