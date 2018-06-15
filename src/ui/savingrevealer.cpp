#include "savingrevealer.hpp"
#include <glibmm/i18n.h>

namespace Slicer {

SavingRevealer::SavingRevealer()
{
    m_labelDone.set_label(_("Saved!"));
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
    add(m_boxDone);
    set_halign(Gtk::ALIGN_CENTER);
    set_valign(Gtk::ALIGN_START);

    m_buttonCloseDone.signal_clicked().connect([this]() {
        set_reveal_child(false);
    });
}

void SavingRevealer::saved()
{
    // Prevent any previous previous saving operation started
    // from hiding the popup prematurely
    if (m_connectionSaved.connected())
        m_connectionSaved.disconnect();

    // Show notification and automatically hide it later
    set_reveal_child(true);
    m_connectionSaved = Glib::signal_timeout().connect([this]() {
        set_reveal_child(false);
        return false;
    },
                                                       5000);
}

} // namespace Slicer
