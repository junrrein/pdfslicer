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
    virtual ~Window();

private:
    enum class ZoomLevel {
        small = 200,
        medium = 300,
        large = 400
    };

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
    Gtk::Button m_buttonZoomOut;
    Gtk::Button m_buttonZoomIn;
    Gtk::Box m_boxZoom;
    ZoomLevel m_zoomLevel;

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
    sigc::signal<void> m_signalZoomChanged;

    // Functions
    void removeSelectedPage();
    void removePreviousPages();
    void removeNextPages();
    void previewPage(int pageNumber);
    void increaseZoomLevel();
    void decreaseZoomLevel();
    void buildView();

    // Callbacks
    void onSaveAction();
    void onOpenAction();
};
}

#endif // SLICERWINDOW_HPP
