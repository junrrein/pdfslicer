#ifndef TEMPFILE_HPP
#define TEMPFILE_HPP

#include <giomm/file.h>

namespace Slicer {

Glib::RefPtr<Gio::File> generateTempFile();
}

#endif // TEMPFILE_HPP
