#ifndef PAGE_HPP
#define PAGE_HPP

#include <glibmm/object.h>
#include <poppler.h>
#include <gdkmm/pixbuf.h>

namespace Slicer {

class Page : public Glib::Object {
public:
    Page(PopplerPage* ppage);
    virtual ~Page();

    int number() const;
    std::pair<int, int> size() const;
    std::pair<int, int> scaledSize(int targetSize) const;
    Glib::RefPtr<Gdk::Pixbuf> renderPage(int targetSize) const;

private:
    const PopplerPage* m_ppage;
};

} // namespace Slicer

#endif // PAGE_HPP
