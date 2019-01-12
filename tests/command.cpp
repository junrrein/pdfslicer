#include <catch.hpp>

#include <command.hpp>
#include <giomm/file.h>
#include <glibmm/miscutils.h>

using namespace Slicer;

static const std::string multipagePdfPath
    = Glib::build_filename(Glib::get_current_dir(), "multipage.pdf");

bool areIndexesMonoIncreasing(const Document& doc);

SCENARIO("Removing a single page from different places of a document using the Command abstraction")
{
    GIVEN("A multipage document with 15 pages")
    {
        auto multipagePdfFile = Gio::File::create_for_path(multipagePdfPath);
        Document doc{multipagePdfFile};
        REQUIRE(doc.numberOfPages() == 15);

        WHEN("The first page is removed")
        {
            RemovePageCommand removePageCommand(doc, 0);
            removePageCommand.execute();

            THEN("The first page of the document should be the second page of the file")
            REQUIRE(doc.getPage(0)->fileIndex() == 1);

            THEN("The document should have 14 pages")
            REQUIRE(doc.numberOfPages() == 14);

            WHEN("The remove command is undone")
            {
                removePageCommand.undo();

                THEN("The first page of the document should be the first page of the file")
                REQUIRE(doc.getPage(0)->fileIndex() == 0);

                THEN("The document should have 15 pages")
                REQUIRE(doc.numberOfPages() == 15);
            }

            WHEN("The new first page is removed")
            {
                RemovePageCommand removeSecondPageCommand{doc, 0};
                removeSecondPageCommand.execute();

                THEN("The first page of the document should be the third page of the file")
                REQUIRE(doc.getPage(0)->fileIndex() == 2);

                THEN("The document should have 13 pages")
                REQUIRE(doc.numberOfPages() == 13);

                WHEN("The second remove command is undone")
                {
                    removeSecondPageCommand.undo();

                    THEN("The first page of the document should be the second page of the file")
                    REQUIRE(doc.getPage(0)->fileIndex() == 1);

                    THEN("The document should have 14 pages")
                    REQUIRE(doc.numberOfPages() == 14);

                    WHEN("The first remove command is undone")
                    {
                        removePageCommand.undo();

                        THEN("The first page of the document should be the first page of the file")
                        REQUIRE(doc.getPage(0)->fileIndex() == 0);

                        THEN("The document should have 15 pages")
                        REQUIRE(doc.numberOfPages() == 15);
                    }
                }
            }
        }

        WHEN("The last page is removed")
        {
            RemovePageCommand removePageCommand{doc, 14};
            removePageCommand.execute();

            THEN("The document should have 14 pages")
            REQUIRE(doc.numberOfPages() == 14);

            THEN("The 14th page of the document should be the 14th page of the file")
            REQUIRE(doc.getPage(13)->fileIndex() == 13);

            WHEN("The remove command is undone")
            {
                removePageCommand.undo();

                THEN("The document should have 15 pages")
                REQUIRE(doc.numberOfPages() == 15);

                THEN("The 15th page of the document should be the 15th page of the file")
                REQUIRE(doc.getPage(14)->fileIndex() == 14);
            }
        }

        WHEN("The 8th page is removed")
        {
            RemovePageCommand removePageCommand{doc, 7};
            removePageCommand.execute();

            THEN("The document should have 14 pages")
            REQUIRE(doc.numberOfPages() == 14);

            THEN("The 7th and 8th pages of the document should be the 7th and 9th pages of the file, respectively")
            {
                REQUIRE(doc.getPage(6)->fileIndex() == 6);
                REQUIRE(doc.getPage(7)->fileIndex() == 8);
            }

            WHEN("The remove command is undone")
            {
                removePageCommand.undo();

                THEN("The document should have 15 pages")
                REQUIRE(doc.numberOfPages() == 15);

                THEN("The 7th, 8th and 9th pages of the document should be the corresponding pages of the file")
                {
                    REQUIRE(doc.getPage(6)->fileIndex() == 6);
                    REQUIRE(doc.getPage(7)->fileIndex() == 7);
                    REQUIRE(doc.getPage(8)->fileIndex() == 8);
                }
            }
        }
    }
}

