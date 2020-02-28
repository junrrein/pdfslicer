#include "common.hpp"
#include <catch.hpp>
#include <command.hpp>

using namespace Slicer;

SCENARIO("Adding a new file to an existing document, using the Command abstraction")
{
    GIVEN("A multipage PDF document with 15 pages")
    {
        auto multipagePdfFile = Gio::File::create_for_path(multipage1Path);
        Document doc{multipagePdfFile};
        REQUIRE(doc.numberOfPages() == 15);

        WHEN("A 5-page PDF file is added to the beggining")
        {
            AddFileCommand command{doc, Gio::File::create_for_path(multipage2Path), 0};
            command.execute();

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

            WHEN("The command is undone")
            {
                command.undo();

                THEN("The document should now have 15 pages")
                REQUIRE(doc.numberOfPages() == 15);

                THEN("The 1st page of the document should be the 1st page of the first file")
                {
                    REQUIRE(doc.getPage(0)->indexInFile() == 0);
                    REQUIRE(doc.getPage(0)->fileName() == multipage1Name);
                }
            }
        }

        WHEN("A 5-page PDF file is added to the end")
        {
            AddFileCommand command{doc, Gio::File::create_for_path(multipage2Path), doc.numberOfPages()};
            command.execute();

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

            WHEN("The commmand is undone")
            {
                command.undo();

                THEN("The document should now have 15 pages")
                REQUIRE(doc.numberOfPages() == 15);

                THEN("The last page of the document should be the last page of the first file")
                {
                    REQUIRE(doc.getPage(14)->indexInFile() == 14);
                    REQUIRE(doc.getPage(14)->fileName() == multipage1Name);
                }
            }
        }

        WHEN("A 5-page PDF file is added at the 5th page of the document")
        {
            AddFileCommand command{doc, Gio::File::create_for_path(multipage2Path), 4};
            command.execute();

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

            WHEN("The command is undone")
            {
                command.undo();

                THEN("The document should now have 15 pages")
                REQUIRE(doc.numberOfPages() == 15);

                THEN("The 5th and 6th pages of the document should be the 5th and 6th pages of the first file")
                {
                    REQUIRE(doc.getPage(4)->indexInFile() == 4);
                    REQUIRE(doc.getPage(4)->fileName() == multipage1Name);
                    REQUIRE(doc.getPage(5)->indexInFile() == 5);
                    REQUIRE(doc.getPage(5)->fileName() == multipage1Name);
                }

                THEN("The last page of the document should be the last page of the first file")
                {
                    REQUIRE(doc.getPage(14)->indexInFile() == 14);
                    REQUIRE(doc.getPage(14)->fileName() == multipage1Name);
                }
            }
        }
    }
}
