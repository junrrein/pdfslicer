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
    ViewChild(const Slicer::Document& document,
              const Glib::RefPtr<Slicer::GPopplerPage>& gPage,
              Glib::Dispatcher& uiDispatcher);
    virtual ~ViewChild(){};

    void renderPage();
    void showPage();

private:
    const Slicer::Document& m_document;
    Glib::Dispatcher& m_uiDispatcher;
    const Glib::RefPtr<Slicer::GPopplerPage> m_gPage;
    Gtk::Image m_thumbnail;
    Gtk::Spinner m_spinner;
    bool m_isRendered;
    bool m_isShown;
};

} // namespace Slicer

#endif // VIEWCHILD_HPP