SCENARIO("Removing a 2-page range from different places of a document using the Command abstraction")
{
    GIVEN("A multipage PDF document with 15 pages")
    {
        auto multipagePdfFile = Gio::File::create_for_path(multipagePdfPath);
        Document doc{multipagePdfFile};
        REQUIRE(doc.numberOfPages() == 15);

        WHEN("The first 2 pages are removed")
        {
            RemovePageRangeCommand removePageRangeCommand{doc, 0, 1};
            removePageRangeCommand.execute();

            THEN("The document should have 13 pages")
            REQUIRE(doc.numberOfPages() == 13);

            THEN("The first page of the document should be the 3rd page of the file")
            REQUIRE(doc.getPage(0)->fileIndex() == 2);

            WHEN("The command is undone")
            {
                removePageRangeCommand.undo();

                THEN("The document should have 15 pages")
                REQUIRE(doc.numberOfPages() == 15);

                THEN("The first page of the document should be the first page of the file")
                REQUIRE(doc.getPage(0)->fileIndex() == 0);
            }
        }

        WHEN("The last 2 pages are removed")
        {
            RemovePageRangeCommand removePageRangeCommand{doc, 13, 14};
            removePageRangeCommand.execute();

            THEN("The document should have 13 pages")
            REQUIRE(doc.numberOfPages() == 13);

            THEN("The last page of the document should be the 13th page of the file")
            REQUIRE(doc.getPage(12)->fileIndex() == 12);

            WHEN("The command is undone")
            {
                removePageRangeCommand.undo();

                THEN("The document should have 15 pages")
                REQUIRE(doc.numberOfPages() == 15);

                THEN("The last page of the document should be the last page of the file")
                REQUIRE(doc.getPage(14)->fileIndex() == 14);
            }
        }

        WHEN("The 8th and 9th pages are removed")
        {
            RemovePageRangeCommand removePageRangeCommand{doc, 7, 8};
            removePageRangeCommand.execute();

            THEN("The document should have 13 pages")
            REQUIRE(doc.numberOfPages() == 13);

            THEN("The new 8th and 9th pages should be the 10th and 11th, respectively")
            {
                REQUIRE(doc.getPage(7)->fileIndex() == 9);
                REQUIRE(doc.getPage(8)->fileIndex() == 10);
            }

            WHEN("The command is undone")
            {
                removePageRangeCommand.undo();

                THEN("The document should have 15 pages")
                REQUIRE(doc.numberOfPages() == 15);

                THEN("The 8th and 9th pages of the document should be the corresponding pages of the files")
                {
                    REQUIRE(doc.getPage(7)->fileIndex() == 7);
                    REQUIRE(doc.getPage(8)->fileIndex() == 8);
                }
            }
        }
    }
}

