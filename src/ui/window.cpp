#include "window.hpp"
#include "openfiledialog.hpp"
#include "savefiledialog.hpp"
#include <glibmm/main.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/popovermenu.h>
#include <gtkmm/cssprovider.h>
#include <gtkmm/adjustment.h>

namespace Slicer {

AppWindow::AppWindow()
    : m_boxMenuRemoveOptions{Gtk::ORIENTATION_VERTICAL}
    , m_zoomLevel{{200, 300, 400}}
    , m_view{nullptr}
    , m_labelDone{"Saved!"}
{
    set_size_request(500, 500);
    set_default_size(800, 600);

    addActions();

    // Widget setup
    set_titlebar(m_headerBar);
    m_headerBar.set_title("PDF Slicer");
    m_headerBar.set_show_close_button();

    m_buttonOpen.set_image_from_icon_name("document-open-symbolic");
    m_buttonOpen.set_tooltip_text("Open document...");
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonOpen.gobj()), "win.open-document");
    m_headerBar.pack_start(m_buttonOpen);

    m_buttonSave.set_image_from_icon_name("document-save-symbolic");
    m_buttonSave.set_tooltip_text("Save as...");
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonSave.gobj()), "win.save-document");
    m_headerBar.pack_start(m_buttonSave);

    m_buttonRemovePages.set_image_from_icon_name("edit-delete-symbolic");
    m_buttonRemovePages.set_tooltip_text("Remove the selected page");
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonRemovePages.gobj()), "win.remove-selected");
    m_boxRemovePages.pack_start(m_buttonRemovePages);

    m_buttonRemovePrevious.set_label("Remove previous pages");
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonRemovePrevious.gobj()), "win.remove-previous");
    m_buttonRemoveNext.set_label("Remove next pages");
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonRemoveNext.gobj()), "win.remove-next");

    m_boxMenuRemoveOptions.pack_start(m_buttonRemovePrevious);
    m_boxMenuRemoveOptions.pack_start(m_buttonRemoveNext);
    m_boxMenuRemoveOptions.set_margin_top(10);
    m_boxMenuRemoveOptions.set_margin_bottom(10);
    m_boxMenuRemoveOptions.set_margin_left(10);
    m_boxMenuRemoveOptions.set_margin_right(10);

    auto menuRemoveOptions = Gtk::manage(new Gtk::PopoverMenu);
    menuRemoveOptions->add(m_boxMenuRemoveOptions);
    menuRemoveOptions->show_all_children();

    m_buttonRemoveOptions.set_tooltip_text("More removing options");
    m_buttonRemoveOptions.get_style_context()->add_class("pepino");
    m_buttonRemoveOptions.set_popover(*menuRemoveOptions);
    m_buttonRemoveOptions.set_sensitive(false);
    m_boxRemovePages.pack_start(m_buttonRemoveOptions);

    m_boxRemovePages.get_style_context()->add_class("linked");
    m_headerBar.pack_start(m_boxRemovePages);

    m_buttonUndo.set_image_from_icon_name("edit-undo-symbolic");
    m_buttonUndo.set_tooltip_text("Undo");
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonUndo.gobj()), "win.undo");
    m_buttonRedo.set_image_from_icon_name("edit-redo-symbolic");
    m_buttonRedo.set_tooltip_text("Redo");
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonRedo.gobj()), "win.redo");

    auto undoBox = Gtk::manage(new Gtk::Box);
    undoBox->get_style_context()->add_class("linked");
    undoBox->pack_start(m_buttonUndo);
    undoBox->pack_start(m_buttonRedo);
    m_headerBar.pack_start(*undoBox);

    m_buttonPreviewPage.set_image_from_icon_name("document-print-preview-symbolic");
    m_buttonPreviewPage.set_tooltip_text("Preview the selected page");
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonPreviewPage.gobj()), "win.preview-selected");
    m_headerBar.pack_end(m_buttonPreviewPage);

    m_buttonZoomOut.set_image_from_icon_name("zoom-out-symbolic");
    m_buttonZoomOut.set_tooltip_text("Zoom out");
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonZoomOut.gobj()), "win.zoom-out");
    m_buttonZoomIn.set_image_from_icon_name("zoom-in-symbolic");
    m_buttonZoomIn.set_tooltip_text("Zoom in");
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonZoomIn.gobj()), "win.zoom-in");
    m_boxZoom.pack_start(m_buttonZoomOut);
    m_boxZoom.pack_start(m_buttonZoomIn);
    m_boxZoom.get_style_context()->add_class("linked");
    m_headerBar.pack_end(m_boxZoom);

    m_labelDone.set_margin_top(10);
    m_labelDone.set_margin_bottom(10);
    m_labelDone.set_margin_left(15);
    m_labelDone.set_margin_right(7);
    m_buttonCloseDone.set_image_from_icon_name("window-close-symbolic");
    m_buttonCloseDone.get_style_context()->add_class("flat");
    m_buttonCloseDone.set_margin_top(5);
    m_buttonCloseDone.set_margin_bottom(5);
    m_buttonCloseDone.set_margin_right(5);
    m_boxDone.pack_start(m_labelDone, true, true);
    m_boxDone.pack_start(m_buttonCloseDone, false, false);
    m_boxDone.get_style_context()->add_class("osd");
    m_boxDone.set_size_request(1, 35);
    m_revealerDone.add(m_boxDone);
    m_revealerDone.set_halign(Gtk::ALIGN_CENTER);
    m_revealerDone.set_valign(Gtk::ALIGN_START);

    m_scroller.add(m_welcomeScreen);

    m_overlay.add(m_scroller);
    m_overlay.add_overlay(m_revealerDone);

    add(m_overlay); // NOLINT

    // Signal handlers
    m_buttonCloseDone.signal_clicked().connect([this]() {
        m_revealerDone.set_reveal_child(false);
    });

    m_signalSaved.connect([this]() {
        // Stop any previous hiding animation started
        if (m_connectionSaved.connected())
            m_connectionSaved.disconnect();

        // Show notification and automatically hide it later
        m_revealerDone.set_reveal_child(true);
        m_connectionSaved = Glib::signal_timeout().connect([this]() {
            m_revealerDone.set_reveal_child(false);
            return false;
        },
                                                           5000);
    });

    m_zoomLevel.changed.connect([this](int) {
        onZoomLevelChanged();
    });

    loadCustomCSS();

    show_all_children();
}

