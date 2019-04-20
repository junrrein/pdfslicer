#include "pdfsaver.hpp"
#include "tempfile.hpp"
#include <qpdf/QPDFPageDocumentHelper.hh>
#include <qpdf/QPDFWriter.hh>

namespace Slicer {

PdfSaver::PdfSaver(Document& document)
    : m_document{document}
{
}

void PdfSaver::save(const Glib::RefPtr<Gio::File>& destinationFile)
{
    Glib::RefPtr<Gio::File> tempFile = TempFile::generate();
    persist(tempFile);
    tempFile->move(destinationFile, Gio::FILE_COPY_OVERWRITE);
}

void PdfSaver::persist(const Glib::RefPtr<Gio::File>& destinationFile)
{
    QPDF destinationPDF;
    destinationPDF.emptyPDF();
    QPDFPageDocumentHelper destinationPageDocumentHelper{destinationPDF};

    QPDF sourcePDF;
    sourcePDF.processFile(m_document.filePath().c_str());
    QPDFPageDocumentHelper sourcePageDocumentHelper{sourcePDF};
    std::vector<QPDFPageObjectHelper> sourcePages = sourcePageDocumentHelper.getAllPages();

    for (unsigned int i = 0; i < m_document.pages()->get_n_items(); ++i) {
        Glib::RefPtr<const Page> slicerPage = m_document.getPage(i);
        const unsigned int pageFileIndex = slicerPage->fileIndex();

        QPDFPageObjectHelper pageCopy = sourcePages.at(pageFileIndex).shallowCopyPage();

        if (slicerPage->rotation() != 0)
            pageCopy.rotatePage(slicerPage->rotation(), true);

        destinationPageDocumentHelper.addPage(pageCopy, false);
    }

    QPDFWriter writer{destinationPDF};
    writer.setOutputFilename(destinationFile->get_path().c_str());
    writer.write();
}

} // namespace Slicer
