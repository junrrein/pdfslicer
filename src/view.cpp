#include "view.hpp"
#include "viewchild.hpp"
#include <gtkmm/image.h>

namespace Slicer {

View::View(const Slicer::Document& document)
    : m_document{document}
    , m_threadPool{1}
{
    set_column_spacing(10);
    set_row_spacing(20);

    set_selection_mode(Gtk::SELECTION_SINGLE);
    set_activate_on_single_click(false);

    bind_list_store(m_document.pages(), [this](const Glib::RefPtr<GPopplerPage>& gPage) {
        auto child = Gtk::manage(new Slicer::ViewChild{gPage,
                                                       m_uiDispatcher});

        m_uiDispatcher.connect([this, child]() {
            child->showPage();
        });

        m_threadPool.enqueue([child]() {
            child->renderPage();
        });

        return child;
    });
}
}
