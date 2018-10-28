#include <catch.hpp>
#include <tempfile.hpp>
#include <config.hpp>
#include <gtkmm/main.h>

using namespace Slicer;

SCENARIO("Temporary file names shouldn't already exist in the filesystem")
{
    GIVEN("A normal application session")
    {
        Gtk::Main::init_gtkmm_internals();

        WHEN("A temporary file name is generated")
        {
            Glib::RefPtr<Gio::File> tempFile = TempFile::generate();

            THEN("A file with that name in that path souldn't already exist")
            {
                REQUIRE(!tempFile->query_exists());
            }
        }
    }
}

SCENARIO("Temporary file names should be contained in an application-specific directory")
{
    GIVEN("A normal application session")
    {
        Gtk::Main::init_gtkmm_internals();

        WHEN("A temporary file name is generated")
        {
            Glib::RefPtr<Gio::File> tempFile = TempFile::generate();

            THEN("The parent directory should be our application's ID")
            {
                REQUIRE(tempFile->get_parent()->get_basename() == config::APPLICATION_ID);
            }
        }
    }
}
