#ifndef EDITOR_HPP
#define EDITOR_HPP

#include "../backend/document.hpp"
#include "view.hpp"
#include "actionbar.hpp"
#include <gtkmm/box.h>
#include <giomm/actionmap.h>
#include <gtkmm/scrolledwindow.h>

namespace Slicer {

class Editor : public Gtk::Box {
public:
    Editor(Gio::ActionMap& actionMap);

    void setDocument(Document& document);
    void waitForRenderCompletion();

private:
    View m_view;
    Gtk::ScrolledWindow m_scroller;
    ActionBar m_actionBar;
};

} // namespace Slicer

#endif // EDITOR_HPP
