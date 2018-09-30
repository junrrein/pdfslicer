// PDF Slicer
// Copyright (C) 2017-2018 Julián Unrrein

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef SLICERWINDOW_HPP
#define SLICERWINDOW_HPP

#include "../application/backgroundthread.hpp"
#include "actionbar.hpp"
#include "headerbar.hpp"
#include "savingrevealer.hpp"
#include "view.hpp"
#include "welcomescreen.hpp"
#include "zoomlevelwithactions.hpp"
#include <gtkmm/applicationwindow.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/label.h>
#include <gtkmm/overlay.h>
#include <gtkmm/revealer.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/stack.h>
#include <giomm/settings.h>

namespace Slicer {

class AppWindow : public Gtk::ApplicationWindow {
public:
    AppWindow(BackgroundThread& backgroundThread);
    virtual ~AppWindow() override;

    void openDocument(const Glib::RefPtr<Gio::File>& file);

protected:
    virtual bool on_delete_event(GdkEventAny*) override;

private:
    std::unique_ptr<Document> m_document;
    std::atomic<bool> m_isSavingDocument{false};
    BackgroundThread& m_backgroundThread;

    Glib::RefPtr<Gio::Settings> m_settings;
    struct {
        int width;
        int height;
        bool isMaximized;
    } m_windowState;

    HeaderBar m_headerBar;
    Gtk::Overlay m_overlay;
    Gtk::Stack m_stack;
    WelcomeScreen m_welcomeScreen;
    Gtk::ScrolledWindow m_scroller;
    View m_view;
    ActionBar m_actionBar;

    ZoomLevelWithActions m_zoomLevel;
    static const std::set<int> zoomLevels;

    SavingRevealer m_savingRevealer;
    Glib::Dispatcher m_savedDispatcher;
    Glib::Dispatcher m_savingFailedDispatcher;

    std::unique_ptr<Gtk::ShortcutsWindow> m_shortcutsWindow;

    // Actions
    Glib::RefPtr<Gio::SimpleAction> m_openAction;
    Glib::RefPtr<Gio::SimpleAction> m_saveAction;
    Glib::RefPtr<Gio::SimpleAction> m_undoAction;
    Glib::RefPtr<Gio::SimpleAction> m_redoAction;
    Glib::RefPtr<Gio::SimpleAction> m_removeSelectedAction;
    Glib::RefPtr<Gio::SimpleAction> m_removeUnselectedAction;
    Glib::RefPtr<Gio::SimpleAction> m_removePreviousAction;
    Glib::RefPtr<Gio::SimpleAction> m_removeNextAction;
    Glib::RefPtr<Gio::SimpleAction> m_rotateRightAction;
    Glib::RefPtr<Gio::SimpleAction> m_rotateLeftAction;
    Glib::RefPtr<Gio::SimpleAction> m_cancelSelectionAction;
    Glib::RefPtr<Gio::SimpleAction> m_shortcutsAction;
    Glib::RefPtr<Gio::SimpleAction> m_aboutAction;

    // Functions
    void loadWindowState();
    void saveWindowState();
    void loadWidgets();
    void addActions();
    void setupWidgets();
    void setupSignalHandlers();
    void loadCustomCSS();
    void disableEditingActions();
    void enableEditingActions();

    // Callbacks
    void onSaveAction();
    void onOpenAction();
    void onUndoAction();
    void onRedoAction();
    void onRemoveSelectedPages();
    void onRemoveUnselectedPages();
    void onRemovePreviousPages();
    void onRemoveNextPages();
    void onRotatePagesRight();
    void onRotatePagesLeft();
    void onCancelSelection();
    void onAboutAction();
    void onShortcutsAction();
    void onSelectedPagesChanged();
    void onCommandExecuted();
    void onSizeAllocate(Gtk::Allocation&);
    bool onWindowStateEvent(GdkEventWindowState* state);
};
}

#endif // SLICERWINDOW_HPP
