#ifndef PREVIEWWINDOW_HPP
#define PREVIEWWINDOW_HPP

#include "../backend/document.hpp"
#include "zoomlevel.hpp"
#include <gtkmm/window.h>
#include <gtkmm/image.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/overlay.h>
#include <gtkmm/button.h>
#include <gtkmm/box.h>

namespace Slicer {

class PreviewWindow : public Gtk::Window {
public:
    PreviewWindow(const Glib::RefPtr<Page>& page);
    virtual ~PreviewWindow() = default;

private:
    Glib::RefPtr<Page> m_page;
    Gtk::Image m_image;
    Gtk::ScrolledWindow m_scroller;
    Gtk::Overlay m_overlay;
    Gtk::Button m_buttonZoomIn;
    Gtk::Button m_buttonZoomOut;
    Gtk::Box m_boxZoom;
    ZoomLevel m_zoomLevel;
};

} // namespace Slicer

#endif // BIGPREVIEWWINDOW_HPP
