#ifndef DOCUMENTSAVER_HPP
#define DOCUMENTSAVER_HPP

#include "document.hpp"

namespace Slicer {

class DocumentSaver {
public:
    DocumentSaver(Document& document);

    void saveDocument(const Glib::RefPtr<Gio::File>& destinationFile);

private:
    Document& m_document;

    void writePdfFile(const Glib::RefPtr<Gio::File>& destinationFile) const;
};

} // namespace Slicer

#endif // DOCUMENTSAVER_HPP
