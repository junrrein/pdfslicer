#ifndef SLICERWINDOW_HPP
#define SLICERWINDOW_HPP

#include "view.hpp"
#include "previewwindow.hpp"
#include "zoomlevel.hpp"
#include "welcomescreen.hpp"
#include "headerbar.hpp"
#include <gtkmm/applicationwindow.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/button.h>
#include <gtkmm/overlay.h>
#include <gtkmm/revealer.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>

namespace Slicer {

class AppWindow : public Gtk::ApplicationWindow {
public:
    AppWindow();
    virtual ~AppWindow() = default;

    void openDocument(const Glib::RefPtr<Gio::File>& file);

private:
    // Model
    std::unique_ptr<Slicer::Document> m_document;

    HeaderBar m_headerBar;
    WelcomeScreen m_welcomeScreen;

    // View
    std::unique_ptr<Slicer::View> m_view;
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

    // Actions
    Glib::RefPtr<Gio::SimpleAction> m_openAction;
    Glib::RefPtr<Gio::SimpleAction> m_saveAction;
    Glib::RefPtr<Gio::SimpleAction> m_undoAction;
    Glib::RefPtr<Gio::SimpleAction> m_redoAction;

    // Functions
    void addActions();
    void setupWidgets();
    void setupSignalHandlers();
    void loadCustomCSS();
    void buildView();

    // Callbacks
    void onSaveAction();
    void onOpenAction();
    void onUndoAction();
    void onRedoAction();
    void onCommandExecuted();
};
}

#endif // SLICERWINDOW_HPP
