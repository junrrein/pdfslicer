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

#include "page.hpp"
#include <cmath>

namespace Slicer {

Page::Page(PopplerDocument* document, int pageNumber)
    : m_ppage{nullptr, &g_object_unref}
{
    PopplerPage* ppage = poppler_document_get_page(document, pageNumber);

    if (ppage == nullptr)
        throw std::runtime_error("Couldn't load page with number: " + std::to_string(pageNumber));

    m_ppage.reset(ppage);
}

int Page::number() const
{
    return poppler_page_get_index(m_ppage.get());
}

Page::Size Page::size() const
{
    double width = 0, height = 0;
    poppler_page_get_size(m_ppage.get(), &width, &height);

    return {static_cast<int>(width), static_cast<int>(height)};
}

Page::Size Page::rotatedSize() const
{
    Size size = this->size();

    if (std::abs((m_rotation / 90) % 2) != 0)
        std::swap(size.width, size.height);

    return size;
}

Page::Size scaleSize(Page::Size sourceSize, int targetSize)
{
    Page::Size scaledSize{};

    if (sourceSize.height > sourceSize.width) {
        scaledSize.height = targetSize;
        const double ratioFactor = static_cast<double>(sourceSize.width) / sourceSize.height;
        scaledSize.width = static_cast<int>(floor(targetSize * ratioFactor));
    }
    else {
        scaledSize.width = targetSize;
        const double ratioFactor = static_cast<double>(sourceSize.height) / sourceSize.width;
        scaledSize.height = static_cast<int>(floor(targetSize * ratioFactor));
    }

    return scaledSize;
}

Page::Size Page::scaledSize(int targetSize) const
{
    return scaleSize(size(), targetSize);
}

Page::Size Page::scaledRotatedSize(int targetSize) const
{
    return scaleSize(rotatedSize(), targetSize);
}

void Page::rotateRight()
{
    if (m_rotation == 270)
        m_rotation = 0;
    else
        m_rotation += 90;
}

void Page::rotateLeft()
{
    if (m_rotation == 0)
        m_rotation = 270;
    else
        m_rotation -= 90;
}

int pageComparator::operator()(const Glib::RefPtr<const Page>& a,
                               const Glib::RefPtr<const Page>& b)
{
    const int aPosition = a->number();
    const int bPosition = b->number();

    if (aPosition < bPosition)
        return -1;

    if (aPosition == bPosition)
        return 0;

    return 1;
}

} // namespace Slicer
