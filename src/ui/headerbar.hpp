#ifndef HEADERBAR_HPP
#define HEADERBAR_HPP

#include <gtkmm/headerbar.h>
#include <gtkmm/applicationwindow.h>
#include <gtkmm/menubutton.h>
#include <gtkmm/modelbutton.h>
#include <gtkmm/box.h>

namespace Slicer {

class HeaderBar : public Gtk::HeaderBar {
public:
    HeaderBar(Gtk::ApplicationWindow& window);

private:
    Gtk::Button m_buttonOpen;
    Gtk::Button m_buttonSave;
    Gtk::Box m_boxRemovePages;
    Gtk::Button m_buttonRemovePages;
    Gtk::MenuButton m_buttonRemoveOptions;
    Gtk::Box m_boxMenuRemoveOptions;
    Gtk::ModelButton m_buttonRemovePrevious;
    Gtk::ModelButton m_buttonRemoveNext;
    Gtk::Button m_buttonUndo;
    Gtk::Button m_buttonRedo;
    Gtk::Button m_buttonPreviewPage;
    Gtk::Button m_buttonZoomOut;
    Gtk::Button m_buttonZoomIn;
    Gtk::Box m_boxZoom;
};

} // namespace Slicer

#endif // HEADERBAR_HPP
