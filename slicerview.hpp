#ifndef SLICERVIEW_HPP
#define SLICERVIEW_HPP

#include "slicerdocument.hpp"
#include <gtkmm/flowbox.h>

namespace Slicer {

class View : public Gtk::FlowBox {
public:
    View(Slicer::Document& document);
    virtual ~View(){};

private:
    Slicer::Document& m_document;
};
}

#endif // SLICERVIEW_HPP