SCENARIO("Removing 2 disjoint pages from different places of a document using the Command abstraction")
{
    GIVEN("A multipage PDF document with 15 pages")
    {
        auto multipagePdfFile = Gio::File::create_for_path(multipagePdfPath);
        Document doc{multipagePdfFile};
        REQUIRE(doc.numberOfPages() == 15);

        WHEN("The 1st and 3rd pages are removed")
        {
            RemovePagesCommand removePagesCommand{doc, {0, 2}};
            removePagesCommand.execute();

            THEN("The document should have 13 pages")
            REQUIRE(doc.numberOfPages() == 13);

            THEN("The 1st and 2nd pages of the document should be the 2nd and 4th pages of the file, respectively")
            {
                REQUIRE(doc.getPage(0)->fileIndex() == 1);
                REQUIRE(doc.getPage(1)->fileIndex() == 3);
            }

            WHEN("The command is undone")
            {
                removePagesCommand.undo();

                THEN("The document should have 15 pages")
                REQUIRE(doc.numberOfPages() == 15);

                THEN("The 1st, 2nd and 3rd pages of the document should be the corresponding pages of the file")
                {
                    REQUIRE(doc.getPage(0)->fileIndex() == 0);
                    REQUIRE(doc.getPage(1)->fileIndex() == 1);
                    REQUIRE(doc.getPage(2)->fileIndex() == 2);
                }
            }
        }

        WHEN("The 13th and 15th pages are removed")
        {
            RemovePagesCommand removePagesCommand{doc, {12, 14}};
            removePagesCommand.execute();

            THEN("The document should have 13 pages")
            REQUIRE(doc.numberOfPages() == 13);

            THEN("The 12th and 13th pages of the document should be the 12th and 14th pages of the file")
            {
                REQUIRE(doc.getPage(11)->fileIndex() == 11);
                REQUIRE(doc.getPage(12)->fileIndex() == 13);
            }

            WHEN("The command is undone")
            {
                removePagesCommand.undo();

                THEN("The document should have 15 pages")
                REQUIRE(doc.numberOfPages() == 15);

                THEN("The 13th, 14th and 15th pages of the document should be the corresponding pages of the file")
                {
                    REQUIRE(doc.getPage(12)->fileIndex() == 12);
                    REQUIRE(doc.getPage(13)->fileIndex() == 13);
                    REQUIRE(doc.getPage(14)->fileIndex() == 14);
                }
            }
        }

        WHEN("The 8th and 10th pages are removed")
        {
            RemovePagesCommand removePagesCommand{doc, {7, 9}};
            removePagesCommand.execute();

            THEN("The document should have 13 pages")
            REQUIRE(doc.numberOfPages() == 13);

            THEN("The 8th and 9th pages of the document should now be the 9th and 11th pages of the file, respectively")
            {
                REQUIRE(doc.getPage(7)->fileIndex() == 8);
                REQUIRE(doc.getPage(8)->fileIndex() == 10);
            }

            WHEN("The command is undone")
            {
                removePagesCommand.undo();

                THEN("The document should have 15 pages")
                REQUIRE(doc.numberOfPages() == 15);

                THEN("The 8th, 9th and 10th pages of the document should be the corresponding pages of the file")
                {
                    REQUIRE(doc.getPage(7)->fileIndex() == 7);
                    REQUIRE(doc.getPage(8)->fileIndex() == 8);
                    REQUIRE(doc.getPage(9)->fileIndex() == 9);
                }
            }
        }
    }
}

SCENARIO("Remove all pages of a document before a certain one using RemovePageRangeCommand")
{
    GIVEN("A multipage PDF document with 15 pages")
    {
        auto multipagePdfFile = Gio::File::create_for_path(multipagePdfPath);
        Document doc{multipagePdfFile};
        REQUIRE(doc.numberOfPages() == 15);

        WHEN("All pages before the 4th are removed")
        {
            RemovePageRangeCommand command{doc, 0, 2};
            command.execute();

            THEN("The document should have 12 pages")
            REQUIRE(doc.numberOfPages() == 12);

            THEN("The first page of the document should be the 4th page of the file")
            REQUIRE(doc.getPage(0)->fileIndex() == 3);

            WHEN("The command is undone")
            {
                command.undo();

                THEN("The document should have 15 pages")
                REQUIRE(doc.numberOfPages() == 15);

                THEN("The first page of the document should be the first page of the file")
                REQUIRE(doc.getPage(0)->fileIndex() == 0);

                WHEN("The command is redone")
                {
                    command.redo();

                    THEN("The document should have 12 pages")
                    REQUIRE(doc.numberOfPages() == 12);

                    THEN("The first page of the document should be the 4th page of the file")
                    REQUIRE(doc.getPage(0)->fileIndex() == 3);
                }
            }
        }
    }
}

