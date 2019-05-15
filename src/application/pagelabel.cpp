#include "pagelabel.hpp"
#include <glibmm/i18n.h>
#include <fmt/format.h>

using namespace fmt::literals;

namespace Slicer {

PageLabel::PageLabel(const Glib::ustring& fileName, unsigned int pageNumber)
    : Gtk::Box{Gtk::ORIENTATION_HORIZONTAL}
{
    m_nameLabel.set_text(fileName);
    m_separatorLabel.set_text(" - ");
    m_pageNumberLabel.set_label(fmt::format(_("Page {pageNumber}"),
                                            "pageNumber"_a = pageNumber)); //NOLINT

    m_nameLabel.set_ellipsize(Pango::ELLIPSIZE_END);
    m_nameLabel.set_max_width_chars(10);

    pack_start(m_nameLabel);
    pack_start(m_separatorLabel, Gtk::PACK_SHRINK);
    pack_start(m_pageNumberLabel, Gtk::PACK_SHRINK);

    show_all();
}

} // namespace Slicer
