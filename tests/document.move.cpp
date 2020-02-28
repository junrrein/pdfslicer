#include <catch.hpp>

#include <document.hpp>
#include <giomm/file.h>
#include <glibmm/miscutils.h>

using namespace Slicer;

static const std::string multipage1Name = "multipage-1";
static const std::string multipage2Name = "multipage-2";
static const std::string multipage1PdfPath
    = Glib::build_filename(Glib::get_current_dir(), multipage1Name + ".pdf");
static const std::string multipage2PdfPath
    = Glib::build_filename(Glib::get_current_dir(), multipage2Name + ".pdf");

SCENARIO("Moving 1 page across different places of a document")
{
    GIVEN("A multipage PDF document with 15 pages")
    {
        auto multipagePdfFile = Gio::File::create_for_path(multipage1PdfPath);
        Document doc{multipagePdfFile};
        REQUIRE(doc.numberOfPages() == 15);

        WHEN("The first page is moved to the 8th place")
        {
            doc.movePage(0, 7);

            THEN("The 8th page of the document should be the 1st page of the file")
            REQUIRE(doc.getPage(7)->indexInFile() == 0);

            THEN("The 7th page of the document should be the 8th page of the file")
            REQUIRE(doc.getPage(6)->indexInFile() == 7);

            THEN("The 9th page of the document should be the 9th page of the file")
            REQUIRE(doc.getPage(8)->indexInFile() == 8);

            THEN("The first page of the document should be the second page of the file")
            REQUIRE(doc.getPage(0)->indexInFile() == 1);
        }

        WHEN("The last page is moved to the 8th place")
        {
            doc.movePage(14, 7);

            THEN("The 8th page of the document should be the 15th page of the file")
            REQUIRE(doc.getPage(7)->indexInFile() == 14);

            THEN("The 9th page of the document should be the 8th page of the file")
            REQUIRE(doc.getPage(8)->indexInFile() == 7);

            THEN("The last page of the document should be the 14th page of the file")
            REQUIRE(doc.getPage(14)->indexInFile() == 13);
        }

        WHEN("The 8th page is moved to the 1st place")
        {
            doc.movePage(7, 0);

            THEN("The first page of the document should be the 8th page of the file")
            REQUIRE(doc.getPage(0)->indexInFile() == 7);

            THEN("The second page of the document should be the first page of the file")
            REQUIRE(doc.getPage(1)->indexInFile() == 0);

            THEN("The 8th page of the document should be the 7th page of the file")
            REQUIRE(doc.getPage(7)->indexInFile() == 6);

            THEN("The 9th page of the document should be the 9th page of the file")
            REQUIRE(doc.getPage(8)->indexInFile() == 8);
        }

        WHEN("The 8th page is moved to the last place")
        {
            doc.movePage(7, 14);

            THEN("The 15th page of the document should be the 8th page of the file")
            REQUIRE(doc.getPage(14)->indexInFile() == 7);

            THEN("The 14th page of the document should be the last page of the file")
            REQUIRE(doc.getPage(13)->indexInFile() == 14);

            THEN("The 8th page of the document should be the 9th page of the file")
            REQUIRE(doc.getPage(7)->indexInFile() == 8);
        }
    }
}

SCENARIO("Moving 2 adjacent pages across different places of a document")
{
    GIVEN("A multipage PDF document with 15 pages")
    {
        auto multipagePdfFile = Gio::File::create_for_path(multipage1PdfPath);
        Document doc{multipagePdfFile};
        REQUIRE(doc.numberOfPages() == 15);

        WHEN("The first 2 pages are moved to the 8th place")
        {
            doc.movePageRange(0, 1, 7);

            THEN("The 8th and 9th pages of the document should be the 1st and 2nd pages of the file, respectively")
            {
                REQUIRE(doc.getPage(7)->indexInFile() == 0);
                REQUIRE(doc.getPage(8)->indexInFile() == 1);
            }

            THEN("The 1st page of the document should be the 3rd page of the file")
            REQUIRE(doc.getPage(0)->indexInFile() == 2);

            THEN("The 6th and 7th pages of the document should be the 8th and 9th pages of the file, respectively")
            {
                REQUIRE(doc.getPage(5)->indexInFile() == 7);
                REQUIRE(doc.getPage(6)->indexInFile() == 8);
            }

            THEN("The 10th page of the document should be the 10th page of the file")
            REQUIRE(doc.getPage(9)->indexInFile() == 9);
        }

        WHEN("The first 2 pages are moved to the end of the document")
        {
            doc.movePageRange(0, 1, 13);

            THEN("The 14th and 15th pages of the document should be the 1st and 2nd pages of the file, respectively")
            {
                REQUIRE(doc.getPage(13)->indexInFile() == 0);
                REQUIRE(doc.getPage(14)->indexInFile() == 1);
            }

            THEN("The 1st page of the document should be the 3rd page of the file")
            REQUIRE(doc.getPage(0)->indexInFile() == 2);

            THEN("The 13th page of the document should be the 15th page of the file")
            REQUIRE(doc.getPage(12)->indexInFile() == 14);
        }

        WHEN("The last 2 pages are moved to the 8th place")
        {
            doc.movePageRange(13, 14, 7);

            THEN("The 8th and 9th pages of the document should be the 14th and 15th pages of the file, respectively")
            {
                REQUIRE(doc.getPage(7)->indexInFile() == 13);
                REQUIRE(doc.getPage(8)->indexInFile() == 14);
            }

            THEN("The last page of the document should be the 13th page of the file")
            REQUIRE(doc.getPage(14)->indexInFile() == 12);

            THEN("The 10th page of the document should be the 8th page of the file")
            REQUIRE(doc.getPage(9)->indexInFile() == 7);
        }

        WHEN("The last 2 pages are moved to the beggining")
        {
            doc.movePageRange(13, 14, 0);

            THEN("The 1st and 2nd pages of the document should be the 14th and 15th pages of the file")
            {
                REQUIRE(doc.getPage(0)->indexInFile() == 13);
                REQUIRE(doc.getPage(1)->indexInFile() == 14);
            }

            THEN("The last page of the document should be the 13th page of the file")
            REQUIRE(doc.getPage(14)->indexInFile() == 12);

            THEN("The 3rd page of the document should be the 1st page of the file")
            REQUIRE(doc.getPage(2)->indexInFile() == 0);
        }

        WHEN("The first 2 pages are moved to the 2nd and 3rd places in the document")
        {
            doc.movePageRange(0, 1, 1);

            THEN("The 2nd and 3rd pages of the document should be the 1st and 2nd pages of the file, respectively")
            {
                REQUIRE(doc.getPage(1)->indexInFile() == 0);
                REQUIRE(doc.getPage(2)->indexInFile() == 1);
            }

            THEN("The 1st page of the document should be the 3rd page of the file")
            REQUIRE(doc.getPage(0)->indexInFile() == 2);

            THEN("The 4rd page of the document should be the 4rd page of the file")
            REQUIRE(doc.getPage(3)->indexInFile() == 3);
        }
    }
}
