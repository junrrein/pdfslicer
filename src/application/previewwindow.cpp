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
#include <fmt/format.h>

using namespace fmt::literals;

namespace Slicer {

const std::vector<int> PreviewWindow::zoomLevels = {1000, 1400, 1800, 2200, 2600};

PreviewWindow::PreviewWindow(const Glib::RefPtr<const Page>& page, TaskRunner& taskRunner)
    : m_page{page}
    , m_taskRunner{taskRunner}
    , m_actionGroup{Gio::SimpleActionGroup::create()}
    , m_zoomLevel{zoomLevels, *(m_actionGroup.operator->())}
{
    m_pageWidget = std::make_shared<PageWidget>(m_page, m_zoomLevel.currentLevel());

	set_size_request(400, 400);
	set_default_size(900, 600);

	insert_action_group("preview", m_actionGroup);

    setTitle();
	setupWidgets();
	setupSignalHandlers();
	loadCustomCSS();
	renderPage();

    show_all_children();
}

PreviewWindow::~PreviewWindow()
{
    m_pageWidget->cancelRendering();
}

void PreviewWindow::setTitle()
{
    set_title(fmt::format(_("Page {pageNumber}"),
                          "pageNumber"_a = m_page->indexInFile() + 1)); //NOLINT
}

void PreviewWindow::setupWidgets()
{
    set_titlebar(m_previewHeaderBar);

    m_eventBox.add(*m_pageWidget);
    m_scroller.add(m_eventBox);
	m_overlay.add(m_scroller);
	add(m_overlay); // NOLINT
}

void PreviewWindow::setupSignalHandlers()
{
    m_eventBox.add_events(Gdk::SCROLL_MASK | Gdk::SMOOTH_SCROLL_MASK);

    m_eventBox.signal_scroll_event().connect([this](GdkEventScroll* event) {
        if ((event->state & Gdk::CONTROL_MASK) != 0) {
            if (event->delta_y < 0) {
                m_actionGroup->activate_action("zoom-in");
                return true;
            }

            if (event->delta_y > 0) {
                m_actionGroup->activate_action("zoom-out");
                return true;
            }
        }

        return false;
    });

    m_zoomLevel.enable();

    m_zoomLevel.zoomLevelIndex().signal_changed().connect([this]() {
        m_pageWidget->cancelRendering();
        m_pageWidget->changeSize(m_zoomLevel.currentLevel());
        m_pageWidget->showSpinner();
        renderPage();
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
    std::weak_ptr<PageWidget> weakWidget = m_pageWidget;

    auto funcExecute = [weakWidget]() {
        if (auto widget = weakWidget.lock(); widget != nullptr)
            widget->renderPage();
    };

    auto funcPostExecute = [weakWidget]() {
        if (auto widget = weakWidget.lock(); widget != nullptr)
            widget->showPage();
    };

    auto task = std::make_shared<Task>(funcExecute, funcPostExecute);
    m_pageWidget->setRenderingTask(task);
    m_taskRunner.queueFront(task);
}

} // namespace Slicer
