#include "slicerview.hpp"
#include <gtkmm/image.h>

namespace Slicer {

View::View(Slicer::Document& document)
    : m_document{document}
{
    set_column_spacing(10);
    set_row_spacing(20);

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

void View::removeSelectedPages()
{
    auto children = get_selected_children();

    // FIXME
    // The following won't work with a multiple selection!
    for (Gtk::FlowBoxChild* c : children) {
        const int index = c->get_index();
        m_document.removePage(index);
        remove(*c);
    }
}

void View::removePreviousPages()
{
    auto selected = get_selected_children();

    if (selected.size() != 1)
        throw std::runtime_error(
            "Tried to remove previous pages with more "
            "than one page selected. This should never happen!");

    const int end = selected.at(0)->get_index();

    for (int i = 0; i < end; ++i) {
        m_document.removePage(0);
        Gtk::FlowBoxChild* c = get_child_at_index(0);
        remove(*c);
    }
}

void View::removeNextPages()
{
    auto selected = get_selected_children();

    if (selected.size() != 1)
        throw std::runtime_error(
            "Tried to remove next pages with more "
            "than one page selected. This should never happen!");

    const int begin = selected.at(0)->get_index();

    for (unsigned int i = begin + 1; i < get_children().size(); ++i) {
        m_document.removePage(begin + 1);
        Gtk::FlowBoxChild* c = get_child_at_index(begin + 1);
        remove(*c);
    }
}
}
