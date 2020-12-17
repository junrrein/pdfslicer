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

#ifndef VIEWCHILD_HPP
#define VIEWCHILD_HPP

#include "task.hpp"
#include <page.hpp>
#include <gtkmm/box.h>
#include <gtkmm/image.h>
#include <gtkmm/spinner.h>

namespace Slicer {

class PageWidget : public Gtk::Box {
public:
    PageWidget(const Glib::RefPtr<const Page>& page,
               int targetSize);

    PageWidget(const PageWidget&) = delete;
    PageWidget& operator=(const PageWidget&) = delete;
    PageWidget(PageWidget&&) = delete;
    PageWidget& operator=(PageWidget&& src) = delete;

    ~PageWidget() override = default;

    void changeSize(int targetSize);
    void renderPage();
    void setImage(const Glib::RefPtr<Gdk::Pixbuf>& image);
    void showSpinner();
    void showPage();
    void setRenderingTask(const std::weak_ptr<Task>& task);
    void cancelRendering();

    const Glib::RefPtr<const Page>& page() const;

private:
    Glib::RefPtr<const Page> m_page;
    int m_targetSize;
    std::weak_ptr<Task> m_renderingTask;

    Gtk::Spinner m_spinner;
    Gtk::Image m_thumbnail;

    void setupWidgets();
    bool isThumbnailVisible();
};

} // namespace Slicer

#endif // VIEWCHILD_HPP
