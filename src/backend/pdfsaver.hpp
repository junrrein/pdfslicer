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

    void persist(const Glib::RefPtr<Gio::File>& destinationFile) const;
    void copyDocumentPage(unsigned int pageNumber,
                          PDFDocumentCopyingContext* sourceCopyingContext,
                          PDFWriter& destinationPdf) const;
};

} // namespace Slicer

#endif // PDFSAVER_HPP
