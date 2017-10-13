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
    ViewChild(const Glib::RefPtr<Slicer::GPopplerPage>& gPage);
    virtual ~ViewChild(){};

    void renderPage();
    void showPage();

private:
    const Glib::RefPtr<Slicer::GPopplerPage> m_gPage;
    Gtk::Image m_thumbnail;
    Gtk::Spinner m_spinner;
    bool m_isRendered;
    bool m_isShown;
};

} // namespace Slicer

#endif // VIEWCHILD_HPP
