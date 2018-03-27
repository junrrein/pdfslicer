#include "utils.hpp"

Glib::RefPtr<Gtk::FileFilter> Slicer::pdfFilter()
{
	auto filter = Gtk::FileFilter::create();
	filter->set_name("PDF documents");
	filter->add_mime_type("application/pdf");

	return filter;
}
