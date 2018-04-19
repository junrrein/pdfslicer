#include "zoomlevelwithactions.hpp"

namespace Slicer {

ZoomLevelWithActions::ZoomLevelWithActions(Gtk::ApplicationWindow& window,
                                           const std::set<int>& levels)
    : ZoomLevel{levels}
    , m_window{window}
{
    m_zoomInAction = m_window.add_action("zoom-in", sigc::mem_fun(*this, &ZoomLevelWithActions::onZoomInAction));
    m_zoomOutAction = m_window.add_action("zoom-out", sigc::mem_fun(*this, &ZoomLevelWithActions::onZoomOutAction));

    m_zoomInAction->set_enabled();
    m_zoomOutAction->set_enabled(false);
}

ZoomLevelWithActions::~ZoomLevelWithActions()
{
    m_window.remove_action("zoom-in");
    m_window.remove_action("zoom-out");
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
