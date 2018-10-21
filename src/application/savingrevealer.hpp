#ifndef SAVINGREVEALER_HPP
#define SAVINGREVEALER_HPP

#include <glibmm/dispatcher.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <gtkmm/revealer.h>
#include <gtkmm/spinner.h>

namespace Slicer {

class SavingRevealer : public Gtk::Revealer {
public:
    SavingRevealer();

    void saving();
    void saved();

private:
    Gtk::Box m_boxSaving;
    Gtk::Label m_labelSaving;
    Gtk::Spinner m_spinner;
    Gtk::Box m_boxDone;
    Gtk::Label m_labelDone;
    Gtk::Image m_iconDone;

    sigc::connection m_connectionSaved;
};

} // namespace Slicer

#endif // SAVINGREVEALER_HPP
