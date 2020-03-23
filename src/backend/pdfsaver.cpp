#include "pdfsaver.hpp"
#include "tempfile.hpp"
#include <qpdf/QPDFWriter.hh>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/set_algorithm.hpp>

namespace Slicer {

PdfSaver::PdfSaver(const SaveData& saveData)
    : m_saveData{saveData}
{
    for (const auto& file : m_saveData.files) {
        auto qpdf = std::make_unique<QPDF>();
        qpdf->processFile(file->get_path().c_str());
        auto qpdfPageDocumentHelper = std::make_unique<QPDFPageDocumentHelper>(*qpdf);
        std::vector<QPDFPageObjectHelper> pages = qpdfPageDocumentHelper->getAllPages();

        qpdfPageDocumentHelper->pushInheritedAttributesToPage();

        m_filesData.emplace_back(FileData{std::move(qpdf),
                                          std::move(qpdfPageDocumentHelper),
                                          std::move(pages)});
    }
}

void PdfSaver::save(const Glib::RefPtr<Gio::File>& destinationFile)
{
    Glib::RefPtr<Gio::File> tempFile = TempFile::generate();
    persist(tempFile);
    tempFile->move(destinationFile, Gio::FILE_COPY_OVERWRITE);
}

void PdfSaver::persist(const Glib::RefPtr<Gio::File>& destinationFile)
{
    // Use the hollow shell of the first PDF to build the result.
    // This preserves the metadata and outline of that file.
    QPDF* destinationPDF = m_filesData.front().qpdf.get();
    QPDFPageDocumentHelper* destinationPageDocumentHelper = m_filesData.front().qpdfPageDocumentHelper.get();
    const std::vector<QPDFPageObjectHelper> originalPages = destinationPageDocumentHelper->getAllPages();

    for (const auto& qpdfPage : originalPages)
        destinationPageDocumentHelper->removePage(qpdfPage);

    // Add the wanted pages into the document.
    // It's necessary to keep track of which pages from the original document
    // got inserted.
    std::set<int> preserverdPagesFromOriginalFile;

    for (PageData page : m_saveData.pages) {
        QPDFPageObjectHelper qpdfPage = m_filesData.at(page.file).qpdfPages.at(page.pageNumber);
        qpdfPage.rotatePage(page.rotation, false);
        destinationPageDocumentHelper->addPage(qpdfPage, false);

        if (page.file == 0)
            preserverdPagesFromOriginalFile.insert(static_cast<int>(page.file));
    }

    // It's necessary to manually delete the page objects of the pages
    // from the original PDF which were deleted.
    // Taken from https://github.com/qpdf/qpdf/blob/release-qpdf-9.1.1/qpdf/qpdf.cc#L4980
    auto pagesFromOriginalFile = ranges::views::ints(0, static_cast<int>(originalPages.size()));
    auto removedPagesFromOriginalFile = ranges::views::set_difference(pagesFromOriginalFile, preserverdPagesFromOriginalFile);

    for (int pageNumber : removedPagesFromOriginalFile)
        destinationPDF->replaceObject(
            originalPages.at(static_cast<unsigned>(pageNumber)).getObjectHandle().getObjGen(),
            QPDFObjectHandle::newNull());

    destinationPageDocumentHelper->removeUnreferencedResources();

    // Write the result to a file
    QPDFWriter writer{*destinationPDF};
    writer.setOutputFilename(destinationFile->get_path().c_str());
    writer.write();
}

} // namespace Slicer
