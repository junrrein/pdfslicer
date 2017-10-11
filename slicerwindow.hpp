#ifndef SLICERWINDOW_HPP
#define SLICERWINDOW_HPP

#include "slicerview.hpp"
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/headerbar.h>
#include <gtkmm/button.h>
#include <gtkmm/overlay.h>
#include <gtkmm/revealer.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <gtkmm/popovermenu.h>

namespace Slicer {

class Window : public Gtk::Window {
public:
    Window(std::string filePath);
    virtual ~Window(){};

private:
    // Model
    Slicer::Document m_document;

    // Header bar
    Gtk::HeaderBar m_headerBar;
    Gtk::Button m_buttonSave;
    Gtk::Box m_boxRemovePages;
    Gtk::Button m_buttonRemovePages;
    Gtk::Button m_buttonRemoveOptions;
    Gtk::PopoverMenu m_menuRemoveOptions;

    // View
    Slicer::View m_view;
    Gtk::ScrolledWindow m_scroller;
    Gtk::Overlay m_overlay;

    Gtk::Revealer m_revealerDone;
    Gtk::Box m_boxDone;
    Gtk::Label m_labelDone;
    Gtk::Button m_buttonDoneClose;

    // Signals
    sigc::signal<void> m_signalSaved;
    sigc::connection m_connectionSaved;

    // Functions
    void removeSelectedPages();
    void removePreviousPages();
    void removeNextPages();

    // Callbacks
    void onSaveAction();
};
}

#endif // SLICERWINDOW_HPP
