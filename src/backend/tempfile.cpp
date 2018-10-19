#include "tempfile.hpp"
#include <glibmm/miscutils.h>
#include <uuid.h>

namespace Slicer {

Glib::RefPtr<Gio::File> generateTempFile()
{
    const std::string path = Glib::build_filename(Glib::get_tmp_dir(),
                                                  uuids::to_string(uuids::uuid_system_generator{}()));

    return Gio::File::create_for_path(path);
}
}
