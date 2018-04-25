#include "welcomescreen.hpp"
#include <glibmm/i18n.h>

namespace Slicer {

WelcomeScreen::WelcomeScreen()
    : Gtk::Box{Gtk::ORIENTATION_VERTICAL}
{
    m_icon.set_from_icon_name("edit-cut-symbolic", Gtk::ICON_SIZE_BUTTON);
    m_icon.set_pixel_size(256);

    m_label.set_label(_("Open a document to start slicing!"));
    m_label.get_style_context()->add_class("dim-label");
    m_label.get_style_context()->add_class("welcome-label");

    m_box.set_orientation(Gtk::ORIENTATION_VERTICAL);
    m_box.set_valign(Gtk::ALIGN_CENTER);
    m_box.pack_start(m_icon, false, false, 10);
    m_box.pack_start(m_label, false, false, 10);

    pack_start(m_box); // NOLINT
}

} // namespace Slicer
