#ifndef PDFSAVER_HPP
#define PDFSAVER_HPP

#include "document.hpp"
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
    std::unique_ptr<PDFDocumentCopyingContext> m_sourceCopyingContext;

    void persist(const Glib::RefPtr<Gio::File>& destinationFile);
    void copyDocumentPage(unsigned int pageNumber);
};

} // namespace Slicer

#endif // PDFSAVER_HPP
