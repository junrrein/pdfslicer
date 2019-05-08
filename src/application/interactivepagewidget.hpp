// PDF Slicer
// Copyright (C) 2018 Julián Unrrein

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

#ifndef INTERACTIVEPAGEWIDGET_HPP
#define INTERACTIVEPAGEWIDGET_HPP

#include "pagelabel.hpp"
#include "pagewidget.hpp"
#include <gtkmm/checkbutton.h>
#include <gtkmm/eventbox.h>
#include <gtkmm/flowboxchild.h>
#include <gtkmm/label.h>
#include <gtkmm/overlay.h>
#include <gtkmm/revealer.h>
#include <atomic>

namespace Slicer {

class InteractivePageWidget : public Gtk::FlowBoxChild {

public:
    InteractivePageWidget(const Glib::RefPtr<const Page>& page,
                          int targetSize);
    virtual ~InteractivePageWidget() = default;

    unsigned int documentIndex() const;

    void setChecked(bool checked);
    bool getChecked() const { return m_isChecked; }

    void enableRendering() { m_isRenderingCancelled = false; }
    void cancelRendering() { m_isRenderingCancelled = true; }
    bool isRenderingCancelled() const { return m_isRenderingCancelled; }

    sigc::signal<void, InteractivePageWidget*> selectedChanged;
    sigc::signal<void, InteractivePageWidget*> shiftSelected;
    sigc::signal<void, Glib::RefPtr<const Page>> previewRequested;

    static int sortFunction(const InteractivePageWidget& a,
                            const InteractivePageWidget& b);

    // Interface of Slicer::PageWidget
    void changeSize(int targetSize);
    void renderPage();
    void showSpinner();
    void showPage();

private:
    bool m_isChecked = false;
    std::atomic<bool> m_isRenderingCancelled = false;

    Gtk::Box m_contentBox;
    Gtk::Overlay m_overlay;
    Gtk::EventBox m_overlayEventBox;
    Slicer::PageWidget m_pageWidget;

    Gtk::Button m_previewButton;
    Gtk::Revealer m_previewButtonRevealer;
    PageLabel m_pageLabel;

    const Glib::RefPtr<const Page>& page() const;
    void setupWidgets();
    void setupSignalHandlers();
};

} // namespace Slicer

#endif // INTERACTIVEPAGEWIDGET_HPP
