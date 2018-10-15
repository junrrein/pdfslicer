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

    std::string getTempFilePath() const;
    void makePDFCopy(const std::string& sourcePath,
                     const std::string& destinationPath) const;
};

} // namespace Slicer

#endif // DOCUMENTSAVER_HPP
