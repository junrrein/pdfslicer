#include "editor.hpp"

namespace Slicer {

Editor::Editor(Gio::ActionMap& actionMap)
    : Gtk::Box{Gtk::ORIENTATION_VERTICAL}
    , m_view{actionMap}
{
    m_scroller.add(m_view);
    pack_start(m_scroller);
    pack_start(m_actionBar, Gtk::PACK_SHRINK);
}

void Editor::setDocument(Document& document)
{
    m_view.setDocument(document);
}

void Editor::waitForRenderCompletion()
{
    m_view.waitForRenderCompletion();
}

} // namespace Slicer
