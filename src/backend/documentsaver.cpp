#include "documentsaver.hpp"
#include <glibmm/miscutils.h>
#include <PDFWriter.h>
#include <PDFPage.h>
#include <PDFPageInput.h>
#include <PDFDocumentCopyingContext.h>

namespace Slicer {

DocumentSaver::DocumentSaver(Document& document)
    : m_document{document}
{
}

void DocumentSaver::saveDocument(const Glib::RefPtr<Gio::File>& destinationFile)
{
    const std::string tempFilePath = getTempFilePath();
    auto tempFile = Gio::File::create_for_path(tempFilePath);

    makePDFCopy(m_document.m_sourceFile->get_path(), tempFilePath);
    tempFile->move(destinationFile, Gio::FILE_COPY_OVERWRITE);

    // FIXME: No need to do this once PDFWriter gets support for
    // in-memory documents.
    // TODO: Maybe save the file to a temporary place when opening?
    // And use that temporary file as the source later
    if (m_document.m_sourceFile->get_path() == destinationFile->get_path())
        m_document.reload();
}

std::string DocumentSaver::getTempFilePath() const
{
    const std::string tempDirectory = Glib::get_tmp_dir();
    const std::string tempFileName = "pdfslicer-temp.pdf";
    const std::vector<std::string> pathParts = {tempDirectory, tempFileName};
    const std::string tempFilePath = Glib::build_filename(pathParts);

    return tempFilePath;
}

void DocumentSaver::makePDFCopy(const std::string& sourcePath,
                                const std::string& destinationPath) const
{
    InputFile sourceFile;
    sourceFile.OpenFile(sourcePath);
    PDFParser parser;
    parser.StartPDFParsing(sourceFile.GetInputStream());
    PDFWriter pdfWriter;
    pdfWriter.StartPDF(destinationPath, ePDFVersionMax);
    std::unique_ptr<PDFDocumentCopyingContext> copyingContext{pdfWriter.CreatePDFCopyingContext(sourcePath)};

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
