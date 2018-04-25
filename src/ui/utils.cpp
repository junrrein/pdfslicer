#include "utils.hpp"
#include <glibmm/i18n.h>

Glib::RefPtr<Gtk::FileFilter> Slicer::pdfFilter()
{
	auto filter = Gtk::FileFilter::create();
    filter->set_name(_("PDF document"));
	filter->add_mime_type("application/pdf");

	return filter;
}
