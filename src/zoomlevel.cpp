#include "zoomlevel.hpp"

#include <algorithm>

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

    if (std::any_of(m_levels.begin(), m_levels.end(), isNonPositive))
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
    return *(--m_levels.end());
}

int ZoomLevel::operator++()
{
    if (currentLevel() != maxLevel()) {
        auto it = m_levels.find(currentLevel());
        m_currentLevel = *(++it);
        changed.emit(currentLevel());
    }

    return currentLevel();
}

int ZoomLevel::operator--()
{
    if (currentLevel() != minLevel()) {
        auto it = m_levels.find(m_currentLevel);
        m_currentLevel = *(--it);
        changed.emit(currentLevel());
    }

    return currentLevel();
}

} // namespace Slicer
