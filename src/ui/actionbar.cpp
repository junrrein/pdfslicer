#include "actionbar.hpp"
#include <gtkmm/revealer.h>

namespace Slicer {

ActionBar::ActionBar()
{
    m_buttonRemovePages.set_label("Remove selected");
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonRemovePages.gobj()), "win.remove-selected"); // NOLINT
    pack_start(m_buttonRemovePages);

    m_buttonRemovePrevious.set_label("Remove previous");
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonRemovePrevious.gobj()), "win.remove-previous"); // NOLINT
    pack_start(m_buttonRemovePrevious);

    m_buttonRemoveNext.set_label("Remove next");
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonRemoveNext.gobj()), "win.remove-next"); // NOLINT
    pack_start(m_buttonRemoveNext);

    m_buttonCancelSelection.set_label("Cancel selection");
    gtk_actionable_set_action_name(GTK_ACTIONABLE(m_buttonCancelSelection.gobj()), "win.cancel-selection"); // NOLINT
    pack_end(m_buttonCancelSelection);

    // The following is to work around a bug with themes other than Adwaita.
    // Without this, the ActionBar is too narrow and the buttons don't fit.
    set_size_request(-1, 45);
}

} // namespace Slicer
