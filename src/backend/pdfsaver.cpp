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

    for (unsigned int i = 0; i < m_document.pages()->get_n_items(); ++i) {
        Glib::RefPtr<Page> slicerPage = m_document.getPage(i);
        QPDFPageObjectHelper pageCopy = slicerPage->m_qpdfPage.shallowCopyPage();

        if (slicerPage->rotation() != 0)
            pageCopy.rotatePage(slicerPage->rotation(), true);

        destinationPageDocumentHelper.addPage(pageCopy, false);
    }

    QPDFWriter writer{destinationPDF};
    writer.setOutputFilename(destinationFile->get_path().c_str());
    writer.write();
}

} // namespace Slicer