void AppWindow::openDocument(const Glib::RefPtr<Gio::File>& file)
{
    m_document = std::make_unique<Slicer::Document>(file->get_path());
    m_document->commandExecuted().connect(sigc::mem_fun(*this, &AppWindow::onCommandExecuted));
    m_headerBar.set_subtitle(file->get_basename());
    m_saveAction->set_enabled();
    buildView();
}

void AppWindow::addActions()
{
    m_openAction = add_action("open-document", sigc::mem_fun(*this, &AppWindow::onOpenAction));
    m_saveAction = add_action("save-document", sigc::mem_fun(*this, &AppWindow::onSaveAction));
    m_removeSelectedAction = add_action("remove-selected", sigc::mem_fun(*this, &AppWindow::removeSelectedPage));
    m_removePreviousAction = add_action("remove-previous", sigc::mem_fun(*this, &AppWindow::removePreviousPages));
    m_removeNextAction = add_action("remove-next", sigc::mem_fun(*this, &AppWindow::removeNextPages));
    m_previewPageAction = add_action("preview-selected", sigc::mem_fun(*this, &AppWindow::previewPage));
    m_zoomInAction = add_action("zoom-in", sigc::mem_fun(*this, &AppWindow::onZoomInAction));
    m_zoomOutAction = add_action("zoom-out", sigc::mem_fun(*this, &AppWindow::onZoomOutAction));
    m_undoAction = add_action("undo", sigc::mem_fun(*this, &AppWindow::onUndoAction));
    m_redoAction = add_action("redo", sigc::mem_fun(*this, &AppWindow::onRedoAction));

    m_saveAction->set_enabled(false);
    m_removeSelectedAction->set_enabled(false);
    m_removePreviousAction->set_enabled(false);
    m_removeNextAction->set_enabled(false);
    m_previewPageAction->set_enabled(false);
    m_zoomInAction->set_enabled(false);
    m_zoomOutAction->set_enabled(false);
    m_undoAction->set_enabled(false);
    m_redoAction->set_enabled(false);
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

void AppWindow::removeSelectedPage()
{
    Gtk::FlowBoxChild* child = m_view->get_selected_children().at(0);
    const int index = child->get_index();
    m_document->removePage(index);
}

void AppWindow::removePreviousPages()
{
    std::vector<Gtk::FlowBoxChild*> selected = m_view->get_selected_children();

    if (selected.size() != 1)
        throw std::runtime_error(
            "Tried to remove previous pages with more "
            "than one page selected. This should never happen!");

    const int index = selected.at(0)->get_index();

    m_document->removePageRange(0, index - 1);
}

void AppWindow::removeNextPages()
{
    std::vector<Gtk::FlowBoxChild*> selected = m_view->get_selected_children();

    if (selected.size() != 1)
        throw std::runtime_error(
            "Tried to remove next pages with more "
            "than one page selected. This should never happen!");

    const int index = selected.at(0)->get_index();

    m_document->removePageRange(index + 1,
                                static_cast<int>(m_document->pages()->get_n_items()) - 1);
}

void AppWindow::previewPage()
{
    // We need to wait till the thumbnails finish rendering
    // before rendering a big preview, to prevent crashes.
    // Poppler isn't designed for rendering many pages of
    // the same document in different threads.
    m_view->waitForRenderCompletion();

    const int pageNumber = m_view->get_selected_children().at(0)->get_index();

    Glib::RefPtr<Slicer::Page> page
        = m_document->pages()->get_item(static_cast<unsigned>(pageNumber));
    m_previewWindow = std::make_unique<Slicer::PreviewWindow>(page);

    m_previewWindow->set_modal();
    m_previewWindow->set_transient_for(*this);
    m_previewWindow->show();
}

void AppWindow::buildView()
{
    if (m_view == nullptr)
        m_zoomInAction->set_enabled();

    m_scroller.remove();

    // It is necessary to invoke an empty reset() as a separate command.
    // reset() erases the old object it owns as the last thing it does.
    // So it may be the case that the new View starts rendering pages while the
    // old one is still rendering pages.
    // Poppler doesn't like two threads rendering pages from the same PDF, so this
    // would crash the program.
    m_view.reset();
    m_view = std::make_unique<Slicer::View>(*m_document, m_zoomLevel.currentLevel());

    m_scroller.add(*m_view);
    m_scroller.show_all_children();

    m_buttonSave.set_sensitive(true);

    m_view->signal_selected_children_changed().connect([this]() {
        const unsigned long numSelected = m_view->get_selected_children().size();

        if (numSelected == 0)
            m_removeSelectedAction->set_enabled(false);
        else
            m_removeSelectedAction->set_enabled();

        if (numSelected == 1) {
            m_buttonRemoveOptions.set_sensitive(true);
            m_previewPageAction->set_enabled();

            const int index = m_view->get_selected_children().at(0)->get_index();
            if (index == 0)
                m_removePreviousAction->set_enabled(false);
            else
                m_removePreviousAction->set_enabled();

            if (index == static_cast<int>(m_view->get_children().size()) - 1)
                m_removeNextAction->set_enabled(false);
            else
                m_removeNextAction->set_enabled();
        }
        else {
            m_buttonRemoveOptions.set_sensitive(false);
            m_previewPageAction->set_enabled(false);
        }
    });

    m_view->signal_child_activated().connect([this](Gtk::FlowBoxChild*) {
        m_previewPageAction->activate();
    });
}

void AppWindow::manageZoomActionsState()
{
    if (m_zoomLevel.currentLevel() == m_zoomLevel.maxLevel())
        m_zoomInAction->set_enabled(false);
    else
        m_zoomInAction->set_enabled();

    if (m_zoomLevel.currentLevel() == m_zoomLevel.minLevel())
        m_zoomOutAction->set_enabled(false);
    else
        m_zoomOutAction->set_enabled();
}

void AppWindow::onSaveAction()
{
    Slicer::SaveFileDialog dialog{*this};

    const int result = dialog.run();

    if (result == Gtk::RESPONSE_OK) {
        m_document->saveDocument(dialog.getSavePath());
        m_signalSaved.emit();
        openDocument(Gio::File::create_for_path(dialog.getSavePath()));
    }
}

void AppWindow::onOpenAction()
{
    Slicer::OpenFileDialog dialog{*this};

    const int result = dialog.run();

    if (result == Gtk::RESPONSE_OK) {
        openDocument(dialog.get_file());
    }
}

void AppWindow::onZoomInAction()
{
    ++m_zoomLevel;
}

void AppWindow::onZoomOutAction()
{
    --m_zoomLevel;
}

void AppWindow::onZoomLevelChanged()
{
    manageZoomActionsState();
    buildView();
}

void AppWindow::onUndoAction()
{
    m_view->waitForRenderCompletion();
    m_document->undoCommand();
}

void AppWindow::onRedoAction()
{
    m_view->waitForRenderCompletion();
    m_document->redoCommand();
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
