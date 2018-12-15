#include "pdfsaver.hpp"
#include "tempfile.hpp"
#include <PDFWriter.h>
#include <PDFPage.h>
#include <PDFPageInput.h>
#include <PDFDocumentCopyingContext.h>

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
    InputFile sourceFile;
    sourceFile.OpenFile(m_document.filePath());
    PDFParser parser;
    parser.StartPDFParsing(sourceFile.GetInputStream());
    PDFWriter pdfWriter;
    pdfWriter.StartPDF(destinationFile->get_path(), ePDFVersionMax);
    std::unique_ptr<PDFDocumentCopyingContext> copyingContext{pdfWriter.CreatePDFCopyingContext(m_document.filePath())};

    for (unsigned int i = 0; i < m_document.pages()->get_n_items(); ++i) {
        const Glib::RefPtr<Page> slicerPage = m_document.pages()->get_item(i);
        const auto pageNumber = static_cast<unsigned int>(slicerPage->number());

        RefCountPtr<PDFDictionary> parsedPage = parser.ParsePage(pageNumber);
        PDFPageInput inputPage{&parser, parsedPage};
        PDFPage outputPage;

        outputPage.SetArtBox(inputPage.GetArtBox());
        outputPage.SetBleedBox(inputPage.GetBleedBox());
        outputPage.SetCropBox(inputPage.GetCropBox());
        outputPage.SetMediaBox(inputPage.GetMediaBox());
        outputPage.SetRotate(inputPage.GetRotate() + slicerPage->rotation());
        outputPage.SetTrimBox(inputPage.GetTrimBox());

        copyingContext->MergePDFPageToPage(&outputPage, pageNumber);
        pdfWriter.WritePage(&outputPage);
    }

    pdfWriter.EndPDF();
}

} // namespace Slicer
