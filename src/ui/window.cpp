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

#include "window.hpp"
#include "openfiledialog.hpp"
#include "savefiledialog.hpp"
#include <glibmm/main.h>
#include <glibmm/i18n.h>
#include <gtkmm/cssprovider.h>
#include <gtkmm/messagedialog.h>

namespace Slicer {

const std::set<int> AppWindow::zoomLevels = {200, 300, 400};

AppWindow::AppWindow(BackgroundThread& backgroundThread)
    : m_backgroundThread{backgroundThread}
    , m_renderer{m_view, m_backgroundThread}
    , m_zoomLevel{zoomLevels, *this}
{
    set_size_request(500, 500);
    set_default_size(800, 600);

    addActions();
    setupWidgets();
    setupSignalHandlers();
    loadCustomCSS();

    show_all_children();
}

void AppWindow::openDocument(const Glib::RefPtr<Gio::File>& file)
{
    auto document = std::make_unique<Document>(file->get_path());
    m_renderer.setDocument(*document, m_zoomLevel.currentLevel());
    m_document = std::move(document);

    m_stack.set_visible_child("editor");

    m_headerBar.set_subtitle(file->get_basename());

    m_saveAction->set_enabled();
    m_zoomLevel.enable();

    m_document->commandExecuted().connect(sigc::mem_fun(*this, &AppWindow::onCommandExecuted));
}

void AppWindow::addActions()
{
    m_openAction = add_action("open-document", sigc::mem_fun(*this, &AppWindow::onOpenAction));
    m_saveAction = add_action("save-document", sigc::mem_fun(*this, &AppWindow::onSaveAction));
    m_undoAction = add_action("undo", sigc::mem_fun(*this, &AppWindow::onUndoAction));
    m_redoAction = add_action("redo", sigc::mem_fun(*this, &AppWindow::onRedoAction));
    m_removeSelectedAction = add_action("remove-selected", sigc::mem_fun(*this, &AppWindow::onRemoveSelectedPages));
    m_removeUnselectedAction = add_action("remove-unselected", sigc::mem_fun(*this, &AppWindow::onRemoveUnselectedPages));
    m_removePreviousAction = add_action("remove-previous", sigc::mem_fun(*this, &AppWindow::onRemovePreviousPages));
    m_removeNextAction = add_action("remove-next", sigc::mem_fun(*this, &AppWindow::onRemoveNextPages));
    m_rotateRightAction = add_action("rotate-right", sigc::mem_fun(*this, &AppWindow::onRotatePagesRight));
    m_rotateLeftAction = add_action("rotate-left", sigc::mem_fun(*this, &AppWindow::onRotatePagesLeft));
    m_previewPageAction = add_action("preview-selected", sigc::mem_fun(*this, &AppWindow::onPreviewPage));
    m_cancelSelectionAction = add_action("cancel-selection", sigc::mem_fun(*this, &AppWindow::onCancelSelection));

    m_saveAction->set_enabled(false);
    m_undoAction->set_enabled(false);
    m_redoAction->set_enabled(false);
    m_removeSelectedAction->set_enabled(false);
    m_removeUnselectedAction->set_enabled(false);
    m_removePreviousAction->set_enabled(false);
    m_removeNextAction->set_enabled(false);
    m_rotateRightAction->set_enabled(false);
    m_rotateLeftAction->set_enabled(false);
    m_previewPageAction->set_enabled(false);
    m_cancelSelectionAction->set_enabled(false);
}

void AppWindow::setupWidgets()
{
    set_titlebar(m_headerBar);

    m_scroller.add(m_view);
    auto editorBox = Gtk::manage(new Gtk::Box{Gtk::ORIENTATION_VERTICAL});
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
    m_view.signal_selected_children_changed().connect([this]() {
        onSelectedPagesChanged();
    });

    m_view.signal_child_activated().connect([this](Gtk::FlowBoxChild*) {
        m_previewPageAction->activate();
    });

    m_zoomLevel.changed.connect([this](int targetSize) {
        m_renderer.setDocument(*m_document, targetSize);
    });

    m_savedDispatcher.connect([this]() {
        m_savingRevealer.saved();
        m_openAction->set_enabled();
        m_saveAction->set_enabled();
    });

    m_savingFailedDispatcher.connect([this]() {
        m_savingRevealer.set_reveal_child(false);
        m_openAction->set_enabled();
        m_saveAction->set_enabled();

        Gtk::MessageDialog errorDialog{_("The current document could not be saved"),
                                       false,
                                       Gtk::MESSAGE_ERROR,
                                       Gtk::BUTTONS_CLOSE,
                                       true};
        errorDialog.set_transient_for(*this);

        errorDialog.run();
    });
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

        .pepino {
            padding: 3px;
        }
    )");
    Gtk::StyleContext::add_provider_for_screen(screen,
                                               provider,
                                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

// FIXME:: We need to make this not freeze the UI too
void AppWindow::onSaveAction()
{
    Slicer::SaveFileDialog dialog{*this};

    const int result = dialog.run();

    if (result == GTK_RESPONSE_ACCEPT) {
        Glib::RefPtr<Gio::File> file = dialog.get_file();
        m_savingRevealer.saving();
        m_openAction->set_enabled(false);
        m_saveAction->set_enabled(false);

        std::thread thread{[this, file]() {
            try {
                m_document->saveDocument(file);
                m_savedDispatcher.emit();
            }
            catch (...) {
                m_savingFailedDispatcher.emit();
            }
        }};

        thread.detach();
    }
}

void AppWindow::onOpenAction()
{
    Slicer::OpenFileDialog dialog{*this};

    const int result = dialog.run();

    if (result == GTK_RESPONSE_ACCEPT)
        try {
            openDocument(dialog.get_file());
        }
        catch (...) {
            Gtk::MessageDialog errorDialog{_("The selected file could not be opened"),
                                           false,
                                           Gtk::MESSAGE_ERROR,
                                           Gtk::BUTTONS_CLOSE,
                                           true};
            errorDialog.set_transient_for(*this);

            errorDialog.run();
        }

    m_undoAction->set_enabled(false);
    m_redoAction->set_enabled(false);
}

void AppWindow::onUndoAction()
{
    m_document->undoCommand();
}

void AppWindow::onRedoAction()
{

    m_document->redoCommand();
}

void AppWindow::onRemoveSelectedPages()
{
    m_document->removePages(m_view.getSelectedChildrenIndexes());
}

void AppWindow::onRemoveUnselectedPages()
{
    m_document->removePages(m_view.getUnselectedChildrenIndexes());
}

void AppWindow::onRemovePreviousPages()
{
    m_document->removePageRange(0, m_view.getSelectedChildIndex() - 1);
}

void AppWindow::onRemoveNextPages()
{
    m_document->removePageRange(m_view.getSelectedChildIndex() + 1,
                                static_cast<int>(m_document->pages()->get_n_items()) - 1);
}

void AppWindow::onRotatePagesRight()
{
    m_document->rotatePagesRight(m_view.getSelectedChildrenIndexes());
}

void AppWindow::onRotatePagesLeft()
{
    m_document->rotatePagesLeft(m_view.getSelectedChildrenIndexes());
}

void AppWindow::onPreviewPage()
{
    const int pageNumber = m_view.getSelectedChildIndex();

    Glib::RefPtr<Slicer::Page> page
        = m_document->pages()->get_item(static_cast<unsigned>(pageNumber));

    (new Slicer::PreviewWindow{page, m_backgroundThread})->show();
}

void AppWindow::onCancelSelection()
{
    m_view.unselect_all();
}

void AppWindow::onSelectedPagesChanged()
{
    const unsigned long numSelected = m_view.get_selected_children().size();

    if (numSelected == 0) {
        m_removeSelectedAction->set_enabled(false);
        m_removeUnselectedAction->set_enabled(false);
        m_removePreviousAction->set_enabled(false);
        m_removeNextAction->set_enabled(false);
        m_rotateRightAction->set_enabled(false);
        m_rotateLeftAction->set_enabled(false);
        m_cancelSelectionAction->set_enabled(false);
    }
    else {
        m_removeSelectedAction->set_enabled();
        m_rotateRightAction->set_enabled();
        m_rotateLeftAction->set_enabled();
        m_removeUnselectedAction->set_enabled();
        m_cancelSelectionAction->set_enabled();
    }

    if (numSelected == 1) {
        m_previewPageAction->set_enabled();

        const int index = m_view.getSelectedChildIndex();
        if (index == 0)
            m_removePreviousAction->set_enabled(false);
        else
            m_removePreviousAction->set_enabled();

        const int numPages = static_cast<int>(m_view.get_children().size());
        if (index == numPages - 1)
            m_removeNextAction->set_enabled(false);
        else
            m_removeNextAction->set_enabled();
    }
    else {
        m_previewPageAction->set_enabled(false);
    }

    if (numSelected > 1) {
        m_removePreviousAction->set_enabled(false);
        m_removeNextAction->set_enabled(false);
    }
}

void AppWindow::onCommandExecuted()
{
    if (m_document->canUndo())
        m_undoAction->set_enabled();
    else
        m_undoAction->set_enabled(false);

    if (m_document->canRedo())
        m_redoAction->set_enabled();
    else
        m_redoAction->set_enabled(false);
}
}
