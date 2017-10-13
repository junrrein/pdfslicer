#include "previewwindow.hpp"

namespace Slicer {

PreviewWindow::PreviewWindow(Glib::RefPtr<Gdk::Pixbuf> previewPixbuf)
{
    set_title("Preview");
    set_default_size(700, 600);
    m_previewImage.set(previewPixbuf);

    m_scroller.add(m_previewImage);
    add(m_scroller);

    show_all_children();
}

} // namespace Slicer
