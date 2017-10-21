#include "application.hpp"

namespace Slicer {

Glib::RefPtr<Application> Application::create()
{
    return Glib::RefPtr<Application>{new Application{}};
}

Application::Application()
    : Gtk::Application{"com.junrrein.pdfslicer", Gio::APPLICATION_HANDLES_OPEN}
{
}

void Application::on_activate()
{
    auto window = createWindow();
    window->present();
}

void Application::on_open(const Application::type_vec_files& files,
                          const Glib::ustring&)
{
    for (const auto& file : files) {
        AppWindow* window = createWindow();
        window->openDocument(file);
        window->present();
    }
}

AppWindow* Application::createWindow()
{
    auto window = new Slicer::AppWindow{};

    window->signal_hide().connect([window]() {
        delete window;
    });

    add_window(*window);

    return window;
}

} // namespace Slicer
