#include "slicerwindow.hpp"
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/modelbutton.h>
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
    m_buttonSave.set_tooltip_text("Save as...");
    m_headerBar.pack_start(m_buttonSave);

    m_buttonRemovePages.set_image_from_icon_name("edit-delete");
    m_buttonRemovePages.set_tooltip_text("Remove selected pages");
    m_boxRemovePages.pack_start(m_buttonRemovePages);

    m_buttonRemoveOptions.set_image_from_icon_name("go-down");
    m_buttonRemoveOptions.set_tooltip_text("More removing options");
    m_boxRemovePages.pack_start(m_buttonRemoveOptions);

    m_headerBar.pack_start(m_boxRemovePages);

    auto removePrevious = Gtk::manage(new Gtk::ModelButton());
    removePrevious->set_label("Remove previous pages");
    auto removeNext = Gtk::manage(new Gtk::ModelButton());
    removeNext->set_label("Remove next pages");
    auto menuBox = Gtk::manage(new Gtk::Box{Gtk::ORIENTATION_VERTICAL});
    menuBox->pack_start(*removePrevious);
    menuBox->pack_start(*removeNext);
    menuBox->set_margin_top(10);
    menuBox->set_margin_bottom(10);
    menuBox->set_margin_left(10);
    menuBox->set_margin_right(10);
    m_menuRemoveOptions.add(*menuBox);
    m_menuRemoveOptions.set_relative_to(m_buttonRemoveOptions);
    m_menuRemoveOptions.show_all_children();

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

    m_buttonRemovePages.signal_clicked().connect([this]() {
        m_view.removeSelectedPages();
    });

    m_buttonRemoveOptions.signal_clicked().connect([this]() {
        m_menuRemoveOptions.popup();
    });

    removePrevious->signal_clicked().connect([this]() {
        m_view.removePreviousPages();
    });

    removeNext->signal_clicked().connect([this]() {
        m_view.removeNextPages();
    });

    m_view.signal_selected_children_changed().connect([this, removePrevious, removeNext]() {
        const int numSelected = m_view.get_selected_children().size();

        if (numSelected == 0)
            m_buttonRemovePages.set_sensitive(false);
        else
            m_buttonRemovePages.set_sensitive(true);

        if (numSelected == 1) {
            m_buttonRemoveOptions.set_sensitive(true);

            const unsigned int index = m_view.get_selected_children().at(0)->get_index();
            if (index == 0)
                removePrevious->set_sensitive(false);
            else
                removePrevious->set_sensitive(true);

            if (index == m_view.get_children().size() - 1)
                removeNext->set_sensitive(false);
            else
                removeNext->set_sensitive(true);
        }
        else
            m_buttonRemoveOptions.set_sensitive(false);

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
