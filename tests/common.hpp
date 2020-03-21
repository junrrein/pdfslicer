#ifndef SLICER_COMMON_HPP
#define SLICER_COMMON_HPP

#include <glibmm/miscutils.h>

static const std::string multipage1Name = "multipage-1.pdf";
static const std::string multipage2Name = "multipage-2.pdf";
static const std::string multipage3Name = "multipage-3.pdf";
static const std::string multipage1Path
	= Glib::build_filename(Glib::get_current_dir(), multipage1Name);
static const std::string multipage2Path
	= Glib::build_filename(Glib::get_current_dir(), multipage2Name);
static const std::string multipage3Path
	= Glib::build_filename(Glib::get_current_dir(), multipage3Name);

#endif // COMMON_HPP
