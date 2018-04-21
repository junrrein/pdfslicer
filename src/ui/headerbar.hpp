#ifndef HEADERBAR_HPP
#define HEADERBAR_HPP

#include <gtkmm/headerbar.h>
#include <giomm/actiongroup.h>
#include <gtkmm/menubutton.h>
#include <gtkmm/modelbutton.h>
#include <gtkmm/box.h>

namespace Slicer {

class HeaderBar : public Gtk::HeaderBar {
public:
    HeaderBar();

private:
    Gtk::Button m_buttonOpen;
    Gtk::Button m_buttonSave;
    Gtk::Button m_buttonUndo;
    Gtk::Button m_buttonRedo;
    Gtk::Button m_buttonPreviewPage;
    Gtk::Button m_buttonZoomOut;
    Gtk::Button m_buttonZoomIn;
    Gtk::Box m_boxZoom;

    void setupWidgets();
};

} // namespace Slicer

#endif // HEADERBAR_HPP
