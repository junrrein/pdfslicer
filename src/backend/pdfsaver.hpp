#ifndef PDFSAVER_HPP
#define PDFSAVER_HPP

#include "document.hpp"
#include "pdfannotationswriter.hpp"
#include <PDFWriter.h>
#include <PDFDocumentCopyingContext.h>

namespace Slicer {

class PdfSaver {
public:
    PdfSaver(Document& document);

    void save(const Glib::RefPtr<Gio::File>& destinationFile);

private:
    Document& m_document;
    PDFWriter m_destinationPdf;
    PdfAnnotationsWriter m_annotationsWriter;
    std::unique_ptr<PDFDocumentCopyingContext> m_sourceCopyingContext;

    void persist(const Glib::RefPtr<Gio::File>& destinationFile);
    void copyDocumentPage(unsigned int pageNumber);
    void addPageAnnotations(RefCountPtr<PDFDictionary> pageDictionary);
};

} // namespace Slicer

#endif // PDFSAVER_HPP
