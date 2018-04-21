#ifndef ACTIONBAR_HPP
#define ACTIONBAR_HPP

#include <gtkmm/actionbar.h>
#include <gtkmm/button.h>

namespace Slicer {

class ActionBar : public Gtk::ActionBar {
public:
    ActionBar();

private:
    Gtk::Button m_buttonRemovePages;
    Gtk::Button m_buttonRemovePrevious;
    Gtk::Button m_buttonRemoveNext;

    Gtk::Button m_buttonCancelSelection;
};

} // namespace Slicer

#endif // ACTIONBAR_HPP