SCENARIO("Moving 1 page across different places of a document using the Command abstraction")
{
    GIVEN("A multipage PDF document with 15 pages")
    {
        auto multipagePdfFile = Gio::File::create_for_path(multipagePdfPath);
        Document doc{multipagePdfFile};
        REQUIRE(doc.numberOfPages() == 15);

        WHEN("The first page is moved to the 8th place")
        {
            MovePageCommand command{doc, 0, 7};
            command.execute();

            THEN("The 8th page of the document should be the 1st page of the file")
            REQUIRE(doc.getPage(7)->fileIndex() == 0);

            THEN("The 7th page of the document should be the 8th page of the file")
            REQUIRE(doc.getPage(6)->fileIndex() == 7);

            THEN("The 9th page of the document should be the 9th page of the file")
            REQUIRE(doc.getPage(8)->fileIndex() == 8);

            THEN("The first page of the document should be the second page of the file")
            REQUIRE(doc.getPage(0)->fileIndex() == 1);

            WHEN("The command is undone")
            {
                command.undo();

                THEN("Indexes should start from 0 and be monotonically increasing")
                REQUIRE(areIndexesMonoIncreasing(doc));
            }
        }

        WHEN("The last page is moved to the 8th place")
        {
            MovePageCommand command{doc, 14, 7};
            command.execute();

            THEN("The 8th page of the document should be the 15th page of the file")
            REQUIRE(doc.getPage(7)->fileIndex() == 14);

            THEN("The 9th page of the document should be the 8th page of the file")
            REQUIRE(doc.getPage(8)->fileIndex() == 7);

            THEN("The last page of the document should be the 14th page of the file")
            REQUIRE(doc.getPage(14)->fileIndex() == 13);

            WHEN("The command is undone")
            {
                command.undo();

                THEN("Indexes should start from 0 and be monotonically increasing")
                REQUIRE(areIndexesMonoIncreasing(doc));
            }
        }

        WHEN("The 8th page is moved to the 1st place")
        {
            MovePageCommand command{doc, 7, 0};
            command.execute();

            THEN("The first page of the document should be the 8th page of the file")
            REQUIRE(doc.getPage(0)->fileIndex() == 7);

            THEN("The second page of the document should be the first page of the file")
            REQUIRE(doc.getPage(1)->fileIndex() == 0);

            THEN("The 8th page of the document should be the 7th page of the file")
            REQUIRE(doc.getPage(7)->fileIndex() == 6);

            THEN("The 9th page of the document should be the 9th page of the file")
            REQUIRE(doc.getPage(8)->fileIndex() == 8);

            WHEN("The command is undone")
            {
                command.undo();

                THEN("Indexes should start from 0 and be monotonically increasing")
                REQUIRE(areIndexesMonoIncreasing(doc));
            }
        }

        WHEN("The 8th page is moved to the last place")
        {
            MovePageCommand command{doc, 7, 14};
            command.execute();

            THEN("The 15th page of the document should be the 8th page of the file")
            REQUIRE(doc.getPage(14)->fileIndex() == 7);

            THEN("The 14th page of the document should be the last page of the file")
            REQUIRE(doc.getPage(13)->fileIndex() == 14);

            THEN("The 8th page of the document should be the 9th page of the file")
            REQUIRE(doc.getPage(7)->fileIndex() == 8);

            WHEN("The command is undone")
            {
                command.undo();

                THEN("Indexes should start from 0 and be monotonically increasing")
                REQUIRE(areIndexesMonoIncreasing(doc));
            }
        }
    }
}

