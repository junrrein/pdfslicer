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
