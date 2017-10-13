#ifndef SLICERWINDOW_HPP
#define SLICERWINDOW_HPP

#include "view.hpp"
#include "previewwindow.hpp"
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/headerbar.h>
#include <gtkmm/button.h>
#include <gtkmm/overlay.h>
#include <gtkmm/revealer.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <gtkmm/modelbutton.h>
#include <gtkmm/menubutton.h>

namespace Slicer {

class Window : public Gtk::Window {
public:
    Window();
    virtual ~Window(){};

private:
    // Model
    std::unique_ptr<Slicer::Document> m_document;

    // Header bar
    Gtk::HeaderBar m_headerBar;
    Gtk::Button m_buttonOpen;
    Gtk::Button m_buttonSave;
    Gtk::Box m_boxRemovePages;
    Gtk::Button m_buttonRemovePages;
    Gtk::MenuButton m_buttonRemoveOptions;
    Gtk::Box m_boxMenuRemoveOptions;
    Gtk::ModelButton m_buttonRemovePrevious;
    Gtk::ModelButton m_buttonRemoveNext;
    Gtk::Button m_buttonPreviewPage;

    // View
    Slicer::View* m_view;
    Gtk::ScrolledWindow m_scroller;
    Gtk::Overlay m_overlay;

    Gtk::Revealer m_revealerDone;
    Gtk::Box m_boxDone;
    Gtk::Label m_labelDone;
    Gtk::Button m_buttonCloseDone;

    // Preview window
    std::unique_ptr<Slicer::PreviewWindow> m_previewWindow;

    // Signals
    sigc::signal<void> m_signalSaved;
    sigc::connection m_connectionSaved;

    // Functions
    void removeSelectedPages();
    void removePreviousPages();
    void removeNextPages();
    void previewPage(int pageNumber);

    // Callbacks
    void onSaveAction();
    void onOpenAction();
};
}

#endif // SLICERWINDOW_HPP
