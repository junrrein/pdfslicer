#ifndef SLICERVIEW_HPP
#define SLICERVIEW_HPP

#include "document.hpp"
#include <gtkmm/flowbox.h>
#include <ctpl_stl.h>

namespace Slicer {

class View : public Gtk::FlowBox {
public:
    View(const Slicer::Document& document,
         int targetThumbnailSize);
    virtual ~View();

    void waitForRenderCompletion();

private:
    const Slicer::Document& m_document;
    std::unique_ptr<ctpl::thread_pool> m_pageRendererPool;
};
}

#endif // SLICERVIEW_HPP
