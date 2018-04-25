#include "application.hpp"
#include <glibmm/i18n.h>
#include <config.hpp>

int main(int num_args, char* args_array[])
{
    bindtextdomain(Slicer::config::GTXT_PACKAGE.c_str(),
                   Slicer::config::GNOME_LOCALE_DIR.c_str());
    bind_textdomain_codeset(Slicer::config::GTXT_PACKAGE.c_str(),
                            "UTF-8");
    textdomain(Slicer::config::GTXT_PACKAGE.c_str());

    auto app = Slicer::Application::create();

    return app->run(num_args, args_array);
}
