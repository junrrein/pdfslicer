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

    bind_list_store(m_document.pages(), [this](const Glib::RefPtr<GPopplerPage>& gPage) {
        auto pixbuf = m_document.renderPage(gPage->page, 200);
        auto image = Gtk::manage(new Gtk::Image);
        image->set(pixbuf);

        return image;
    });
}
}
