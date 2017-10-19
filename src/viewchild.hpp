#ifndef VIEWCHILD_HPP
#define VIEWCHILD_HPP

#include "document.hpp"
#include <glibmm/dispatcher.h>
#include <gtkmm/box.h>
#include <gtkmm/image.h>
#include <gtkmm/spinner.h>

namespace Slicer {

class ViewChild : public Gtk::Box {
public:
    ViewChild(const Glib::RefPtr<Slicer::Page>& page,
              int targetSize);
    virtual ~ViewChild(){};

    void renderPage();
    void showPage();

private:
    const Glib::RefPtr<Slicer::Page> m_page;
    const int m_targetSize;
    Gtk::Image m_thumbnail;
    Gtk::Spinner m_spinner;
    bool m_isRendered;
    bool m_isShown;
};

} // namespace Slicer

#endif // VIEWCHILD_HPP
