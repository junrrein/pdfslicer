#ifndef SLICERVIEW_HPP
#define SLICERVIEW_HPP

#include "document.hpp"
#include <gtkmm/flowbox.h>
#include <ctpl_stl.h>
#include <glibmm/dispatcher.h>

namespace Slicer {

class View : public Gtk::FlowBox {
public:
    View(const Slicer::Document& document,
         int targetThumbnailSize);
    virtual ~View(){};

    // This SHOULD be done manually before removing a view
    // from a window, or the program crashes.
    // Doing it in the destructor isn't soon enough.
    void stopRendering();

private:
    const Slicer::Document& m_document;
    ctpl::thread_pool m_pageRendererPool;
    Glib::Dispatcher m_viewUpdater;
};
}

#endif // SLICERVIEW_HPP
