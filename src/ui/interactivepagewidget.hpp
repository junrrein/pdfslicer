#ifndef INTERACTIVEPAGEWIDGET_HPP
#define INTERACTIVEPAGEWIDGET_HPP

#include "pagewidget.hpp"
#include <gtkmm/checkbutton.h>
#include <gtkmm/revealer.h>

namespace Slicer {

class InteractivePageWidget : public Slicer::PageWidget {

public:
    InteractivePageWidget(const Glib::RefPtr<Page>& page,
                          int targetSize);
    virtual ~InteractivePageWidget() = default;

    void setChecked(bool checked);
    bool getChecked() const { return m_isChecked; }

    sigc::signal<void, InteractivePageWidget*> selectedChanged;
    sigc::signal<void, InteractivePageWidget*> shiftSelected;
    sigc::signal<void, const Glib::RefPtr<Page>&> previewRequested;

private:
    bool m_isChecked = false;

    Gtk::CheckButton m_check;
    Gtk::Button m_previewButton;
    Gtk::Revealer m_previewButtonRevealer;

    void setupInteractiveWidgets();
    void setupSignalHandlers();
};

} // namespace Slicer

#endif // INTERACTIVEPAGEWIDGET_HPP
