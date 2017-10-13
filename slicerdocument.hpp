#ifndef DOCUMENT_HPP
#define DOCUMENT_HPP

#include <gdkmm/pixbuf.h>
#include <poppler.h>
#include <giomm/liststore.h>
#include <string>
#include <vector>

namespace Slicer {

// Convenience struct for storing Poppler pages in a Glib::ListStore
struct GPopplerPage : public Glib::Object {
    GPopplerPage(PopplerPage* t_page)
        : page{t_page} {};

    virtual ~GPopplerPage() { g_object_unref(page); };

    PopplerPage* page;
};

class Document {
public:
    // Constructor and destructor
    Document(std::string filePath);

    // Interface
    Glib::RefPtr<Gdk::Pixbuf> renderPage(PopplerPage* page,
                                         int targetSize) const;
    Glib::RefPtr<Gdk::Pixbuf> renderPage(int pageNumber,
                                         int targetSize) const;
    void saveDocument(std::string filePath) const;
    void removePage(int pageNumber);
    void removePageRange(int first, int last);

    // Member element access
    const Glib::RefPtr<Gio::ListStore<GPopplerPage>>& pages() const { return m_pages; };

private:
    // Member objects
    std::string m_sourcePath;
    Glib::RefPtr<Gio::ListStore<GPopplerPage>> m_pages;
};
}

#endif // DOCUMENT_HPP
