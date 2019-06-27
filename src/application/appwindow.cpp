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

#include "appwindow.hpp"
#include "aboutdialog.hpp"
#include "openfiledialog.hpp"
#include "savefiledialog.hpp"
#include "guicommand.hpp"
#include <pdfsaver.hpp>
#include <glibmm/convert.h>
#include <glibmm/main.h>
#include <glibmm/i18n.h>
#include <gtkmm/builder.h>
#include <gtkmm/cssprovider.h>
#include <gtkmm/messagedialog.h>
#include <gtkmm/shortcutswindow.h>
#include <config.hpp>
#include <logger.hpp>

namespace Slicer {

const std::vector<int> AppWindow::zoomLevels = {200, 300, 400};

AppWindow::AppWindow(BackgroundThread& backgroundThread, SettingsManager& settingsManager)
    : m_backgroundThread{backgroundThread}
    , m_settingsManager{settingsManager}
    , m_windowState{}
    , m_zoomLevel{zoomLevels, *this}
    , m_headerBar{m_zoomLevel.zoomLevelIndex()}
    , m_view{m_backgroundThread}
{
    set_size_request(500, 500);

    loadWindowState();
    loadWidgets();
    addActions();
    setupWidgets();
    setupSignalHandlers();
    loadCustomCSS();

    show_all_children();
}

AppWindow::~AppWindow()
{
    saveWindowState();
}

void AppWindow::setDocument(std::unique_ptr<Document> document)
{
    m_view.setDocument(*document, m_zoomLevel.currentLevel());
    m_view.setShowFileNames(false);
    m_document = std::move(document);

    m_stack.set_visible_child("editor");

    m_commandManager.reset();
    m_headerBar.enableAddDocumentButton();
    m_headerBar.enableZoomSlider();
    m_saveAction->set_enabled();
    m_zoomLevel.enable();
}

bool AppWindow::on_delete_event(GdkEventAny*)
{
    // TODO: Right now, when we are saving a document and the user
    // tries to close the window, the app does nothing. It doesn't
    // react at all.
    // Maybe we should tell the user visually that his request to
    // close the window was purposefully ignored.
    return m_isSavingDocument;
}

void AppWindow::loadWindowState()
{
    m_windowState = m_settingsManager.loadWindowState();

    set_default_size(m_windowState.width, m_windowState.height);

    if (m_windowState.isMaximized)
        maximize();
}

void AppWindow::saveWindowState()
{
    m_settingsManager.saveWindowState(m_windowState);
}

void AppWindow::loadWidgets()
{
    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_resource("/pdfslicer/ui/shortcuts.ui");
    Gtk::ShortcutsWindow* shortcutsWindow;
    builder->get_widget("shortcuts-pdfslicer", shortcutsWindow);

    shortcutsWindow->set_transient_for(*this);

    m_shortcutsWindow.reset(shortcutsWindow);
}

void AppWindow::addActions()
{
    m_openAction = add_action("open-document", sigc::mem_fun(*this, &AppWindow::onOpenAction));
    m_addDocumentAtBeginningAction = add_action("add-document-at-beginning", sigc::mem_fun(*this, &AppWindow::onAddDocumentAtBeginningAction));
    m_addDocumentAtEndAction = add_action("add-document-at-end", sigc::mem_fun(*this, &AppWindow::onAddDocumentAtEndAction));
    m_addDocumentAfterSelectedAction = add_action("add-document-after-selected", sigc::mem_fun(*this, &AppWindow::onAddDocumentAfterSelectedAction));
    m_saveAction = add_action("save-document", sigc::mem_fun(*this, &AppWindow::onSaveAction));
    m_undoAction = add_action("undo", sigc::mem_fun(*this, &AppWindow::onUndoAction));
    m_redoAction = add_action("redo", sigc::mem_fun(*this, &AppWindow::onRedoAction));
    m_removeSelectedAction = add_action("remove-selected", sigc::mem_fun(*this, &AppWindow::onRemoveSelectedPages));
    m_removeUnselectedAction = add_action("remove-unselected", sigc::mem_fun(*this, &AppWindow::onRemoveUnselectedPages));
    m_removePreviousAction = add_action("remove-previous", sigc::mem_fun(*this, &AppWindow::onRemovePreviousPages));
    m_removeNextAction = add_action("remove-next", sigc::mem_fun(*this, &AppWindow::onRemoveNextPages));
    m_rotateRightAction = add_action("rotate-right", sigc::mem_fun(*this, &AppWindow::onRotatePagesRight));
    m_rotateLeftAction = add_action("rotate-left", sigc::mem_fun(*this, &AppWindow::onRotatePagesLeft));
    m_moveLeftAction = add_action("move-left", sigc::mem_fun(*this, &AppWindow::onMovePagesLeft));
    m_moveRightAction = add_action("move-right", sigc::mem_fun(*this, &AppWindow::onMovePagesRight));
    m_selectAllAction = add_action("select-all", sigc::mem_fun(*this, &AppWindow::onSelectAll));
    m_selectOddPagesAction = add_action("select-odd", sigc::mem_fun(*this, &AppWindow::onSelectOddPages));
    m_selectEvenPagesAction = add_action("select-even", sigc::mem_fun(*this, &AppWindow::onSelectEvenPages));
    m_cancelSelectionAction = add_action("cancel-selection", sigc::mem_fun(*this, &AppWindow::onCancelSelection));
    m_shortcutsAction = add_action("shortcuts", sigc::mem_fun(*this, &AppWindow::onShortcutsAction));
    m_aboutAction = add_action("about", sigc::mem_fun(*this, &AppWindow::onAboutAction));

    m_headerBar.disableAddDocumentButton();
    m_addDocumentAfterSelectedAction->set_enabled(false);
    m_headerBar.disableZoomSlider();
    m_saveAction->set_enabled(false);
    m_undoAction->set_enabled(false);
    m_redoAction->set_enabled(false);
    m_removeSelectedAction->set_enabled(false);
    m_removeUnselectedAction->set_enabled(false);
    m_removePreviousAction->set_enabled(false);
    m_removeNextAction->set_enabled(false);
    m_actionBar.disableButtonRemovePagesMore();
    m_rotateRightAction->set_enabled(false);
    m_rotateLeftAction->set_enabled(false);
    m_moveLeftAction->set_enabled(false);
    m_moveRightAction->set_enabled(false);
    m_selectAllAction->set_enabled(false);
    m_cancelSelectionAction->set_enabled(false);
}

void AppWindow::setupWidgets()
{
    set_titlebar(m_headerBar);

    m_scroller.add(m_view);
    auto editorBox = Gtk::manage(new Gtk::Box{Gtk::ORIENTATION_VERTICAL}); // NOLINT
    editorBox->pack_start(m_scroller);
    editorBox->pack_start(m_actionBar, Gtk::PACK_SHRINK);

    m_stack.add(m_welcomeScreen, "welcome");
    m_stack.add(*editorBox, "editor");

    m_overlay.add(m_stack);
    m_overlay.add_overlay(m_savingRevealer);

    add(m_overlay); // NOLINT
    show_all_children();
}

void AppWindow::setupSignalHandlers()
{
    m_view.selectedPagesChanged.connect([this]() {
        onSelectedPagesChanged();
    });

    m_zoomLevel.zoomLevelIndex().signal_changed().connect([this]() {
        onZoomLevelChanged();
    });

    m_savedDispatcher.connect([this]() {
        m_savingRevealer.saved();
        enableEditingActions();
        setTitleModified(false);
    });

    m_savingFailedDispatcher.connect([this]() {
        m_savingRevealer.set_reveal_child(false);
        enableEditingActions();

        Gtk::MessageDialog errorDialog{_("The current document could not be saved"),
                                       false,
                                       Gtk::MESSAGE_ERROR,
                                       Gtk::BUTTONS_CLOSE,
                                       true};
        errorDialog.set_transient_for(*this);

        errorDialog.run();
    });

    m_scroller.get_vadjustment()->signal_value_changed().connect([this]() {
        onScrollPositionChanged();
    });

    signal_configure_event().connect(sigc::mem_fun(*this, &AppWindow::onWindowConfigureEvent),
                                     false); // Run before default handler
    signal_window_state_event().connect(sigc::mem_fun(*this, &AppWindow::onWindowStateEvent));
    m_commandManager.commandExecuted.connect(sigc::mem_fun(*this, &AppWindow::onCommandExecuted));
}

void AppWindow::loadCustomCSS()
{
    auto screen = Gdk::Screen::get_default();
    auto provider = Gtk::CssProvider::create();
    provider->load_from_data(R"(
        overlay > revealer > box {
            border-radius: 0px 0px 11px 11px;
        }

        .welcome-label {
            font-size: 19px;
            font-weight: bold;
        }

        .thin-button {
            padding-left: 4px;
            padding-right: 4px;
        }
    )");
    Gtk::StyleContext::add_provider_for_screen(screen,
                                               provider,
                                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

void AppWindow::disableEditingActions()
{
    m_openAction->set_enabled(false);
    m_saveAction->set_enabled(false);
    m_undoAction->set_enabled(false);
    m_redoAction->set_enabled(false);
    m_removeSelectedAction->set_enabled(false);
    m_removeUnselectedAction->set_enabled(false);
    m_removePreviousAction->set_enabled(false);
    m_removeNextAction->set_enabled(false);
    m_rotateRightAction->set_enabled(false);
    m_rotateLeftAction->set_enabled(false);
    m_moveLeftAction->set_enabled(false);
    m_moveRightAction->set_enabled(false);
    m_cancelSelectionAction->set_enabled(false);
}

void AppWindow::enableEditingActions()
{
    m_openAction->set_enabled();
    m_saveAction->set_enabled();
    onSelectedPagesChanged();
    onCommandExecuted();
}

void AppWindow::onAboutAction()
{
    (new Slicer::AboutDialog{*this})->present();
}

void AppWindow::onShortcutsAction()
{
    m_shortcutsWindow->present();
    m_shortcutsWindow->show_all_children();
}

void AppWindow::onSaveAction()
{
    Slicer::SaveFileDialog dialog{*this, m_document->originalDocumentParentPath()};

    const int result = dialog.run();

    if (result == GTK_RESPONSE_ACCEPT)
        trySaveDocument(dialog.get_file());
}

void AppWindow::trySaveDocument(const Glib::RefPtr<Gio::File>& file)
{
    m_savingRevealer.saving();
    disableEditingActions();
    m_isSavingDocument = true;

    std::thread thread{[this, file]() {
        try {
            PdfSaver{*m_document}.save(file);
            m_savedDispatcher.emit();
        }
        catch (...) {
            Logger::logError("Saving the document failed");
            Logger::logError("The destination file was: " + file->get_path());
            m_savingFailedDispatcher.emit();
        }

        m_isSavingDocument = false;
    }};

    thread.detach();
}

void AppWindow::onOpenAction()
{
    Slicer::OpenFileDialog dialog{*this};

    const int result = dialog.run();

    if (result == GTK_RESPONSE_ACCEPT)
        tryOpenDocument(dialog.get_file());
}

void AppWindow::showOpenFileFailedErrorDialog(const std::string& filePath)
{
    Logger::logError("The file couldn't be opened");
    Logger::logError("Filepath: " + filePath);

    Gtk::MessageDialog errorDialog{_("The selected file could not be opened"),
                                   false,
                                   Gtk::MESSAGE_ERROR,
                                   Gtk::BUTTONS_CLOSE,
                                   true};
    errorDialog.set_transient_for(*this);
    errorDialog.run();
}

void AppWindow::setTitleModified(bool modified)
{
    if (modified && m_headerBar.get_title().at(0) != '*')
        m_headerBar.set_title("*" + m_headerBar.get_title());

    if (!modified && m_headerBar.get_title().at(0) == '*')
        m_headerBar.set_title(m_headerBar.get_title().substr(1));
}

void AppWindow::tryAddDocumentAt(const Glib::RefPtr<Gio::File>& file, unsigned int position)
{
    try {
        auto command = std::make_shared<GuiAddFileCommand>(*m_document,
                                                           file,
                                                           position,
                                                           m_headerBar,
                                                           m_view);
        m_commandManager.execute(command);
    }
    catch (...) {
        showOpenFileFailedErrorDialog(file->get_path());
    }
}

void AppWindow::onAddDocumentAtBeginningAction()
{
    Slicer::OpenFileDialog dialog{*this, m_document->lastAddedFileParentPath()};

    const int result = dialog.run();

    if (result == Gtk::RESPONSE_ACCEPT)
        tryAddDocumentAt(dialog.get_file(), 0);
}

void AppWindow::onAddDocumentAtEndAction()
{
    Slicer::OpenFileDialog dialog{*this, m_document->lastAddedFileParentPath()};

    const int result = dialog.run();

    if (result == Gtk::RESPONSE_ACCEPT)
        tryAddDocumentAt(dialog.get_file(), m_document->numberOfPages());
}

void AppWindow::onAddDocumentAfterSelectedAction()
{
    Slicer::OpenFileDialog dialog{*this, m_document->lastAddedFileParentPath()};

    const int result = dialog.run();

    if (result == Gtk::RESPONSE_ACCEPT)
        tryAddDocumentAt(dialog.get_file(), m_view.getSelectedChildIndex() + 1);
}

void AppWindow::tryOpenDocument(const Glib::RefPtr<Gio::File>& file)
{
    try {
        auto document = std::make_unique<Document>(file);
        setDocument(std::move(document));
        m_headerBar.set_title(Glib::filename_display_basename(file->get_path()));
        m_headerBar.set_subtitle("");
    }
    catch (...) {
        showOpenFileFailedErrorDialog(file->get_path());
    }
}

void AppWindow::onUndoAction()
{
    m_commandManager.undo();
}

void AppWindow::onRedoAction()
{

    m_commandManager.redo();
}

void AppWindow::onRemoveSelectedPages()
{
    auto command = std::make_shared<RemovePagesCommand>(*m_document, m_view.getSelectedChildrenIndexes());
    m_commandManager.execute(command);
}

void AppWindow::onRemoveUnselectedPages()
{
    auto command = std::make_shared<RemovePagesCommand>(*m_document, m_view.getUnselectedChildrenIndexes());
    m_commandManager.execute(command);
}

void AppWindow::onRemovePreviousPages()
{
    auto command = std::make_shared<RemovePageRangeCommand>(*m_document, 0, m_view.getSelectedChildIndex() - 1);
    m_commandManager.execute(command);
}

void AppWindow::onRemoveNextPages()
{
    auto command = std::make_shared<RemovePageRangeCommand>(*m_document,
                                                            m_view.getSelectedChildIndex() + 1,
                                                            m_document->numberOfPages() - 1);
    m_commandManager.execute(command);
}

void AppWindow::onRotatePagesRight()
{
    auto command = std::make_shared<RotatePagesRightCommand>(*m_document, m_view.getSelectedChildrenIndexes());
    m_commandManager.execute(command);
}

void AppWindow::onRotatePagesLeft()
{
    auto command = std::make_shared<RotatePagesLeftCommand>(*m_document, m_view.getSelectedChildrenIndexes());
    m_commandManager.execute(command);
}

void AppWindow::onMovePagesLeft()
{
    const std::vector<unsigned int> indexToMove = m_view.getSelectedChildrenIndexes();

    std::shared_ptr<Command> command;

    if (indexToMove.size() == 1)
        command = std::make_shared<MovePageCommand>(*m_document,
                                                    indexToMove.front(),
                                                    indexToMove.front() - 1);
    else
        command = std::make_shared<MovePageRangeCommand>(*m_document,
                                                         indexToMove.front(),
                                                         indexToMove.back(),
                                                         indexToMove.front() - 1);

    m_commandManager.execute(command);
}

void AppWindow::onMovePagesRight()
{
    const std::vector<unsigned int> indexToMove = m_view.getSelectedChildrenIndexes();

    std::shared_ptr<Command> command;

    if (indexToMove.size() == 1)
        command = std::make_shared<MovePageCommand>(*m_document,
                                                    indexToMove.front(),
                                                    indexToMove.front() + 1);
    else
        command = std::make_shared<MovePageRangeCommand>(*m_document,
                                                         indexToMove.front(),
                                                         indexToMove.back(),
                                                         indexToMove.front() + 1);

    m_commandManager.execute(command);
}

void AppWindow::onSelectAll()
{
    m_view.selectAllPages();
}

void AppWindow::onSelectOddPages()
{
    m_view.selectOddPages();
}

void AppWindow::onSelectEvenPages()
{
    m_view.selectEvenPages();
}

void AppWindow::onCancelSelection()
{
    m_view.clearSelection();
}

bool isVectorContigous(const std::vector<unsigned int>& vector)
{
    for (unsigned int i = 0; i < vector.size(); ++i)
        if (vector.front() + i != vector.at(i))
            return false;

    return true;
}

void AppWindow::onSelectedPagesChanged()
{
    const std::vector<unsigned int> indexSelected = m_view.getSelectedChildrenIndexes();
    const unsigned long numSelected = indexSelected.size();
    const unsigned long numPages = m_view.get_children().size();

    if (numSelected == 0) {
        m_removeSelectedAction->set_enabled(false);
        m_removeUnselectedAction->set_enabled(false);
        m_removePreviousAction->set_enabled(false);
        m_removeNextAction->set_enabled(false);
        m_actionBar.disableButtonRemovePagesMore();
        m_rotateRightAction->set_enabled(false);
        m_rotateLeftAction->set_enabled(false);
        m_moveLeftAction->set_enabled(false);
        m_moveRightAction->set_enabled(false);
        m_cancelSelectionAction->set_enabled(false);
    }
    else {
        m_removeSelectedAction->set_enabled();
        m_actionBar.enableButtonRemovePagesMore();
        m_rotateRightAction->set_enabled();
        m_rotateLeftAction->set_enabled();
        m_cancelSelectionAction->set_enabled();

        if (numSelected == numPages)
            m_removeUnselectedAction->set_enabled(false);
        else
            m_removeUnselectedAction->set_enabled();

        if (indexSelected.front() == 0)
            m_moveLeftAction->set_enabled(false);
        else
            m_moveLeftAction->set_enabled();

        if (indexSelected.back() == numPages - 1)
            m_moveRightAction->set_enabled(false);
        else
            m_moveRightAction->set_enabled();
    }

    if (numSelected == 1) {
        if (indexSelected.front() == 0)
            m_removePreviousAction->set_enabled(false);
        else
            m_removePreviousAction->set_enabled();

        if (indexSelected.front() == numPages - 1)
            m_removeNextAction->set_enabled(false);
        else
            m_removeNextAction->set_enabled();

        m_addDocumentAfterSelectedAction->set_enabled();
    }
    else {
        m_addDocumentAfterSelectedAction->set_enabled(false);
    }

    if (numSelected > 1) {
        m_removePreviousAction->set_enabled(false);
        m_removeNextAction->set_enabled(false);

        if (!isVectorContigous(indexSelected)) {
            m_moveLeftAction->set_enabled(false);
            m_moveRightAction->set_enabled(false);
        }
    }

    if (numPages == 0 || numPages == numSelected)
        m_selectAllAction->set_enabled(false);
    else
        m_selectAllAction->set_enabled();
}

void AppWindow::onCommandExecuted()
{
    if (m_commandManager.canUndo()) {
        m_undoAction->set_enabled();
        setTitleModified(true);
    }
    else {
        m_undoAction->set_enabled(false);
        setTitleModified(false);
    }

    if (m_commandManager.canRedo())
        m_redoAction->set_enabled();
    else
        m_redoAction->set_enabled(false);
}

void AppWindow::onZoomLevelChanged()
{
    m_view.changePageSize(m_zoomLevel.currentLevel());

    queueRestoreScrollPosition();
}

void AppWindow::saveScrollPosition()
{
    m_scrollPosition = m_scroller.get_vadjustment()->get_value()
                       / m_scroller.get_vadjustment()->get_upper();
}

void AppWindow::onScrollPositionChanged()
{
    saveScrollPosition();
}

void AppWindow::restoreScrollPosition()
{
    m_scroller.get_vadjustment()->set_value(m_scrollPosition
                                            * m_scroller.get_vadjustment()->get_upper());
}

void AppWindow::queueRestoreScrollPosition()
{
    m_onScrollLimitChangedConnection = m_scroller.get_vadjustment()->signal_changed().connect([this]() {
        onScrollLimitChanged();
    });
}

void AppWindow::onScrollLimitChanged()
{
    restoreScrollPosition();

    m_onScrollLimitChangedConnection.disconnect();
}

bool AppWindow::onWindowConfigureEvent(GdkEventConfigure*)
{
    if (!is_maximized())
        get_size(m_windowState.width, m_windowState.height);

    if (m_document != nullptr)
        queueRestoreScrollPosition();

    return false;
}

bool AppWindow::onWindowStateEvent(GdkEventWindowState* state)
{
    m_windowState.isMaximized = (state->new_window_state & GDK_WINDOW_STATE_MAXIMIZED) != 0;

    return false;
}
}
