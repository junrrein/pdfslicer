#ifndef PDFSAVER_HPP
#define PDFSAVER_HPP

#include "document.hpp"

namespace Slicer {

class PdfSaver {
public:
    PdfSaver(Document& document);

    void save(const Glib::RefPtr<Gio::File>& destinationFile);

private:
    Document& m_document;

    void persist(const Glib::RefPtr<Gio::File>& destinationFile);
};

} // namespace Slicer

#endif // PDFSAVER_HPP
