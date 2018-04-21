#ifndef PREVIEWWINDOW_HPP
#define PREVIEWWINDOW_HPP

#include "../backend/page.hpp"
#include "zoomlevelwithactions.hpp"
#include <gtkmm/window.h>
#include <giomm/simpleactiongroup.h>
#include <gtkmm/image.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/overlay.h>
#include <gtkmm/button.h>
#include <gtkmm/box.h>

namespace Slicer {

class PreviewWindow : public Gtk::Window {
public:
    PreviewWindow(Glib::RefPtr<Page> page);
    virtual ~PreviewWindow() = default;

private:
    Glib::RefPtr<Page> m_page;
    Glib::RefPtr<Gio::SimpleActionGroup> m_actionGroup;
    ZoomLevelWithActions m_zoomLevel;
    static const std::set<int> zoomLevels;

    Gtk::Image m_image;
    Gtk::ScrolledWindow m_scroller;
    Gtk::Overlay m_overlay;
    Gtk::Button m_buttonZoomIn;
    Gtk::Button m_buttonZoomOut;
    Gtk::Box m_boxZoom;

    void setupWidgets();
    void setupSignalHandlers();
    void loadCustomCSS();
};

} // namespace Slicer

#endif // BIGPREVIEWWINDOW_HPP
