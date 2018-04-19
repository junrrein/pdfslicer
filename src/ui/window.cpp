#include "window.hpp"
#include "openfiledialog.hpp"
#include "savefiledialog.hpp"
#include <glibmm/main.h>
#include <gtkmm/cssprovider.h>

namespace Slicer {

AppWindow::AppWindow()
    : m_headerBar{*this}
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
    m_undoAction = add_action("undo", sigc::mem_fun(*this, &AppWindow::onUndoAction));
    m_redoAction = add_action("redo", sigc::mem_fun(*this, &AppWindow::onRedoAction));

    m_saveAction->set_enabled(false);
    m_undoAction->set_enabled(false);
    m_redoAction->set_enabled(false);
}

void AppWindow::setupWidgets()
{
    m_view = nullptr;

    set_titlebar(m_headerBar);

    m_labelDone.set_label("Saved!");
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
}

void AppWindow::setupSignalHandlers()
{
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

void AppWindow::buildView()
{
    m_scroller.remove();

    // It is necessary to invoke an empty reset() as a separate command.
    // reset() erases the old object it owns as the last thing it does.
    // So it may be the case that the new View starts rendering pages while the
    // old one is still rendering pages.
    // Poppler doesn't like two threads rendering pages from the same PDF, so this
    // would crash the program.
    m_view.reset();
    m_view = std::make_unique<Slicer::View>(*this, *m_document);

    m_scroller.add(*m_view);
    m_scroller.show_all_children();
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
