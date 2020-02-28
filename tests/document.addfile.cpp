#include <catch.hpp>

#include <document.hpp>
#include <giomm/file.h>
#include <glibmm/miscutils.h>

using namespace Slicer;

static const std::string multipage1Name = "multipage-1.pdf";
static const std::string multipage2Name = "multipage-2.pdf";
static const std::string multipage1PdfPath
    = Glib::build_filename(Glib::get_current_dir(), multipage1Name);
static const std::string multipage2PdfPath
    = Glib::build_filename(Glib::get_current_dir(), multipage2Name);

SCENARIO("Adding a new file to an existing document")
{
    GIVEN("A multipage PDF document with 15 pages")
    {
        auto multipagePdfFile = Gio::File::create_for_path(multipage1PdfPath);
        Document doc{multipagePdfFile};
        REQUIRE(doc.numberOfPages() == 15);

        WHEN("A 5-page PDF file is added to the beggining")
        {
            doc.addFile(Gio::File::create_for_path(multipage2PdfPath), 0);

            THEN("The document should now have 20 pages")
            REQUIRE(doc.numberOfPages() == 20);

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

            THEN("The 6th page of the document should be the first page of the first file")
            {
                REQUIRE(doc.getPage(5)->indexInFile() == 0);
                REQUIRE(doc.getPage(5)->fileName() == multipage1Name);
            }

            THEN("The last page of the document should be the last page of the first file")
            {
                REQUIRE(doc.getPage(19)->indexInFile() == 14);
                REQUIRE(doc.getPage(19)->fileName() == multipage1Name);
            }
        }

        WHEN("A 5-page PDF file is added to the end")
        {
            doc.addFile(Gio::File::create_for_path(multipage2PdfPath), doc.numberOfPages());

            THEN("The document should now have 20 pages")
            REQUIRE(doc.numberOfPages() == 20);

            THEN("The first page of the document should be the first page of the first file")
            {
                REQUIRE(doc.getPage(0)->indexInFile() == 0);
                REQUIRE(doc.getPage(0)->fileName() == multipage1Name);
            }

            THEN("The 15th page of the document should be the last page of the first file")
            {
                REQUIRE(doc.getPage(14)->indexInFile() == 14);
                REQUIRE(doc.getPage(14)->fileName() == multipage1Name);
            }

            THEN("The 16th page of the document should be the first page of the second file")
            {
                REQUIRE(doc.getPage(15)->indexInFile() == 0);
                REQUIRE(doc.getPage(15)->fileName() == multipage2Name);
            }

            THEN("The last page of the document should be the last page of the second file")
            {
                REQUIRE(doc.getPage(19)->indexInFile() == 4);
                REQUIRE(doc.getPage(19)->fileName() == multipage2Name);
            }
        }

        WHEN("A 5-page PDF file is added at the 5th page of the document")
        {
            doc.addFile(Gio::File::create_for_path(multipage2PdfPath), 4);

            THEN("The document should now have 20 pages")
            REQUIRE(doc.numberOfPages() == 20);

            THEN("The 4th page of the document should be the 4th page of the first file")
            {
                REQUIRE(doc.getPage(3)->indexInFile() == 3);
                REQUIRE(doc.getPage(3)->fileName() == multipage1Name);
            }

            THEN("The 5th page of the document should be the 1st page of the second file")
            {
                REQUIRE(doc.getPage(4)->indexInFile() == 0);
                REQUIRE(doc.getPage(4)->fileName() == multipage2Name);
            }

            THEN("The 9th page of the document should be the 5th page of the second file")
            {
                REQUIRE(doc.getPage(8)->indexInFile() == 4);
                REQUIRE(doc.getPage(8)->fileName() == multipage2Name);
            }

            THEN("The 10th page of the document should be the 5th page of the first file")
            {
                REQUIRE(doc.getPage(9)->indexInFile() == 4);
                REQUIRE(doc.getPage(9)->fileName() == multipage1Name);
            }
        }
    }
}
