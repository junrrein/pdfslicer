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

#include "../backend/page.hpp"
#include <ctpl_stl.h>
#include <gtkmm/box.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/eventbox.h>
#include <gtkmm/flowboxchild.h>
#include <gtkmm/image.h>
#include <gtkmm/overlay.h>
#include <gtkmm/revealer.h>
#include <gtkmm/spinner.h>

namespace Slicer {

class PageWidget : public Gtk::FlowBoxChild {
public:
    PageWidget(const Glib::RefPtr<Page>& page,
               int targetSize,
               bool isInteractive = true);
    virtual ~PageWidget() = default;

    void changeSize(int targetSize);
    void renderPage();
    void showSpinner();
    void showPage();
    void setChecked(bool checked);
    bool getChecked() const { return m_isChecked; }

    sigc::signal<void, PageWidget*> selectedChanged;
    sigc::signal<void, PageWidget*> shiftSelected;
    sigc::signal<void, const Glib::RefPtr<Page>&> previewRequested;

private:
    const Glib::RefPtr<Page> m_page;
    int m_targetSize;
    const bool m_isInteractive;
    bool m_isChecked = false;

    Gtk::Box m_contentBox;
    Gtk::Spinner m_spinner;

    Gtk::EventBox m_overlayEventBox;
    Gtk::Overlay m_overlay;
    Gtk::Image m_thumbnail;
    Gtk::CheckButton m_check;
    Gtk::Button m_previewButton;
    Gtk::Revealer m_previewButtonRevealer;

    void setupWidgets();
    bool isThumbnailVisible();
    void setupSignalHandlers();
};

} // namespace Slicer

#endif // VIEWCHILD_HPP
