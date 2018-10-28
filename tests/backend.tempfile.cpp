#include <catch.hpp>
#include <tempfile.hpp>
#include <config.hpp>
#include <gtkmm/main.h>
#include <uuid.h>

using namespace Slicer;

SCENARIO("Generated temporary files should be unique and isolated from the system")
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

            THEN("The name of the temporary file should be a valid UUID")
            {
                REQUIRE_NOTHROW(uuids::uuid::from_string(tempFile->get_basename()));
            }

            THEN("The parent directory should be our application's ID")
            {
                REQUIRE(tempFile->get_parent()->get_basename() == config::APPLICATION_ID);
            }
        }
    }
}
