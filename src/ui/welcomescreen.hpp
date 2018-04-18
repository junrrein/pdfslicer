#ifndef WELCOMESCREEN_HPP
#define WELCOMESCREEN_HPP

#include <gtkmm/box.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>

namespace Slicer {

class WelcomeScreen : public Gtk::Box {
public:
    WelcomeScreen();

private:
    Gtk::Image m_icon;
    Gtk::Label m_label;
    Gtk::Box m_box;
};

} // namespace Slicer

#endif // WELCOMESCREEN_HPP
