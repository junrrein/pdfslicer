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

unsigned int maxIndex(const Document& doc)
{
    return doc.getPage(doc.numberOfPages() - 1)->getDocumentIndex();
}

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
            REQUIRE(doc.getPage(0)->indexInFile() == 1);

            THEN("The document should have 14 pages")
            REQUIRE(doc.numberOfPages() == 14);

            THEN("The max document index should be lower")
            REQUIRE(maxIndex(doc) == 13);

            WHEN("The remove command is undone")
            {
                doc.insertPage(removedPage);

                THEN("The first page of the document should be the first page of the file")
                REQUIRE(doc.getPage(0)->indexInFile() == 0);

                THEN("The document should have 15 pages")
                REQUIRE(doc.numberOfPages() == 15);

                THEN("The max index should be the original")
                REQUIRE(maxIndex(doc) == 14);
            }

            WHEN("The new first page is removed")
            {
                auto secondRemovedPage = doc.removePage(0);

                THEN("The first page of the document should be the third page of the file")
                REQUIRE(doc.getPage(0)->indexInFile() == 2);

                THEN("The document should have 13 pages")
                REQUIRE(doc.numberOfPages() == 13);

                THEN("The max document index should be even lower")
                REQUIRE(maxIndex(doc) == 12);

                WHEN("The second remove command is undone")
                {
                    doc.insertPage(secondRemovedPage);

                    THEN("The first page of the document should be the second page of the file")
                    REQUIRE(doc.getPage(0)->indexInFile() == 1);

                    THEN("The document should have 14 pages")
                    REQUIRE(doc.numberOfPages() == 14);

                    THEN("The max document index should be higher lower")
                    REQUIRE(maxIndex(doc) == 13);

                    WHEN("The first remove command is undone")
                    {
                        doc.insertPage(removedPage);

                        THEN("The first page of the document should be the first page of the file")
                        REQUIRE(doc.getPage(0)->indexInFile() == 0);

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
            REQUIRE(doc.getPage(13)->indexInFile() == 13);

            WHEN("The remove command is undone")
            {
                doc.insertPage(removedPage);

                THEN("The document should have 15 pages")
                REQUIRE(doc.numberOfPages() == 15);

                THEN("The max document index should be the original")
                REQUIRE(maxIndex(doc) == 14);

                THEN("The 15th page of the document should be the 15th page of the file")
                REQUIRE(doc.getPage(14)->indexInFile() == 14);
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
                REQUIRE(doc.getPage(6)->indexInFile() == 6);
                REQUIRE(doc.getPage(7)->indexInFile() == 8);
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
                    REQUIRE(doc.getPage(6)->indexInFile() == 6);
                    REQUIRE(doc.getPage(7)->indexInFile() == 7);
                    REQUIRE(doc.getPage(8)->indexInFile() == 8);
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
            REQUIRE(doc.getPage(0)->indexInFile() == 2);

            WHEN("The command is undone")
            {
                doc.insertPageRange(removedPageRange, positionFirst);

                THEN("The document should have 15 pages")
                REQUIRE(doc.numberOfPages() == 15);

                THEN("The max document index should be the original")
                REQUIRE(maxIndex(doc) == 14);

                THEN("The first page of the document should be the first page of the file")
                REQUIRE(doc.getPage(0)->indexInFile() == 0);
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
            REQUIRE(doc.getPage(12)->indexInFile() == 12);

            WHEN("The command is undone")
            {
                doc.insertPageRange(removedPageRange, positionFirst);

                THEN("The document should have 15 pages")
                REQUIRE(doc.numberOfPages() == 15);

                THEN("The max document index should be the original")
                REQUIRE(maxIndex(doc) == 14);

                THEN("The last page of the document should be the last page of the file")
                REQUIRE(doc.getPage(14)->indexInFile() == 14);
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
                REQUIRE(doc.getPage(7)->indexInFile() == 9);
                REQUIRE(doc.getPage(8)->indexInFile() == 10);
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
                    REQUIRE(doc.getPage(7)->indexInFile() == 7);
                    REQUIRE(doc.getPage(8)->indexInFile() == 8);
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
                REQUIRE(doc.getPage(0)->indexInFile() == 1);
                REQUIRE(doc.getPage(1)->indexInFile() == 3);
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
                    REQUIRE(doc.getPage(0)->indexInFile() == 0);
                    REQUIRE(doc.getPage(1)->indexInFile() == 1);
                    REQUIRE(doc.getPage(2)->indexInFile() == 2);
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
                REQUIRE(doc.getPage(11)->indexInFile() == 11);
                REQUIRE(doc.getPage(12)->indexInFile() == 13);
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
                    REQUIRE(doc.getPage(12)->indexInFile() == 12);
                    REQUIRE(doc.getPage(13)->indexInFile() == 13);
                    REQUIRE(doc.getPage(14)->indexInFile() == 14);
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
                REQUIRE(doc.getPage(7)->indexInFile() == 8);
                REQUIRE(doc.getPage(8)->indexInFile() == 10);
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
                    REQUIRE(doc.getPage(7)->indexInFile() == 7);
                    REQUIRE(doc.getPage(8)->indexInFile() == 8);
                    REQUIRE(doc.getPage(9)->indexInFile() == 9);
                }
            }
        }
    }
}
