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

private:
    Gio::ActionMap& m_actionMap;

    Glib::RefPtr<Gio::SimpleAction> m_zoomInAction;
    Glib::RefPtr<Gio::SimpleAction> m_zoomOutAction;

    void onZoomInAction();
    void onZoomOutAction();
};

} // namespace Slicer

#endif // ZOOMLEVELWITHACTIONS_HPP
