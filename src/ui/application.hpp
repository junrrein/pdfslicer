#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "window.hpp"
#include <gtkmm/application.h>

namespace Slicer {

class Application : public Gtk::Application {
public:
    static Glib::RefPtr<Application> create();
    virtual ~Application() override = default;

private:
    Application();

    AppWindow* createWindow();

    void on_startup() override;
    void on_activate() override;
    void on_open(const Gio::Application::type_vec_files& files,
                 const Glib::ustring& hint) override;

    void onActionAbout();
    void onActionQuit();
};

} // namespace Slicer

#endif // APPLICATION_HPP
