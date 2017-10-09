#ifndef SLICERWINDOW_HPP
#define SLICERWINDOW_HPP

#include "slicerview.hpp"
#include <gtkmm/window.h>
#include <gtkmm/scrolledwindow.h>

namespace Slicer {

class Window : public Gtk::Window {
public:
    Window(std::string filePath);
    virtual ~Window(){};

private:
    Slicer::Document m_document;
    Slicer::View m_view;
    Gtk::ScrolledWindow m_scroller;
};
}

#endif // SLICERWINDOW_HPP
