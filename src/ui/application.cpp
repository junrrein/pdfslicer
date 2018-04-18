#include "application.hpp"
#include "aboutdialog.hpp"

namespace Slicer {

Glib::RefPtr<Application> Application::create()
{
    return Glib::RefPtr<Application>{new Application{}};
}

Application::Application()
    : Gtk::Application{"com.junrrein.pdfslicer", Gio::APPLICATION_HANDLES_OPEN}
{
}

void Application::on_startup()
{
    Gtk::Application::on_startup();

    Gtk::Window::set_default_icon_name("edit-cut-symbolic");

    add_action("about", sigc::mem_fun(*this, &Application::onActionAbout));
    add_action("quit", sigc::mem_fun(*this, &Application::onActionQuit));

    auto menu = Gio::Menu::create();
    menu->append("About", "app.about");
    menu->append("Quit", "app.quit");
    set_app_menu(menu);
}

void Application::on_activate()
{
    createWindow()->present();
}

void Application::on_open(const Application::type_vec_files& files,
                          __attribute__((unused)) const Glib::ustring& hint)
{
    for (const Glib::RefPtr<Gio::File>& file : files) {
        AppWindow* window = createWindow();
        window->openDocument(file);
        window->present();
    }
}

void Application::onActionAbout()
{
    new Slicer::AboutDialog{*get_active_window()};
}

void Application::onActionQuit()
{
    for (Gtk::Window* window : get_windows())
        window->hide();
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
