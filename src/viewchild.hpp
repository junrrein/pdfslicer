#ifndef VIEWCHILD_HPP
#define VIEWCHILD_HPP

#include "document.hpp"
#include <ctpl_stl.h>
#include <glibmm/dispatcher.h>
#include <gtkmm/box.h>
#include <gtkmm/image.h>
#include <gtkmm/spinner.h>

namespace Slicer {

class ViewChild : public Gtk::Box {
public:
    ViewChild(Glib::RefPtr<Page> page,
              int targetSize,
              ctpl::thread_pool& threadPool);
    virtual ~ViewChild() = default;

    void renderPage();
    void showPage();

private:
    const Glib::RefPtr<Slicer::Page> m_page;
    const int m_targetSize;
    Gtk::Image m_thumbnail;
    Gtk::Spinner m_spinner;
    Glib::Dispatcher m_signalRendered;
};

} // namespace Slicer

#endif // VIEWCHILD_HPP
