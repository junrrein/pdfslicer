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

#include "previewwindow.hpp"
#include <gtkmm/cssprovider.h>
#include <glibmm/i18n.h>

namespace Slicer {

const std::set<int> PreviewWindow::zoomLevels = {1000, 1400, 1800};

PreviewWindow::PreviewWindow(const Glib::RefPtr<Page>& page, BackgroundThread& backgroundThread)
	: m_page{page}
	, m_backgroundThread{backgroundThread}
	, m_actionGroup{Gio::SimpleActionGroup::create()}
	, m_zoomLevel{zoomLevels, *(m_actionGroup.operator->())}
	, m_pageWidget{std::make_unique<PageWidget>(m_page, m_zoomLevel.currentLevel())}
{
	set_title(_("Preview"));
	set_size_request(400, 400);
	set_default_size(900, 600);

	insert_action_group("preview", m_actionGroup);

	setupWidgets();
	setupSignalHandlers();
	loadCustomCSS();
	renderPage();

	show_all_children();
}

void PreviewWindow::setupWidgets()
{
	m_buttonZoomOut.set_image_from_icon_name("zoom-out-symbolic");
	m_buttonZoomOut.set_tooltip_text(_("Zoom out"));
	m_buttonZoomOut.get_style_context()->add_class("flat");
	gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonZoomOut.gobj()), "preview.zoom-out"); // NOLINT
	m_buttonZoomOut.set_margin_top(8);
	m_buttonZoomOut.set_margin_bottom(8);
	m_buttonZoomOut.set_margin_left(8);
	m_buttonZoomIn.set_image_from_icon_name("zoom-in-symbolic");
	m_buttonZoomIn.set_tooltip_text(_("Zoom in"));
	m_buttonZoomIn.get_style_context()->add_class("flat");
	gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonZoomIn.gobj()), "preview.zoom-in"); // NOLINT
	m_buttonZoomIn.set_margin_top(8);
	m_buttonZoomIn.set_margin_bottom(8);
	m_buttonZoomIn.set_margin_right(8);
	m_boxZoom.pack_start(m_buttonZoomOut);
	m_boxZoom.pack_start(m_buttonZoomIn);
	m_boxZoom.get_style_context()->add_class("osd");
	m_boxZoom.set_valign(Gtk::ALIGN_END);
	m_boxZoom.set_halign(Gtk::ALIGN_END);
	m_boxZoom.set_margin_bottom(15);
	m_boxZoom.set_margin_right(15);

	m_scroller.add(*m_pageWidget);
	m_overlay.add(m_scroller);
	m_overlay.add_overlay(m_boxZoom);
	add(m_overlay); // NOLINT
}

void PreviewWindow::setupSignalHandlers()
{
	m_zoomLevel.enable();

	m_zoomLevel.changed.connect([this](int level) {
		m_scroller.remove();
		m_pageWidget = std::make_unique<PageWidget>(m_page, level);
		m_scroller.add(*m_pageWidget);
		renderPage();
	});

	m_pageRenderedDispatcher.connect([this]() {
		m_pageWidget->showPage();
	});

	signal_hide().connect([this]() {
		delete this;
	});
}

void PreviewWindow::loadCustomCSS()
{
	auto screen = Gdk::Screen::get_default();
	auto provider = Gtk::CssProvider::create();
	provider->load_from_data(R"(
							 overlay > box {
								border-radius: 8px;
							 }
							 )");
	Gtk::StyleContext::add_provider_for_screen(screen,
											   provider,
											   GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

void PreviewWindow::renderPage()
{
	m_backgroundThread.push([this]() {
		m_pageWidget->renderPage();
		m_pageRenderedDispatcher.emit();
	});
}

} // namespace Slicer
