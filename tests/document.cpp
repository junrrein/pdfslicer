#include <catch.hpp>

#include <command.hpp>
#include <document.hpp>
#include <giomm/file.h>
#include <glibmm/miscutils.h>

using namespace Slicer;

static const std::string multipagePdfPath
    = Glib::build_filename(Glib::get_current_dir(), "multipage.pdf");

SCENARIO("Removing a single page from different places of a document")
{
    GIVEN("A multipage document with 15 pages")
    {
        auto multipagePdfFile = Gio::File::create_for_path(multipagePdfPath);
        Document doc{multipagePdfFile};
        REQUIRE(doc.numberOfPages() == 15);

        WHEN("The first page is removed")
        {
            doc.removePage(0);

            THEN("The first page of the document should be the second page of the file")
            REQUIRE(doc.getPage(0)->number() == 1);

            THEN("The document should have 14 pages")
            REQUIRE(doc.numberOfPages() == 14);

            WHEN("The remove command is undone")
            {
                doc.undoCommand();

                THEN("The first page of the document should be the first page of the file")
                REQUIRE(doc.getPage(0)->number() == 0);

                THEN("The document should have 15 pages")
                REQUIRE(doc.numberOfPages() == 15);

                WHEN("The undone command is redone")
                {
                    doc.redoCommand();

                    THEN("The first page of the document should be the second page of the file")
                    REQUIRE(doc.getPage(0)->number() == 1);

                    THEN("The document should have 14 pages")
                    REQUIRE(doc.numberOfPages() == 14);

                    WHEN("The redone command is undone")
                    {
                        doc.undoCommand();

                        THEN("The first page of the document should be the first page of the file")
                        REQUIRE(doc.getPage(0)->number() == 0);

                        THEN("The document should have 15 pages")
                        REQUIRE(doc.numberOfPages() == 15);
                    }
                }
            }

            WHEN("The new first page is removed")
            {
                doc.removePage(0);

                THEN("The first page of the document should be the third page of the file")
                REQUIRE(doc.getPage(0)->number() == 2);

                THEN("The document should have 13 pages")
                REQUIRE(doc.numberOfPages() == 13);

                WHEN("The second remove command is undone")
                {
                    doc.undoCommand();

                    THEN("The first page of the document should be the second page of the file")
                    REQUIRE(doc.getPage(0)->number() == 1);

                    THEN("The document should have 14 pages")
                    REQUIRE(doc.numberOfPages() == 14);

                    WHEN("The first remove command is undone")
                    {
                        doc.undoCommand();

                        THEN("The first page of the document should be the first page of the file")
                        REQUIRE(doc.getPage(0)->number() == 0);

                        THEN("The document should have 15 pages")
                        REQUIRE(doc.numberOfPages() == 15);
                    }
                }
            }
        }

        WHEN("The last page is removed")
        {
            doc.removePage(14);

            THEN("The document should have 14 pages")
            REQUIRE(doc.numberOfPages() == 14);

            THEN("The 14th page of the document should be the 14th page of the file")
            REQUIRE(doc.getPage(13)->number() == 13);

            WHEN("The remove command is undone")
            {
                doc.undoCommand();

                THEN("The document should have 15 pages")
                REQUIRE(doc.numberOfPages() == 15);

                THEN("The 15th page of the document should be the 15th page of the file")
                REQUIRE(doc.getPage(14)->number() == 14);
            }
        }

        WHEN("The 8th page is removed")
        {
            doc.removePage(7);

            THEN("The document should have 14 pages")
            REQUIRE(doc.numberOfPages() == 14);

            THEN("The 7th and 8th pages of the document should be the 7th and 9th pages of the file, respectively")
            {
                REQUIRE(doc.getPage(6)->number() == 6);
                REQUIRE(doc.getPage(7)->number() == 8);
            }

            WHEN("The remove command is undone")
            {
                doc.undoCommand();

                THEN("The document should have 15 pages")
                REQUIRE(doc.numberOfPages() == 15);

                THEN("The 7th, 8th and 9th pages of the document should be the corresponding pages of the file")
                {
                    REQUIRE(doc.getPage(6)->number() == 6);
                    REQUIRE(doc.getPage(7)->number() == 7);
                    REQUIRE(doc.getPage(8)->number() == 8);
                }
            }
        }
    }
}
