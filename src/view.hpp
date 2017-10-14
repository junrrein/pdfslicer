#ifndef SLICERVIEW_HPP
#define SLICERVIEW_HPP

#include "document.hpp"
#include <gtkmm/flowbox.h>
#include <ThreadPool.h>
#include <glibmm/dispatcher.h>

namespace Slicer {

class View : public Gtk::FlowBox {
public:
    View(const Slicer::Document& document,
         int targetThumbnailSize);
    virtual ~View(){};

private:
    const Slicer::Document& m_document;
    ThreadPool m_pageRendererPool;
    Glib::Dispatcher m_viewUpdater;
};
}

#endif // SLICERVIEW_HPP
