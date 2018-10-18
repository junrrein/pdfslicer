#ifndef PAGERENDERER_HPP
#define PAGERENDERER_HPP

#include "page.hpp"

namespace Slicer {

class PageRenderer {
public:
    PageRenderer(const Page& page);

    Glib::RefPtr<Gdk::Pixbuf> render(int targetSize) const;

private:
    const Page& m_page;
};

} // namespace Slicer

#endif // PAGERENDERER_HPP
