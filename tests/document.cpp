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

unsigned int maxIndex(const Document& doc);

SCENARIO("Removing a single page from different places of a document")
{
    GIVEN("A multipage document with 15 pages")
    {
        auto multipagePdfFile = Gio::File::create_for_path(multipage1PdfPath);
        Document doc{multipagePdfFile};
        REQUIRE(doc.numberOfPages() == 15);

        WHEN("The first page is removed")
        {
            auto removedPage = doc.removePage(0);

            THEN("The first page of the document should be the second page of the file")
            REQUIRE(doc.getPage(0)->fileIndex() == 1);

            THEN("The document should have 14 pages")
            REQUIRE(doc.numberOfPages() == 14);

            THEN("The max document index should be lower")
            REQUIRE(maxIndex(doc) == 13);

            WHEN("The remove command is undone")
            {
                doc.insertPage(removedPage);

                THEN("The first page of the document should be the first page of the file")
                REQUIRE(doc.getPage(0)->fileIndex() == 0);

                THEN("The document should have 15 pages")
                REQUIRE(doc.numberOfPages() == 15);

                THEN("The max index should be the original")
                REQUIRE(maxIndex(doc) == 14);
            }

            WHEN("The new first page is removed")
            {
                auto secondRemovedPage = doc.removePage(0);

                THEN("The first page of the document should be the third page of the file")
                REQUIRE(doc.getPage(0)->fileIndex() == 2);

                THEN("The document should have 13 pages")
                REQUIRE(doc.numberOfPages() == 13);

                THEN("The max document index should be even lower")
                REQUIRE(maxIndex(doc) == 12);

                WHEN("The second remove command is undone")
                {
                    doc.insertPage(secondRemovedPage);

                    THEN("The first page of the document should be the second page of the file")
                    REQUIRE(doc.getPage(0)->fileIndex() == 1);

                    THEN("The document should have 14 pages")
                    REQUIRE(doc.numberOfPages() == 14);

                    THEN("The max document index should be higher lower")
                    REQUIRE(maxIndex(doc) == 13);

                    WHEN("The first remove command is undone")
                    {
                        doc.insertPage(removedPage);

                        THEN("The first page of the document should be the first page of the file")
                        REQUIRE(doc.getPage(0)->fileIndex() == 0);

                        THEN("The document should have 15 pages")
                        REQUIRE(doc.numberOfPages() == 15);

                        THEN("The max index should be the original")
                        REQUIRE(maxIndex(doc) == 14);
                    }
                }
            }
        }

        WHEN("The last page is removed")
        {
            auto removedPage = doc.removePage(14);

            THEN("The document should have 14 pages")
            REQUIRE(doc.numberOfPages() == 14);

            THEN("The max document index should be lower")
            REQUIRE(maxIndex(doc) == 13);

            THEN("The 14th page of the document should be the 14th page of the file")
            REQUIRE(doc.getPage(13)->fileIndex() == 13);

            WHEN("The remove command is undone")
            {
                doc.insertPage(removedPage);

                THEN("The document should have 15 pages")
                REQUIRE(doc.numberOfPages() == 15);

                THEN("The max document index should be the original")
                REQUIRE(maxIndex(doc) == 14);

                THEN("The 15th page of the document should be the 15th page of the file")
                REQUIRE(doc.getPage(14)->fileIndex() == 14);
            }
        }

        WHEN("The 8th page is removed")
        {
            auto removedPage = doc.removePage(7);

            THEN("The document should have 14 pages")
            REQUIRE(doc.numberOfPages() == 14);

            THEN("The max document index should be lower")
            REQUIRE(maxIndex(doc) == 13);

            THEN("The 7th and 8th pages of the document should be the 7th and 9th pages of the file, respectively")
            {
                REQUIRE(doc.getPage(6)->fileIndex() == 6);
                REQUIRE(doc.getPage(7)->fileIndex() == 8);
            }

            WHEN("The remove command is undone")
            {
                doc.insertPage(removedPage);

                THEN("The document should have 15 pages")
                REQUIRE(doc.numberOfPages() == 15);

                THEN("The max document index should be the original")
                REQUIRE(maxIndex(doc) == 14);

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

SCENARIO("Removing a 2-page range from different places of a document")
{
    GIVEN("A multipage PDF document with 15 pages")
    {
        auto multipagePdfFile = Gio::File::create_for_path(multipage1PdfPath);
        Document doc{multipagePdfFile};
        REQUIRE(doc.numberOfPages() == 15);

        WHEN("The first 2 pages are removed")
        {
            const int positionFirst = 0;
            auto removedPageRange = doc.removePageRange(positionFirst, 1);

            THEN("The document should have 13 pages")
            REQUIRE(doc.numberOfPages() == 13);

            THEN("The max document index should be lower")
            REQUIRE(maxIndex(doc) == 12);

            THEN("The first page of the document should be the 3rd page of the file")
            REQUIRE(doc.getPage(0)->fileIndex() == 2);

            WHEN("The command is undone")
            {
                doc.insertPageRange(removedPageRange, positionFirst);

                THEN("The document should have 15 pages")
                REQUIRE(doc.numberOfPages() == 15);

                THEN("The max document index should be the original")
                REQUIRE(maxIndex(doc) == 14);

                THEN("The first page of the document should be the first page of the file")
                REQUIRE(doc.getPage(0)->fileIndex() == 0);
            }
        }

        WHEN("The last 2 pages are removed")
        {
            const int positionFirst = 13;
            auto removedPageRange = doc.removePageRange(positionFirst, 14);

            THEN("The document should have 13 pages")
            REQUIRE(doc.numberOfPages() == 13);

            THEN("The max document index should be lower")
            REQUIRE(maxIndex(doc) == 12);

            THEN("The last page of the document should be the 13th page of the file")
            REQUIRE(doc.getPage(12)->fileIndex() == 12);

            WHEN("The command is undone")
            {
                doc.insertPageRange(removedPageRange, positionFirst);

                THEN("The document should have 15 pages")
                REQUIRE(doc.numberOfPages() == 15);

                THEN("The max document index should be the original")
                REQUIRE(maxIndex(doc) == 14);

                THEN("The last page of the document should be the last page of the file")
                REQUIRE(doc.getPage(14)->fileIndex() == 14);
            }
        }

        WHEN("The 8th and 9th pages are removed")
        {
            const int positionFirst = 7;
            auto removedPageRange = doc.removePageRange(positionFirst, 8);

            THEN("The document should have 13 pages")
            REQUIRE(doc.numberOfPages() == 13);

            THEN("The max document index should be lower")
            REQUIRE(maxIndex(doc) == 12);

            THEN("The new 8th and 9th pages should be the 10th and 11th, respectively")
            {
                REQUIRE(doc.getPage(7)->fileIndex() == 9);
                REQUIRE(doc.getPage(8)->fileIndex() == 10);
            }

            WHEN("The command is undone")
            {
                doc.insertPageRange(removedPageRange, positionFirst);

                THEN("The document should have 15 pages")
                REQUIRE(doc.numberOfPages() == 15);

                THEN("The max document index should be original")
                REQUIRE(maxIndex(doc) == 14);

                THEN("The 8th and 9th pages of the document should be the corresponding pages of the files")
                {
                    REQUIRE(doc.getPage(7)->fileIndex() == 7);
                    REQUIRE(doc.getPage(8)->fileIndex() == 8);
                }
            }
        }
    }
}

SCENARIO("Removing 2 disjoint pages from different places of a document")
{
    GIVEN("A multipage PDF document with 15 pages")
    {
        auto multipagePdfFile = Gio::File::create_for_path(multipage1PdfPath);
        Document doc{multipagePdfFile};
        REQUIRE(doc.numberOfPages() == 15);

        WHEN("The 1st and 3rd pages are removed")
        {
            auto removedPages = doc.removePages({0, 2});

            THEN("The document should have 13 pages")
            REQUIRE(doc.numberOfPages() == 13);

            THEN("The max document index should be lower")
            REQUIRE(maxIndex(doc) == 12);

            THEN("The 1st and 2nd pages of the document should be the 2nd and 4th pages of the file, respectively")
            {
                REQUIRE(doc.getPage(0)->fileIndex() == 1);
                REQUIRE(doc.getPage(1)->fileIndex() == 3);
            }

            WHEN("The command is undone")
            {
                doc.insertPages(removedPages);

                THEN("The document should have 15 pages")
                REQUIRE(doc.numberOfPages() == 15);

                THEN("The max document index should be the original")
                REQUIRE(maxIndex(doc) == 14);

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
            auto removedPages = doc.removePages({12, 14});

            THEN("The document should have 13 pages")
            REQUIRE(doc.numberOfPages() == 13);

            THEN("The max document index should be lower")
            REQUIRE(maxIndex(doc) == 12);

            THEN("The 12th and 13th pages of the document should be the 12th and 14th pages of the file")
            {
                REQUIRE(doc.getPage(11)->fileIndex() == 11);
                REQUIRE(doc.getPage(12)->fileIndex() == 13);
            }

            WHEN("The command is undone")
            {
                doc.insertPages(removedPages);

                THEN("The document should have 15 pages")
                REQUIRE(doc.numberOfPages() == 15);

                THEN("The max document index should be the original")
                REQUIRE(maxIndex(doc) == 14);

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
            auto removedPages = doc.removePages({7, 9});

            THEN("The document should have 13 pages")
            REQUIRE(doc.numberOfPages() == 13);

            THEN("The max document index should be lower")
            REQUIRE(maxIndex(doc) == 12);

            THEN("The 8th and 9th pages of the document should now be the 9th and 11th pages of the file, respectively")
            {
                REQUIRE(doc.getPage(7)->fileIndex() == 8);
                REQUIRE(doc.getPage(8)->fileIndex() == 10);
            }

            WHEN("The command is undone")
            {
                doc.insertPages(removedPages);

                THEN("The document should have 15 pages")
                REQUIRE(doc.numberOfPages() == 15);

                THEN("The max document index should be the original")
                REQUIRE(maxIndex(doc) == 14);

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
            REQUIRE(doc.getPage(7)->fileIndex() == 0);

            THEN("The 7th page of the document should be the 8th page of the file")
            REQUIRE(doc.getPage(6)->fileIndex() == 7);

            THEN("The 9th page of the document should be the 9th page of the file")
            REQUIRE(doc.getPage(8)->fileIndex() == 8);

            THEN("The first page of the document should be the second page of the file")
            REQUIRE(doc.getPage(0)->fileIndex() == 1);
        }

        WHEN("The last page is moved to the 8th place")
        {
            doc.movePage(14, 7);

            THEN("The 8th page of the document should be the 15th page of the file")
            REQUIRE(doc.getPage(7)->fileIndex() == 14);

            THEN("The 9th page of the document should be the 8th page of the file")
            REQUIRE(doc.getPage(8)->fileIndex() == 7);

            THEN("The last page of the document should be the 14th page of the file")
            REQUIRE(doc.getPage(14)->fileIndex() == 13);
        }

        WHEN("The 8th page is moved to the 1st place")
        {
            doc.movePage(7, 0);

            THEN("The first page of the document should be the 8th page of the file")
            REQUIRE(doc.getPage(0)->fileIndex() == 7);

            THEN("The second page of the document should be the first page of the file")
            REQUIRE(doc.getPage(1)->fileIndex() == 0);

            THEN("The 8th page of the document should be the 7th page of the file")
            REQUIRE(doc.getPage(7)->fileIndex() == 6);

            THEN("The 9th page of the document should be the 9th page of the file")
            REQUIRE(doc.getPage(8)->fileIndex() == 8);
        }

        WHEN("The 8th page is moved to the last place")
        {
            doc.movePage(7, 14);

            THEN("The 15th page of the document should be the 8th page of the file")
            REQUIRE(doc.getPage(14)->fileIndex() == 7);

            THEN("The 14th page of the document should be the last page of the file")
            REQUIRE(doc.getPage(13)->fileIndex() == 14);

            THEN("The 8th page of the document should be the 9th page of the file")
            REQUIRE(doc.getPage(7)->fileIndex() == 8);
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
        }

        WHEN("The first 2 pages are moved to the end of the document")
        {
            doc.movePageRange(0, 1, 13);

            THEN("The 14th and 15th pages of the document should be the 1st and 2nd pages of the file, respectively")
            {
                REQUIRE(doc.getPage(13)->fileIndex() == 0);
                REQUIRE(doc.getPage(14)->fileIndex() == 1);
            }

            THEN("The 1st page of the document should be the 3rd page of the file")
            REQUIRE(doc.getPage(0)->fileIndex() == 2);

            THEN("The 13th page of the document should be the 15th page of the file")
            REQUIRE(doc.getPage(12)->fileIndex() == 14);
        }

        WHEN("The last 2 pages are moved to the 8th place")
        {
            doc.movePageRange(13, 14, 7);

            THEN("The 8th and 9th pages of the document should be the 14th and 15th pages of the file, respectively")
            {
                REQUIRE(doc.getPage(7)->fileIndex() == 13);
                REQUIRE(doc.getPage(8)->fileIndex() == 14);
            }

            THEN("The last page of the document should be the 13th page of the file")
            REQUIRE(doc.getPage(14)->fileIndex() == 12);

            THEN("The 10th page of the document should be the 8th page of the file")
            REQUIRE(doc.getPage(9)->fileIndex() == 7);
        }

        WHEN("The last 2 pages are moved to the beggining")
        {
            doc.movePageRange(13, 14, 0);

            THEN("The 1st and 2nd pages of the document should be the 14th and 15th pages of the file")
            {
                REQUIRE(doc.getPage(0)->fileIndex() == 13);
                REQUIRE(doc.getPage(1)->fileIndex() == 14);
            }

            THEN("The last page of the document should be the 13th page of the file")
            REQUIRE(doc.getPage(14)->fileIndex() == 12);

            THEN("The 3rd page of the document should be the 1st page of the file")
            REQUIRE(doc.getPage(2)->fileIndex() == 0);
        }

        WHEN("The first 2 pages are moved to the 2nd and 3rd places in the document")
        {
            doc.movePageRange(0, 1, 1);

            THEN("The 2nd and 3rd pages of the document should be the 1st and 2nd pages of the file, respectively")
            {
                REQUIRE(doc.getPage(1)->fileIndex() == 0);
                REQUIRE(doc.getPage(2)->fileIndex() == 1);
            }

            THEN("The 1st page of the document should be the 3rd page of the file")
            REQUIRE(doc.getPage(0)->fileIndex() == 2);

            THEN("The 4rd page of the document should be the 4rd page of the file")
            REQUIRE(doc.getPage(3)->fileIndex() == 3);
        }
    }
}

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
                REQUIRE(doc.getPage(0)->fileIndex() == 0);
                REQUIRE(doc.getPage(0)->fileName() == multipage2Name);
            }

            THEN("The 5th page of the document should be the last page of the second file")
            {
                REQUIRE(doc.getPage(4)->fileIndex() == 4);
                REQUIRE(doc.getPage(4)->fileName() == multipage2Name);
            }

            THEN("The 6th page of the document should be the first page of the first file")
            {
                REQUIRE(doc.getPage(5)->fileIndex() == 0);
                REQUIRE(doc.getPage(5)->fileName() == multipage1Name);
            }

            THEN("The last page of the document should be the last page of the first file")
            {
                REQUIRE(doc.getPage(19)->fileIndex() == 14);
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
                REQUIRE(doc.getPage(0)->fileIndex() == 0);
                REQUIRE(doc.getPage(0)->fileName() == multipage1Name);
            }

            THEN("The 15th page of the document should be the last page of the first file")
            {
                REQUIRE(doc.getPage(14)->fileIndex() == 14);
                REQUIRE(doc.getPage(14)->fileName() == multipage1Name);
            }

            THEN("The 16th page of the document should be the first page of the second file")
            {
                REQUIRE(doc.getPage(15)->fileIndex() == 0);
                REQUIRE(doc.getPage(15)->fileName() == multipage2Name);
            }

            THEN("The last page of the document should be the last page of the second file")
            {
                REQUIRE(doc.getPage(19)->fileIndex() == 4);
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
                REQUIRE(doc.getPage(3)->fileIndex() == 3);
                REQUIRE(doc.getPage(3)->fileName() == multipage1Name);
            }

            THEN("The 5th page of the document should be the 1st page of the second file")
            {
                REQUIRE(doc.getPage(4)->fileIndex() == 0);
                REQUIRE(doc.getPage(4)->fileName() == multipage2Name);
            }

            THEN("The 9th page of the document should be the 5th page of the second file")
            {
                REQUIRE(doc.getPage(8)->fileIndex() == 4);
                REQUIRE(doc.getPage(8)->fileName() == multipage2Name);
            }

            THEN("The 10th page of the document should be the 5th page of the first file")
            {
                REQUIRE(doc.getPage(9)->fileIndex() == 4);
                REQUIRE(doc.getPage(9)->fileName() == multipage1Name);
            }
        }
    }
}

unsigned int maxIndex(const Document& doc)
{
    return doc.getPage(doc.numberOfPages() - 1)->getDocumentIndex();
}
