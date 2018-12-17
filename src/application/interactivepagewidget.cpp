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

#include "interactivepagewidget.hpp"
#include <glibmm/i18n.h>
#include <fmt/format.h>

using namespace fmt::literals;

namespace Slicer {

InteractivePageWidget::InteractivePageWidget(const Glib::RefPtr<Page>& page,
                                             int targetSize)
    : PageWidget(page, targetSize)
{
    setupInteractiveWidgets();
    setupLabel(page->number() + 1);
    setupSignalHandlers();
}

void InteractivePageWidget::setChecked(bool checked)
{
    if (m_isChecked != checked) {
        m_isChecked = checked;
        m_check.set_active(m_isChecked);
    }
}

void InteractivePageWidget::setupInteractiveWidgets()
{
    m_check.set_halign(Gtk::ALIGN_END);
    m_check.set_valign(Gtk::ALIGN_END);
    m_check.set_margin_bottom(10);
    m_check.set_margin_right(10);

    m_previewButton.set_image_from_icon_name("document-print-preview-symbolic");
    m_previewButtonRevealer.add(m_previewButton);
    m_previewButtonRevealer.set_transition_type(Gtk::REVEALER_TRANSITION_TYPE_CROSSFADE);
    m_previewButtonRevealer.set_halign(Gtk::ALIGN_END);
    m_previewButtonRevealer.set_valign(Gtk::ALIGN_START);
    m_previewButtonRevealer.set_margin_top(10);
    m_previewButtonRevealer.set_margin_right(10);

    m_overlay.add_overlay(m_check);
    m_overlay.add_overlay(m_previewButtonRevealer);
}

void InteractivePageWidget::setupLabel(int pageNumber)
{
    m_pageNumberLabel.set_label(fmt::format(_("Page {pageNumber}"), "pageNumber"_a = pageNumber));
    m_pageNumberLabel.set_margin_top(5);
    m_pageNumberLabel.set_visible();

    m_contentGrid.attach(m_pageNumberLabel, 1, 3, 1, 1);
}

void InteractivePageWidget::setupSignalHandlers()
{
    m_overlayEventBox.signal_button_release_event().connect([this](GdkEventButton* eventButton) {
        if (eventButton->button == 1) {
            if ((eventButton->state & GDK_SHIFT_MASK) != 0) {
                setChecked(true);
                shiftSelected.emit(this);
            }
            else {
                setChecked(!getChecked());
                selectedChanged.emit(this);
            }

            return true;
        }

        return false;
    });

    m_overlayEventBox.signal_enter_notify_event().connect([this](GdkEventCrossing*) {
        m_previewButtonRevealer.set_reveal_child(true);

        return false;
    });

    m_overlayEventBox.signal_leave_notify_event().connect([this](GdkEventCrossing*) {
        m_previewButtonRevealer.set_reveal_child(false);

        return false;
    });

    m_previewButton.signal_enter_notify_event().connect([this](GdkEventCrossing*) {
        m_previewButtonRevealer.set_reveal_child(true);

        return false;
    });

    m_previewButton.signal_button_release_event().connect([](GdkEventButton* eventButton) {
        // If the event was a left click, don't propagate the event further
        return eventButton->button == 1;
    });

    m_previewButton.signal_clicked().connect([this]() {
        previewRequested.emit(m_page);
    });
}

} // namespace Slicer
