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

#include "pagewidget.hpp"

namespace Slicer {

PageWidget::PageWidget(const Glib::RefPtr<Page>& page,
                       int targetSize)
    : m_page{page}
    , m_targetSize{targetSize}
{
    const Page::Size pageSize = m_page->scaledRotatedSize(m_targetSize);
    set_size_request(pageSize.width, pageSize.height);

    m_spinner.set_size_request(38, 38);
    m_spinner.start();
    m_contentBox.pack_start(m_spinner, true, false);

    m_check.set_halign(Gtk::ALIGN_END);
    m_check.set_valign(Gtk::ALIGN_END);
    m_check.set_margin_bottom(10);
    m_check.set_margin_right(10);

    m_overlay.set_halign(Gtk::ALIGN_CENTER);
    m_overlay.set_valign(Gtk::ALIGN_CENTER);
    m_overlay.add(m_thumbnail);
    m_overlay.add_overlay(m_check);

    add(m_contentBox);

    show_all();

    m_check.signal_clicked().connect([this]() {
        // FIXME: This should be made so that the mouse event
        // passes through without handling it.
        // Using m_check.activate() doesn't work, since GtkCheckButton
        // also emits signal_clicked when activated.
        m_check.set_active(m_isChecked);
    });
}

void PageWidget::renderPage()
{
    m_thumbnail.set(m_page->renderPage(m_targetSize));
}

void PageWidget::showSpinner()
{
    if (!m_spinner.is_visible()) {
        m_spinner.show();
        m_spinner.start();
        m_contentBox.remove(m_overlay);
    }
}

void PageWidget::showPage()
{
    if (!isThumbnailVisible()) {
        m_spinner.stop();
        m_contentBox.pack_start(m_overlay, Gtk::PACK_SHRINK);
        m_overlay.show_all();
        m_spinner.hide();
    }
}

void PageWidget::setChecked(bool checked)
{
    if (m_isChecked != checked) {
        m_isChecked = checked;
        m_check.set_active(m_isChecked);
    }
}

bool PageWidget::isThumbnailVisible()
{
    return get_children().size() == 2;
}

} // namespace Slicer
