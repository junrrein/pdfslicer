#include "headerbar.hpp"
#include <gtkmm/popovermenu.h>

namespace Slicer {

HeaderBar::HeaderBar(Gtk::ApplicationWindow& window)
{
    set_title("PDF Slicer");
    set_show_close_button();

    m_buttonOpen.set_image_from_icon_name("document-open-symbolic");
    m_buttonOpen.set_tooltip_text("Open document...");
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonOpen.gobj()), "win.open-document"); // NOLINT
    pack_start(m_buttonOpen);

    m_buttonSave.set_image_from_icon_name("document-save-symbolic");
    m_buttonSave.set_tooltip_text("Save as...");
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonSave.gobj()), "win.save-document"); // NOLINT
    pack_start(m_buttonSave);

    m_buttonRemovePages.set_image_from_icon_name("edit-delete-symbolic");
    m_buttonRemovePages.set_tooltip_text("Remove the selected page");
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonRemovePages.gobj()), "win.remove-selected"); // NOLINT
    m_boxRemovePages.pack_start(m_buttonRemovePages);

    m_buttonRemovePrevious.set_label("Remove previous pages");
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonRemovePrevious.gobj()), "win.remove-previous"); // NOLINT
    m_buttonRemoveNext.set_label("Remove next pages");
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonRemoveNext.gobj()), "win.remove-next"); // NOLINT

    m_boxMenuRemoveOptions.set_orientation(Gtk::ORIENTATION_VERTICAL);
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
    pack_start(m_boxRemovePages);

    m_buttonUndo.set_image_from_icon_name("edit-undo-symbolic");
    m_buttonUndo.set_tooltip_text("Undo");
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonUndo.gobj()), "win.undo"); // NOLINT
    m_buttonRedo.set_image_from_icon_name("edit-redo-symbolic");
    m_buttonRedo.set_tooltip_text("Redo");
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonRedo.gobj()), "win.redo"); // NOLINT

    auto undoBox = Gtk::manage(new Gtk::Box);
    undoBox->get_style_context()->add_class("linked");
    undoBox->pack_start(m_buttonUndo);
    undoBox->pack_start(m_buttonRedo);
    pack_start(*undoBox);

    m_buttonPreviewPage.set_image_from_icon_name("document-print-preview-symbolic");
    m_buttonPreviewPage.set_tooltip_text("Preview the selected page");
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonPreviewPage.gobj()), "win.preview-selected"); // NOLINT
    pack_end(m_buttonPreviewPage);

    m_buttonZoomOut.set_image_from_icon_name("zoom-out-symbolic");
    m_buttonZoomOut.set_tooltip_text("Zoom out");
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonZoomOut.gobj()), "win.zoom-out"); // NOLINT
    m_buttonZoomIn.set_image_from_icon_name("zoom-in-symbolic");
    m_buttonZoomIn.set_tooltip_text("Zoom in");
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonZoomIn.gobj()), "win.zoom-in"); // NOLINT
    m_boxZoom.pack_start(m_buttonZoomOut);
    m_boxZoom.pack_start(m_buttonZoomIn);
    m_boxZoom.get_style_context()->add_class("linked");
    pack_end(m_boxZoom);

    auto buttonRemoveEnabler = [&](const Glib::ustring&, bool) {
        const bool removePreviousEnabled = window.get_action_enabled("remove-previous");
        const bool removeNextEnabled = window.get_action_enabled("remove-next");

        if (removePreviousEnabled || removeNextEnabled)
            m_buttonRemoveOptions.set_sensitive();
        else
            m_buttonRemoveOptions.set_sensitive(false);
    };

    window.signal_action_enabled_changed("remove-previous").connect(buttonRemoveEnabler);
    window.signal_action_enabled_changed("remove-next").connect(buttonRemoveEnabler);
}

} // namespace Slicer