SCENARIO("Moving 2 adjacent pages across different places of a document, using the Command abstraction")
{
    GIVEN("A multipage PDF document with 15 pages")
    {
        auto multipagePdfFile = Gio::File::create_for_path(multipagePdfPath);
        Document doc{multipagePdfFile};
        REQUIRE(doc.numberOfPages() == 15);

        WHEN("The first 2 pages are moved to the 8th place")
        {
            MovePageRangeCommand command{doc, 0, 1, 7};
            command.execute();

            THEN("The 8th and 9th pages of the document should be the 1st and 2nd pages of the file, respectively")
            {
                REQUIRE(doc.getPage(7)->fileIndex() == 0);
                REQUIRE(doc.getPage(8)->fileIndex() == 1);
            }

            THEN("The 1st page of the document should be the 3rd page of the file")
            REQUIRE(doc.getPage(0)->fileIndex() == 2);

            THEN("The 6th and 7th pages of the document should be the 8th and 9th pages of the file, respectively")
            {
                REQUIRE(doc.getPage(5)->fileIndex() == 7);
                REQUIRE(doc.getPage(6)->fileIndex() == 8);
            }

            THEN("The 10th page of the document should be the 10th page of the file")
            REQUIRE(doc.getPage(9)->fileIndex() == 9);

            WHEN("The command is undone")
            {
                command.undo();

                THEN("Indexes should start from 0 and be monotonically increasing")
                REQUIRE(areIndexesMonoIncreasing(doc));
            }
        }

        WHEN("The first 2 pages are moved to the end of the document")
        {
            MovePageRangeCommand command{doc, 0, 1, 13};
            command.execute();

            THEN("The 14th and 15th pages of the document should be the 1st and 2nd pages of the file, respectively")
            {
                REQUIRE(doc.getPage(13)->fileIndex() == 0);
                REQUIRE(doc.getPage(14)->fileIndex() == 1);
            }

            THEN("The 1st page of the document should be the 3rd page of the file")
            REQUIRE(doc.getPage(0)->fileIndex() == 2);

            THEN("The 13th page of the document should be the 15th page of the file")
            REQUIRE(doc.getPage(12)->fileIndex() == 14);

            WHEN("The command is undone")
            {
                command.undo();

                THEN("Indexes should start from 0 and be monotonically increasing")
                REQUIRE(areIndexesMonoIncreasing(doc));
            }
        }

        WHEN("The last 2 pages are moved to the 8th place")
        {
            MovePageRangeCommand command{doc, 13, 14, 7};
            command.execute();

            THEN("The 8th and 9th pages of the document should be the 14th and 15th pages of the file, respectively")
            {
                REQUIRE(doc.getPage(7)->fileIndex() == 13);
                REQUIRE(doc.getPage(8)->fileIndex() == 14);
            }

            THEN("The last page of the document should be the 13th page of the file")
            REQUIRE(doc.getPage(14)->fileIndex() == 12);

            THEN("The 10th page of the document should be the 8th page of the file")
            REQUIRE(doc.getPage(9)->fileIndex() == 7);

            WHEN("The command is undone")
            {
                command.undo();

                THEN("Indexes should start from 0 and be monotonically increasing")
                REQUIRE(areIndexesMonoIncreasing(doc));
            }
        }

        WHEN("The last 2 pages are moved to the beggining")
        {
            MovePageRangeCommand command{doc, 13, 14, 0};
            command.execute();

            THEN("The 1st and 2nd pages of the document should be the 14th and 15th pages of the file")
            {
                REQUIRE(doc.getPage(0)->fileIndex() == 13);
                REQUIRE(doc.getPage(1)->fileIndex() == 14);
            }

            THEN("The last page of the document should be the 13th page of the file")
            REQUIRE(doc.getPage(14)->fileIndex() == 12);

            THEN("The 3rd page of the document should be the 1st page of the file")
            REQUIRE(doc.getPage(2)->fileIndex() == 0);

            WHEN("The command is undone")
            {
                command.undo();

                THEN("Indexes should start from 0 and be monotonically increasing")
                REQUIRE(areIndexesMonoIncreasing(doc));
            }
        }

        WHEN("The first 2 pages are moved to the 2nd and 3rd places in the document")
        {
            MovePageRangeCommand command{doc, 0, 1, 1};
            command.execute();

            THEN("The 2nd and 3rd pages of the document should be the 1st and 2nd pages of the file, respectively")
            {
                REQUIRE(doc.getPage(1)->fileIndex() == 0);
                REQUIRE(doc.getPage(2)->fileIndex() == 1);
            }

            THEN("The 1st page of the document should be the 3rd page of the file")
            REQUIRE(doc.getPage(0)->fileIndex() == 2);

            THEN("The 4rd page of the document should be the 4rd page of the file")
            REQUIRE(doc.getPage(3)->fileIndex() == 3);

            WHEN("The command is undone")
            {
                command.undo();

                THEN("Indexes should start from 0 and be monotonically increasing")
                REQUIRE(areIndexesMonoIncreasing(doc));
            }
        }
    }
}

bool areIndexesMonoIncreasing(const Document& doc)
{
    for (unsigned int i = 0; i < doc.numberOfPages(); ++i)
        if (doc.getPage(i)->getDocumentIndex() != i || doc.getPage(i)->fileIndex() != i)
            return false;

    return true;
}
