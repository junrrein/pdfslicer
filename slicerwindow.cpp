#include "slicerwindow.hpp"
#include <gtkmm/filechooserdialog.h>
#include <glibmm/main.h>

namespace Slicer {

Window::Window(std::string filePath)
    : m_document{filePath}
    , m_view{m_document}
    , m_labelDone{"Saved!"}
{
    // Widget setup
    set_titlebar(m_headerBar);
    set_default_size(800, 600);

    m_headerBar.set_title("PDF Slicer");
    m_headerBar.set_show_close_button(true);
    m_headerBar.set_has_subtitle(false);

    m_buttonSave.set_image_from_icon_name("document-save");
    m_headerBar.pack_start(m_buttonSave);

    m_scroller.add(m_view);

    m_labelDone.set_margin_left(10);
    m_labelDone.set_margin_right(10);
    m_labelDone.set_margin_top(10);
    m_labelDone.set_margin_bottom(10);
    m_buttonDoneClose.set_image_from_icon_name("window-close",
                                               Gtk::ICON_SIZE_SMALL_TOOLBAR);
    m_boxDone.pack_start(m_labelDone);
    m_boxDone.pack_start(m_buttonDoneClose);
    auto styleContext = m_boxDone.get_style_context();
    styleContext->add_class("osd");
    m_revealerDone.add(m_boxDone);
    m_revealerDone.set_halign(Gtk::ALIGN_CENTER);
    m_revealerDone.set_valign(Gtk::ALIGN_START);

    m_overlay.add(m_scroller);
    m_overlay.add_overlay(m_revealerDone);

    add(m_overlay);

    // Signal handlers
    m_buttonSave.signal_clicked().connect([this]() {
        onSaveAction();
    });

    m_buttonDoneClose.signal_clicked().connect([this]() {
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

    show_all_children();
}

void Window::onSaveAction()
{
    Gtk::FileChooserDialog dialog{*this,
                                  "Save file as",
                                  Gtk::FILE_CHOOSER_ACTION_SAVE};
    dialog.set_transient_for(*this);

    dialog.add_button("Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("Save", Gtk::RESPONSE_OK);

    const int result = dialog.run();

    if (result == Gtk::RESPONSE_OK) {
        std::string filePath = dialog.get_filename();
        m_document.saveDocument(filePath);
        m_signalSaved.emit();
    }
}
}
