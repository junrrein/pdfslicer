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

void PdfSaver::persist(const Glib::RefPtr<Gio::File>& destinationFile)
{
    m_destinationPdf.StartPDF(destinationFile->get_path(), ePDFVersionMax);
    m_destinationPdf.GetDocumentContext().AddDocumentContextExtender(&m_annotationsWriter);
    m_sourceCopyingContext.reset(m_destinationPdf.CreatePDFCopyingContext(m_document.filePath()));

    for (unsigned int i = 0; i < m_document.pages()->get_n_items(); ++i)
        copyDocumentPage(i);

    m_sourceCopyingContext.reset();
    m_destinationPdf.GetDocumentContext().RemoveDocumentContextExtender(&m_annotationsWriter);
    m_destinationPdf.EndPDF();
}

void PdfSaver::copyDocumentPage(unsigned int pageNumber)
{
    Glib::RefPtr<const Page> slicerPage = m_document.getPage(pageNumber);
    const unsigned int pageFileIndex = slicerPage->fileIndex();

    RefCountPtr<PDFDictionary> parsedPage = m_sourceCopyingContext->GetSourceDocumentParser()->ParsePage(pageFileIndex);
    PDFPageInput inputPage{m_sourceCopyingContext->GetSourceDocumentParser(), parsedPage};
    PDFPage outputPage;

    outputPage.SetArtBox(inputPage.GetArtBox());
    outputPage.SetBleedBox(inputPage.GetBleedBox());
    outputPage.SetCropBox(inputPage.GetCropBox());
    outputPage.SetMediaBox(inputPage.GetMediaBox());
    outputPage.SetRotate(inputPage.GetRotate() + slicerPage->rotation());
    outputPage.SetTrimBox(inputPage.GetTrimBox());

    m_sourceCopyingContext->MergePDFPageToPage(&outputPage, pageFileIndex);
    m_annotationsWriter.addAnnotationsFromPage(parsedPage, m_sourceCopyingContext.get());
    m_destinationPdf.WritePage(&outputPage);
}

} // namespace Slicer
