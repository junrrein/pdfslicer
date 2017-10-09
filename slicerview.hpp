#ifndef SLICERVIEW_HPP
#define SLICERVIEW_HPP

#include "slicerdocument.hpp"
#include "previewwindow.hpp"
#include <gtkmm/flowbox.h>

namespace Slicer {

class View : public Gtk::FlowBox {
public:
    View(Slicer::Document& document);
    virtual ~View(){};

private:
    // A class to store a child with its associated page index
    //    class ViewChild : public Gtk::FlowBoxChild {
    //    public:
    //        ViewChild(Gdk::Pixbuf pixbuf, int pageIndex);

    //    private:
    //        const int m_pageIndex;
    //    };

    void onChildActivated(Gtk::FlowBoxChild* child);

    Slicer::Document& m_document;
    std::unique_ptr<Slicer::PreviewWindow> m_previewWindow;
};
}

#endif // SLICERVIEW_HPP
