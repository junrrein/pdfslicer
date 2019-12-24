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

#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "appwindow.hpp"
#include <gtkmm/application.h>
#include <giomm/simpleaction.h>

namespace Slicer {

class Application : public Gtk::Application {
public:
    static Glib::RefPtr<Application> create();
    virtual ~Application() override = default;

private:
    BackgroundThread m_backgroundThread;
    SettingsManager m_settingsManager;

    Glib::RefPtr<Gio::SimpleAction> m_newWindowAction;

    Application();
    AppWindow* createWindow();

    void addActions();
    void addAccels();

    void on_startup() override;
    void on_activate() override;
    void on_open(const Gio::Application::type_vec_files& files,
                 const Glib::ustring& hint) override;
    void onNewWindowAction();
};

} // namespace Slicer

#endif // APPLICATION_HPP
