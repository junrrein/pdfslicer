#define CATCH_CONFIG_RUNNER

#include <catch.hpp>
#include <config.hpp>
#include <gtkmm/main.h>

int main(int argc, char* argv[])
{
    Gtk::Main::init_gtkmm_internals();
    Slicer::config::createSlicerDirsIfNotExistent();

    int result = Catch::Session().run(argc, argv);

    return result;
}
