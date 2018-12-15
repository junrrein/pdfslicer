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

#ifndef ZOOMLEVEL_HPP
#define ZOOMLEVEL_HPP

#include <set>
#include <sigc++/signal.h>

namespace Slicer {

class ZoomLevel {
public:
	ZoomLevel() = delete;
    ZoomLevel(const std::set<int>& levels);

    int currentLevel() const;
	int minLevel() const;
	int maxLevel() const;

	int operator++();
	int operator--();

    sigc::signal<void, int> changed;

private:
    const std::set<int> m_levels;
	int m_currentLevel;
};

} // namespace Slicer

#endif // ZOOMLEVEL_HPP
