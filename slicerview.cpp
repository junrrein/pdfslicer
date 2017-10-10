#include "slicerview.hpp"
#include <gtkmm/image.h>

namespace Slicer {

View::View(Slicer::Document& document)
    : m_document{document}
{
    set_column_spacing(10);
    set_row_spacing(10);

    set_selection_mode(Gtk::SELECTION_SINGLE);
    set_activate_on_single_click(false);

    const auto& pages = m_document.pages();

    for (unsigned int i = 0; i < pages.size(); ++i) {
        auto pixbuf = m_document.renderPage(i, 160, 200);
        auto image = Gtk::manage(new Gtk::Image);
        image->set(pixbuf);

        add(*image);
    }

    signal_child_activated().connect([this](Gtk::FlowBoxChild* child) {
        // Render a big preview and create the preview window
        const int pageIndex = child->get_index();
        auto pixbuf = m_document.renderPage(pageIndex, 640, 800);
        m_previewWindow = std::make_unique<Slicer::PreviewWindow>(pixbuf);

        // Make the preview window modal
        m_previewWindow->set_modal();
        Gtk::Window* parent = dynamic_cast<Gtk::Window*>(this->get_toplevel());
        m_previewWindow->set_transient_for(*parent);

        m_previewWindow->show();
    });
}
}
