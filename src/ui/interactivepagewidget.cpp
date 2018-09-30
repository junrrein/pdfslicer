#include "interactivepagewidget.hpp"

namespace Slicer {

InteractivePageWidget::InteractivePageWidget(const Glib::RefPtr<Page>& page,
                                             int targetSize)
    : PageWidget(page, targetSize)
{
    setupInteractiveWidgets();
    setupSignalHandlers();
}

void InteractivePageWidget::setChecked(bool checked)
{
    if (m_isChecked != checked) {
        m_isChecked = checked;
        m_check.set_active(m_isChecked);
    }
}

void InteractivePageWidget::setupInteractiveWidgets()
{
    m_check.set_halign(Gtk::ALIGN_END);
    m_check.set_valign(Gtk::ALIGN_END);
    m_check.set_margin_bottom(10);
    m_check.set_margin_right(10);

    m_previewButton.set_image_from_icon_name("document-print-preview-symbolic");
    m_previewButtonRevealer.add(m_previewButton);
    m_previewButtonRevealer.set_transition_type(Gtk::REVEALER_TRANSITION_TYPE_CROSSFADE);
    m_previewButtonRevealer.set_halign(Gtk::ALIGN_END);
    m_previewButtonRevealer.set_valign(Gtk::ALIGN_START);
    m_previewButtonRevealer.set_margin_top(10);
    m_previewButtonRevealer.set_margin_right(10);

    m_overlay.add_overlay(m_check);
    m_overlay.add_overlay(m_previewButtonRevealer);
}

void InteractivePageWidget::setupSignalHandlers()
{
    m_overlayEventBox.signal_button_release_event().connect([this](GdkEventButton* eventButton) {
        if (eventButton->button == 1) {
            if (eventButton->state & GDK_SHIFT_MASK) {
                setChecked(true);
                shiftSelected.emit(this);
            }
            else {
                setChecked(!getChecked());
                selectedChanged.emit(this);
            }

            return true;
        }

        return false;
    });

    m_overlayEventBox.signal_enter_notify_event().connect([this](GdkEventCrossing*) {
        m_previewButtonRevealer.set_reveal_child(true);

        return false;
    });

    m_overlayEventBox.signal_leave_notify_event().connect([this](GdkEventCrossing*) {
        m_previewButtonRevealer.set_reveal_child(false);

        return false;
    });

    m_previewButton.signal_enter_notify_event().connect([this](GdkEventCrossing*) {
        m_previewButtonRevealer.set_reveal_child(true);

        return false;
    });

    m_previewButton.signal_button_release_event().connect([](GdkEventButton* eventButton) {
        if (eventButton->button == 1)
            return true;

        return false;
    });

    m_previewButton.signal_clicked().connect([this]() {
        previewRequested.emit(m_page);
    });
}

} // namespace Slicer
