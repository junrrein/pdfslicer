#include "common.hpp"
#include <catch.hpp>
#include <document.hpp>

using namespace Slicer;

SCENARIO("Adding 2 new files to an existing document")
{
    GIVEN("A multipage PDF document with 15 pages")
    {
        auto multipagePdfFile = Gio::File::create_for_path(multipage1Path);
        Document doc{multipagePdfFile};
        REQUIRE(doc.numberOfPages() == 15);

        const std::vector<Glib::RefPtr<Gio::File>> filesToAdd = {
            Gio::File::create_for_path(multipage2Path),
            Gio::File::create_for_path(multipage3Path)};

        WHEN("Two PDF files (5 and 15 pages) are added to the beginning")
        {
            doc.addFiles(filesToAdd, 0);

            THEN("The document should now have 35 pages")
            REQUIRE(doc.numberOfPages() == 35);

            THEN("The first page of the document should be the first page of the second file")
            {
                REQUIRE(doc.getPage(0)->indexInFile() == 0);
                REQUIRE(doc.getPage(0)->fileName() == multipage2Name);
            }

            THEN("The 5th page of the document should be the last page of the second file")
            {
                REQUIRE(doc.getPage(4)->indexInFile() == 4);
                REQUIRE(doc.getPage(4)->fileName() == multipage2Name);
            }

            THEN("The 6th page of the document should be the first page of the third file")
            {
                REQUIRE(doc.getPage(5)->indexInFile() == 0);
                REQUIRE(doc.getPage(5)->fileName() == multipage3Name);
            }

            THEN("The 20th page of the document should be the last page of the third file")
            {
                REQUIRE(doc.getPage(19)->indexInFile() == 14);
                REQUIRE(doc.getPage(19)->fileName() == multipage3Name);
            }

            THEN("The 21st page of the document should be the first page of the original file")
            {
                REQUIRE(doc.getPage(20)->indexInFile() == 0);
                REQUIRE(doc.getPage(20)->fileName() == multipage1Name);
            }

            THEN("The last page of the document should be the last page of the original file")
            {
                REQUIRE(doc.getPage(34)->indexInFile() == 14);
                REQUIRE(doc.getPage(34)->fileName() == multipage1Name);
            }
        }
    }
}
