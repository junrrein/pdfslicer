#include "pdfsaver.hpp"
#include "tempfile.hpp"
#include <qpdf/QPDFWriter.hh>

namespace Slicer {

PdfSaver::PdfSaver(const SaveData& saveData)
    : m_saveData{saveData}
{
    for (const auto& file : m_saveData.files) {
        auto qpdf = std::make_unique<QPDF>();
        qpdf->processFile(file->get_path().c_str());
        auto qpdfPageDocumentHelper = std::make_unique<QPDFPageDocumentHelper>(*qpdf);
        std::vector<QPDFPageObjectHelper> pages = qpdfPageDocumentHelper->getAllPages();

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
    QPDF* destinationPDF = m_filesData.front().qpdf.get();
    QPDFPageDocumentHelper* destinationPageDocumentHelper = m_filesData.front().qpdfPageDocumentHelper.get();

    for (auto qpdfPage : m_filesData.front().qpdfPages)
        destinationPageDocumentHelper->removePage(qpdfPage);

    for (PageData page : m_saveData.pages) {
        QPDFPageObjectHelper qpdfPage = m_filesData.at(page.file).qpdfPages.at(page.pageNumber);

        destinationPageDocumentHelper->addPage(qpdfPage, false);
    }

    QPDFWriter writer{*destinationPDF};
    writer.setOutputFilename(destinationFile->get_path().c_str());
    writer.write();

    //    QPDF destinationPDF;
    //    destinationPDF.emptyPDF();
    //    QPDFPageDocumentHelper destinationPageDocumentHelper{destinationPDF};

    //    for (unsigned int i = 0; i < m_document.pages()->get_n_items(); ++i) {
    //        Glib::RefPtr<Page> slicerPage = m_document.getPage(i);
    //        QPDFPageObjectHelper pageCopy = slicerPage->m_qpdfPage.shallowCopyPage();

    //        if (slicerPage->currentRotation() != slicerPage->sourceRotation())
    //            pageCopy.rotatePage(slicerPage->currentRotation(), false);

    //        destinationPageDocumentHelper.addPage(pageCopy, false);
    //    }

    //    QPDFWriter writer{destinationPDF};
    //    writer.setOutputFilename(destinationFile->get_path().c_str());
    //    writer.write();
}

} // namespace Slicer
