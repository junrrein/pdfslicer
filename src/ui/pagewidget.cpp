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
    pack_start(m_spinner, true, false);

    m_check.set_halign(Gtk::ALIGN_END);
    m_check.set_valign(Gtk::ALIGN_END);
    m_check.set_margin_bottom(10);
    m_check.set_margin_right(10);
    renderCheck(CheckState::unchecked);

    m_overlay.set_halign(Gtk::ALIGN_CENTER);
    m_overlay.set_valign(Gtk::ALIGN_CENTER);
    m_overlay.add(m_thumbnail);
    m_overlay.add_overlay(m_check);

    show_all();
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
        remove(m_overlay);
    }
}

void PageWidget::showPage()
{
    if (!isThumbnailVisible()) {
        m_spinner.stop();
        pack_start(m_overlay, Gtk::PACK_SHRINK);
        m_overlay.show_all();
        m_spinner.hide();
    }
}

bool PageWidget::isThumbnailVisible()
{
    return get_children().size() == 2;
}

void PageWidget::renderCheck(CheckState checkState)
{
    const int checkSize = 36;

    Cairo::RefPtr<Cairo::ImageSurface> surface = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32,
                                                                             checkSize,
                                                                             checkSize);
    Cairo::RefPtr<Cairo::Context> cr = Cairo::Context::create(surface);
    Glib::RefPtr<Gtk::StyleContext> styleContext = get_style_context();

    styleContext->context_save();
    styleContext->add_class(GTK_STYLE_CLASS_CHECK);

    if (checkState == CheckState::checked)
        styleContext->set_state(Gtk::STATE_FLAG_CHECKED);

    styleContext->render_check(cr, 0, 0, checkSize, checkSize);
    styleContext->context_restore();

    m_check.set(surface);
}

} // namespace Slicer
