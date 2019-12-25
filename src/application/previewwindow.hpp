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

#ifndef PREVIEWWINDOW_HPP
#define PREVIEWWINDOW_HPP

#include <page.hpp>
#include "pagewidget.hpp"
#include "taskrunner.hpp"
#include "zoomlevelwithactions.hpp"
#include <glibmm/dispatcher.h>
#include <gtkmm/window.h>
#include <giomm/simpleactiongroup.h>
#include <gtkmm/image.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/overlay.h>
#include <gtkmm/button.h>
#include <gtkmm/box.h>

namespace Slicer {

class PreviewWindow : public Gtk::Window {
public:
    PreviewWindow(const Glib::RefPtr<const Page>& page,
                  TaskRunner& taskRunner);
    virtual ~PreviewWindow();

private:
    Glib::RefPtr<const Page> m_page;
    TaskRunner& m_taskRunner;
	Glib::RefPtr<Gio::SimpleActionGroup> m_actionGroup;
	ZoomLevelWithActions m_zoomLevel;
    static const std::vector<int> zoomLevels;

    std::shared_ptr<PageWidget> m_pageWidget;
	Gtk::ScrolledWindow m_scroller;
	Gtk::Overlay m_overlay;
	Gtk::Button m_buttonZoomIn;
	Gtk::Button m_buttonZoomOut;
	Gtk::Box m_boxZoom;

    std::shared_ptr<Task> m_renderingTask;

    void setTitle();
	void setupWidgets();
	void setupSignalHandlers();
	void loadCustomCSS();
	void renderPage();
};

} // namespace Slicer

#endif // PREVIEWWINDOW_HPP
