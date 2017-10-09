#include "slicerwindow.hpp"

namespace Slicer {

Window::Window(std::string filePath)
    : m_document{filePath}
    , m_view{m_document}
{
    set_title("PDF Slicer");
    set_default_size(800, 600);

    m_scroller.add(m_view);
    add(m_scroller);

    show_all_children();
}
}
