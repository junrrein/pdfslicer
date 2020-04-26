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

#include "actionbar.hpp"
#include "headerbar.hpp"
#include "savingrevealer.hpp"
#include "settingsmanager.hpp"
#include "taskrunner.hpp"
#include "view.hpp"
#include "welcomescreen.hpp"
#include "zoomlevelwithactions.hpp"
#include <commandmanager.hpp>
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
    AppWindow(TaskRunner& taskRunner,
              SettingsManager& settingsManager);

    AppWindow(const AppWindow&) = delete;
    AppWindow& operator=(const AppWindow&) = delete;
    AppWindow(AppWindow&&) = delete;
    AppWindow& operator=(AppWindow&& src) = delete;

    ~AppWindow() override;

    void setDocument(std::unique_ptr<Document> document);

protected:
    bool on_delete_event(GdkEventAny*) override;

private:
    enum class SaveFileIn {
        Foreground,
        Background
    };

    static void loadCustomCSS();

    std::unique_ptr<Document> m_document;
    bool m_isDocumentModified = false;
    std::atomic<bool> m_isSavingDocument{false};
    TaskRunner& m_taskRunner;

    SettingsManager& m_settingsManager;
    WindowState m_windowState;
    CommandManager m_commandManager;

    ZoomLevelWithActions m_zoomLevel;
    static const std::vector<int> zoomLevels;

    HeaderBar m_headerBar;
    Gtk::Overlay m_overlay;
    Gtk::Stack m_stack;
    WelcomeScreen m_welcomeScreen;
    Gtk::ScrolledWindow m_scroller;
    double m_scrollPosition = 0;
    View m_view;
    ActionBar m_actionBar;

    SavingRevealer m_savingRevealer;
    Glib::Dispatcher m_savedDispatcher;
    Glib::Dispatcher m_savingFailedDispatcher;

    std::unique_ptr<Gtk::ShortcutsWindow> m_shortcutsWindow;

    // Actions
    Glib::RefPtr<Gio::SimpleAction> m_openAction;
    Glib::RefPtr<Gio::SimpleAction> m_addDocumentAtBeginningAction;
    Glib::RefPtr<Gio::SimpleAction> m_addDocumentAtEndAction;
    Glib::RefPtr<Gio::SimpleAction> m_addDocumentAfterSelectedAction;
    Glib::RefPtr<Gio::SimpleAction> m_saveAction;
    Glib::RefPtr<Gio::SimpleAction> m_undoAction;
    Glib::RefPtr<Gio::SimpleAction> m_redoAction;
    Glib::RefPtr<Gio::SimpleAction> m_removeSelectedAction;
    Glib::RefPtr<Gio::SimpleAction> m_removeUnselectedAction;
    Glib::RefPtr<Gio::SimpleAction> m_removePreviousAction;
    Glib::RefPtr<Gio::SimpleAction> m_removeNextAction;
    Glib::RefPtr<Gio::SimpleAction> m_rotateRightAction;
    Glib::RefPtr<Gio::SimpleAction> m_rotateLeftAction;
    Glib::RefPtr<Gio::SimpleAction> m_moveLeftAction;
    Glib::RefPtr<Gio::SimpleAction> m_moveRightAction;
    Glib::RefPtr<Gio::SimpleAction> m_selectAllAction;
    Glib::RefPtr<Gio::SimpleAction> m_selectOddPagesAction;
    Glib::RefPtr<Gio::SimpleAction> m_selectEvenPagesAction;
    Glib::RefPtr<Gio::SimpleAction> m_invertSelectionAction;
    Glib::RefPtr<Gio::SimpleAction> m_cancelSelectionAction;
    Glib::RefPtr<Gio::SimpleAction> m_shortcutsAction;
    Glib::RefPtr<Gio::SimpleAction> m_aboutAction;
    Glib::RefPtr<Gio::SimpleAction> m_closeWindowAction;

    // Functions
    void loadPreviousSessionState();
    void saveCurrentSessionState();
    void loadWidgets();
    void addActions();
    void setupWidgets();
    void setupSignalHandlers();
    void disableEditingActions();
    void enableEditingActions();
    bool showSaveFileDialogAndSave(SaveFileIn howToSave);
    bool saveFileInForeground(const Glib::RefPtr<Gio::File>& file);
    void saveFileInBackground(const Glib::RefPtr<Gio::File>& file);
    void tryOpenDocument(const Glib::RefPtr<Gio::File>& file);
    void tryAddDocumentsAt(const std::vector<Glib::RefPtr<Gio::File>>& files,
                           unsigned int position);
    void showOpenFileFailedErrorDialog();
    void showSaveFileFailedErrorDialog();
    void setModified(bool modified);
    void saveScrollPosition();
    void restoreScrollPosition();
    void queueRestoreScrollPosition();

    // Callbacks
    void onOpenAction();
    void onAddDocumentAtBeginningAction();
    void onAddDocumentAtEndAction();
    void onAddDocumentAfterSelectedAction();
    void onSaveAction();
    void onUndoAction();
    void onRedoAction();
    void onRemoveSelectedPages();
    void onRemoveUnselectedPages();
    void onRemovePreviousPages();
    void onRemoveNextPages();
    void onRotatePagesRight();
    void onRotatePagesLeft();
    void onMovePagesLeft();
    void onMovePagesRight();
    void onSelectAll();
    void onSelectOddPages();
    void onSelectEvenPages();
    void onInvertSelection();
    void onCancelSelection();
    void onAboutAction();
    void onCloseWindowAction();
    void onShortcutsAction();
    void onSelectedPagesChanged();
    void onCommandExecuted();
    void onZoomLevelChanged();
    void onScrollPositionChanged();
    void onScrollLimitChanged();
    void onViewMouseWheelUp();
    void onViewMouseWheelDown();
    sigc::connection m_onScrollLimitChangedConnection;
    bool onWindowConfigureEvent(GdkEventConfigure*);
    bool onWindowStateEvent(GdkEventWindowState* state);
};
}

#endif // SLICERWINDOW_HPP
