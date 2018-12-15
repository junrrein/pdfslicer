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

#include "pagewidget.hpp"
#include <gtkmm/checkbutton.h>
#include <gtkmm/revealer.h>

namespace Slicer {

class InteractivePageWidget : public Slicer::PageWidget {

public:
    InteractivePageWidget(const Glib::RefPtr<Page>& page,
                          int targetSize);
    virtual ~InteractivePageWidget() = default;

    void setChecked(bool checked);
    bool getChecked() const { return m_isChecked; }

    sigc::signal<void, InteractivePageWidget*> selectedChanged;
    sigc::signal<void, InteractivePageWidget*> shiftSelected;
    sigc::signal<void, const Glib::RefPtr<Page>&> previewRequested;

private:
    bool m_isChecked = false;

    Gtk::CheckButton m_check;
    Gtk::Button m_previewButton;
    Gtk::Revealer m_previewButtonRevealer;

    void setupInteractiveWidgets();
    void setupSignalHandlers();
};

} // namespace Slicer

#endif // INTERACTIVEPAGEWIDGET_HPP
