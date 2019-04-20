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

InteractivePageWidget::InteractivePageWidget(const Glib::RefPtr<const Page>& page,
                                             int targetSize)
    : m_pageWidget(page, targetSize)
{
    setupWidgets();
    setupSignalHandlers();
}

unsigned int InteractivePageWidget::documentIndex() const
{
    return page()->getDocumentIndex();
}

void InteractivePageWidget::setChecked(bool checked)
{
    if (m_isChecked != checked) {
        m_isChecked = checked;
        m_check.set_active(m_isChecked);
    }
}

int InteractivePageWidget::sortFunction(const InteractivePageWidget& a,
                                        const InteractivePageWidget& b)
{
    return Page::sortFunction(a.page(), b.page());
}

void InteractivePageWidget::changeSize(int targetSize)
{
    m_pageWidget.changeSize(targetSize);
}

void InteractivePageWidget::renderPage()
{
    m_pageWidget.renderPage();
}

void InteractivePageWidget::showSpinner()
{
    m_pageWidget.showSpinner();
}

void InteractivePageWidget::showPage()
{
    m_pageWidget.showPage();
}

const Glib::RefPtr<const Page> InteractivePageWidget::page() const
{
    return m_pageWidget.page();
}

void InteractivePageWidget::setupWidgets()
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

    m_overlay.set_halign(Gtk::ALIGN_CENTER);
    m_overlay.set_valign(Gtk::ALIGN_CENTER);
    m_overlay.add_overlay(m_check);
    m_overlay.add_overlay(m_previewButtonRevealer);
    m_overlay.add(m_pageWidget);
    m_overlayEventBox.add(m_overlay);

    m_pageNumberLabel.set_label(fmt::format(_("Page {pageNumber}"),
                                            "pageNumber"_a = page()->fileIndex() + 1));
    m_pageNumberLabel.set_margin_top(5);
    m_pageNumberLabel.set_visible();

    m_contentBox.set_orientation(Gtk::ORIENTATION_VERTICAL);
    m_contentBox.pack_start(m_overlayEventBox);
    m_contentBox.pack_start(m_pageNumberLabel, false, true);
    add(m_contentBox);

    show_all();
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
        previewRequested.emit(page());
    });
}

} // namespace Slicer
