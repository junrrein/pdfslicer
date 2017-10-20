#include "view.hpp"
#include "viewchild.hpp"

namespace Slicer {

View::View(const Slicer::Document& document,
           int targetThumbnailSize)
    : m_document{document}
    , m_pageRendererPool{1} // Only one thread - Poppler doesn't handle more
{
    set_column_spacing(10);
    set_row_spacing(20);

    set_selection_mode(Gtk::SELECTION_SINGLE);
    set_activate_on_single_click(false);

    bind_list_store(m_document.pages(), [this, targetThumbnailSize](const Glib::RefPtr<Page>& page) {
        return Gtk::manage(new Slicer::ViewChild{page,
                                                 targetThumbnailSize,
                                                 m_pageRendererPool});
    });
}

View::~View()
{
    m_pageRendererPool.stop();
}

void View::waitForRenderCompletion()
{
    m_pageRendererPool.stop(true);
}
}
