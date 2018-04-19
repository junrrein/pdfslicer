#ifndef ZOOMLEVELWITHACTIONS_HPP
#define ZOOMLEVELWITHACTIONS_HPP

#include "zoomlevel.hpp"
#include <gtkmm/applicationwindow.h>
#include <giomm/simpleaction.h>

namespace Slicer {

class ZoomLevelWithActions : public ZoomLevel {
public:
    ZoomLevelWithActions() = delete;
    ZoomLevelWithActions(Gtk::ApplicationWindow& window,
                         const std::set<int>& levels);
    ~ZoomLevelWithActions();

private:
    Gtk::ApplicationWindow& m_window;

    Glib::RefPtr<Gio::SimpleAction> m_zoomInAction;
    Glib::RefPtr<Gio::SimpleAction> m_zoomOutAction;

    void onZoomInAction();
    void onZoomOutAction();
};

} // namespace Slicer

#endif // ZOOMLEVELWITHACTIONS_HPP
