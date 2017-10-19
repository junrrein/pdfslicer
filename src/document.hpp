#ifndef DOCUMENT_HPP
#define DOCUMENT_HPP

#include "page.hpp"
#include <giomm/liststore.h>
#include <string>

namespace Slicer {

class Document {
public:
    // Constructor and destructor
    Document(std::string filePath);

    // Interface
    void saveDocument(std::string filePath) const;
    void removePage(int pageNumber);
    void removePageRange(int first, int last);

    // Member element access
    const Glib::RefPtr<Gio::ListStore<Page>>& pages() const { return m_pages; };

private:
    // Member objects
    std::string m_sourcePath;
    Glib::RefPtr<Gio::ListStore<Page>> m_pages;
};
}

#endif // DOCUMENT_HPP
