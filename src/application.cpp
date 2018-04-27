// PDF Slicer
// Copyright (C) 2017-2018 Julián Unrrein

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "application.hpp"
#include "ui/aboutdialog.hpp"
#include <gtkmm.h>
#include <glibmm/i18n.h>
#include <config.hpp>

namespace Slicer {

Glib::RefPtr<Application> Application::create()
{
    return Glib::RefPtr<Application>{new Application{}};
}

Application::Application()
    : Gtk::Application{config::PROGRAM_NAME, Gio::APPLICATION_HANDLES_OPEN}
{
    Glib::set_application_name(config::APP_NAME);
}

void Application::on_startup()
{
    Gtk::Application::on_startup();

    Gtk::Window::set_default_icon_name("edit-cut-symbolic");

    add_action("about", sigc::mem_fun(*this, &Application::onActionAbout));
    add_action("quit", sigc::mem_fun(*this, &Application::onActionQuit));

    auto menu = Gio::Menu::create();
    menu->append(_("About"), "app.about");
    menu->append(_("Quit"), "app.quit");
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
