#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "window.hpp"
#include <gtkmm/application.h>

namespace Slicer {

class Application : public Gtk::Application {
public:
    static Glib::RefPtr<Application> create();

private:
    Application();

    void on_activate() override;
    void on_open(const Gio::Application::type_vec_files& files,
                 const Glib::ustring& hint) override;

    AppWindow* createWindow();
};

} // namespace Slicer

#endif // APPLICATION_HPP
