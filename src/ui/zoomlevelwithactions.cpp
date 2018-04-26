#include "zoomlevelwithactions.hpp"

namespace Slicer {

ZoomLevelWithActions::ZoomLevelWithActions(const std::set<int>& levels, Gio::ActionMap& actionMap)
    : ZoomLevel{levels}
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
    m_zoomInAction->set_enabled();
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
