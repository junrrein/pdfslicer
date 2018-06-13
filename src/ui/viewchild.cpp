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

#include "viewchild.hpp"

namespace Slicer {

ViewChild::ViewChild(const Glib::RefPtr<Page>& page,
                     int targetSize)
    : m_page{page}
    , m_targetSize{targetSize}
{
    const Page::Size pageSize = m_page->scaledRotatedSize(m_targetSize);
    set_size_request(pageSize.width, pageSize.height);

    m_spinner.set_size_request(38, 38);
    m_spinner.start();
    pack_start(m_spinner, true, false);

    show_all();
}

void ViewChild::renderPage()
{
    Glib::RefPtr<Gdk::Pixbuf> pixbuf = m_page->renderPage(m_targetSize);
    m_thumbnail.set(pixbuf);
}

void ViewChild::showSpinner()
{
    if (!m_spinner.is_visible()) {
        m_spinner.show();
        m_spinner.start();
        remove(m_thumbnail);
    }
}

void ViewChild::showPage()
{
    if (!isThumbnailVisible()) {
        m_spinner.stop();
        pack_start(m_thumbnail);
        m_thumbnail.show();
        m_spinner.hide();
    }
}

bool ViewChild::isThumbnailVisible()
{
    return get_children().size() == 2;
}

} // namespace Slicer
