#ifndef DOCUMENT_HPP
#define DOCUMENT_HPP

#include <gdkmm/pixbuf.h>
#include <poppler.h>
#include <string>
#include <vector>

namespace Slicer {

class Document {
public:
    // Constructor and destructor
    Document(std::string filePath);
    ~Document();

    // Interface
    Glib::RefPtr<Gdk::Pixbuf> renderPage(int pageNumber,
                                         int width,
                                         int height) const;
    void saveDocument(std::string filePath) const;

    // Member element access
    const std::vector<const PopplerPage*>& pages() const { return m_pages; };

private:
    // Member objects
    std::string m_sourcePath;
    const PopplerDocument* m_popplerDocument;
    std::vector<const PopplerPage*> m_pages;
};
}

#endif // DOCUMENT_HPP
