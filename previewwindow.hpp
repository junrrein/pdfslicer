#ifndef PREVIEWWINDOW_HPP
#define PREVIEWWINDOW_HPP

#include <gtkmm/window.h>
#include <gtkmm/image.h>
#include <gtkmm/scrolledwindow.h>

namespace Slicer {

class PreviewWindow : public Gtk::Window {
public:
    PreviewWindow(){};
    PreviewWindow(Glib::RefPtr<Gdk::Pixbuf> previewPixbuf);
    virtual ~PreviewWindow(){};

private:
    Gtk::ScrolledWindow m_scroller;
    Gtk::Image m_previewImage;
};

} // namespace Slicer

#endif // BIGPREVIEWWINDOW_HPP
