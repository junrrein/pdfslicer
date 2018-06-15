#include "savingrevealer.hpp"
#include <glibmm/i18n.h>

namespace Slicer {

SavingRevealer::SavingRevealer()
{
    auto parentBox = Gtk::manage(new Gtk::Box{Gtk::ORIENTATION_HORIZONTAL});

    m_labelSaving.set_label(_("Saving…"));
    m_boxSaving.pack_start(m_labelSaving, Gtk::PACK_EXPAND_WIDGET);
    m_boxSaving.pack_start(m_spinner, Gtk::PACK_SHRINK);
    m_boxSaving.set_spacing(10);
    m_labelSaving.set_margin_left(10);
    m_spinner.set_margin_right(10);

    m_labelDone.set_label(_("Saved!"));
    m_iconDone.set_from_icon_name("emblem-ok-symbolic", Gtk::ICON_SIZE_SMALL_TOOLBAR);
    m_boxDone.pack_start(m_labelDone, Gtk::PACK_EXPAND_WIDGET);
    m_boxDone.pack_start(m_iconDone, Gtk::PACK_SHRINK);
    m_boxDone.set_spacing(10);
    m_labelDone.set_margin_left(10);
    m_iconDone.set_margin_right(10);

    parentBox->pack_start(m_boxSaving);
    parentBox->pack_start(m_boxDone);
    parentBox->get_style_context()->add_class("osd");
    parentBox->set_size_request(1, 35);
    add(*parentBox);

    set_halign(Gtk::ALIGN_CENTER);
    set_valign(Gtk::ALIGN_START);
}

void SavingRevealer::saving()
{
    m_boxSaving.show();
    m_boxDone.hide();
    m_spinner.start();

    // Prevent any previous previous saving operation started
    // from hiding the popup prematurely
    if (m_connectionSaved.connected())
        m_connectionSaved.disconnect();

    set_reveal_child(true);
}

void SavingRevealer::saved()
{
    m_boxDone.show();
    m_boxSaving.hide();
    m_spinner.stop();

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
