#ifndef PREVIEWWINDOW_HPP
#define PREVIEWWINDOW_HPP

#include "document.hpp"
#include <gtkmm/window.h>
#include <gtkmm/image.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/overlay.h>
#include <gtkmm/button.h>
#include <gtkmm/box.h>

namespace Slicer {

class PreviewWindow : public Gtk::Window {
public:
    PreviewWindow(Glib::RefPtr<Page>& gPage);
    virtual ~PreviewWindow(){};

private:
    enum class ZoomLevel {
        small = 800,
        medium = 1200,
        large = 1600
    };

    Glib::RefPtr<Page> m_gPage;
    Gtk::ScrolledWindow m_scroller;
    Gtk::Overlay m_overlay;
    Gtk::Button m_buttonZoomIn;
    Gtk::Button m_buttonZoomOut;
    Gtk::Box m_boxZoom;
    ZoomLevel m_zoomLevel;

    sigc::signal<void> m_signalZoomChanged;

    void increaseZoomLevel();
    void decreaseZoomLevel();
};

} // namespace Slicer

#endif // BIGPREVIEWWINDOW_HPP
