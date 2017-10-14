#include "view.hpp"
#include "viewchild.hpp"
#include <gtkmm/image.h>

namespace Slicer {

View::View(const Slicer::Document& document,
           int targetThumbnailSize)
    : m_document{document}
    , m_pageRendererPool{1}
{
    set_column_spacing(10);
    set_row_spacing(20);

    set_selection_mode(Gtk::SELECTION_SINGLE);
    set_activate_on_single_click(false);

    bind_list_store(m_document.pages(), [=](const Glib::RefPtr<GPopplerPage>& gPage) {
        auto child = Gtk::manage(new Slicer::ViewChild{gPage, targetThumbnailSize});

        // We are going to render the pages on another thread,
        // so the UI stays responsive, but updating the view
        // (i.e. touching any widget at all) has to be done on
        // the main thread, or strange crashes will occur!

        // The dispatcher runs the view updates on the main thread
        m_viewUpdater.connect([=]() {
            child->showPage();
        });

        // The thread pool renders the pages on another thread
        // and signals the dispatcher when it finishes rendering.
        m_pageRendererPool.enqueue([=]() {
            child->renderPage();
            m_viewUpdater.emit();
        });

        return child;
    });
}
}
