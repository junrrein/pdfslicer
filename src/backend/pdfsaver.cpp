#include "pdfsaver.hpp"
#include "tempfile.hpp"
#include <PDFPage.h>
#include <PDFPageInput.h>

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

void PdfSaver::persist(const Glib::RefPtr<Gio::File>& destinationFile) const
{
    PDFWriter destinationPdf;
    destinationPdf.StartPDF(destinationFile->get_path(), ePDFVersionMax);
    std::unique_ptr<PDFDocumentCopyingContext> sourceCopyingContext{destinationPdf.CreatePDFCopyingContext(m_document.filePath())};

    for (unsigned int i = 0; i < m_document.pages()->get_n_items(); ++i)
        copyDocumentPage(i, sourceCopyingContext.get(), destinationPdf);

    destinationPdf.EndPDF();
}

void PdfSaver::copyDocumentPage(unsigned int pageNumber,
                                PDFDocumentCopyingContext* sourceCopyingContext,
                                PDFWriter& destinationPdf) const
{
    Glib::RefPtr<const Page> slicerPage = m_document.getPage(pageNumber);
    const unsigned int pageFileIndex = slicerPage->fileIndex();

    RefCountPtr<PDFDictionary> parsedPage = sourceCopyingContext->GetSourceDocumentParser()->ParsePage(pageFileIndex);
    PDFPageInput inputPage{sourceCopyingContext->GetSourceDocumentParser(), parsedPage};
    PDFPage outputPage;

    outputPage.SetArtBox(inputPage.GetArtBox());
    outputPage.SetBleedBox(inputPage.GetBleedBox());
    outputPage.SetCropBox(inputPage.GetCropBox());
    outputPage.SetMediaBox(inputPage.GetMediaBox());
    outputPage.SetRotate(inputPage.GetRotate() + slicerPage->rotation());
    outputPage.SetTrimBox(inputPage.GetTrimBox());

    sourceCopyingContext->MergePDFPageToPage(&outputPage, pageFileIndex);
    destinationPdf.WritePage(&outputPage);
}

} // namespace Slicer
