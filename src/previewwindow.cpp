#include "previewwindow.hpp"
#include <gtkmm/cssprovider.h>

namespace Slicer {

PreviewWindow::PreviewWindow(Glib::RefPtr<Page>& page)
    : m_page{page}
    , m_zoomLevel{ZoomLevel::small}
{
    set_title("Preview");
    set_size_request(400, 400);
    set_default_size(900, 600);

    Glib::RefPtr<Gdk::Pixbuf> pixbuf = m_page->renderPage(static_cast<int>(m_zoomLevel));
    auto image = Gtk::manage(new Gtk::Image);
    image->set(pixbuf);

    m_buttonZoomOut.set_image_from_icon_name("zoom-out-symbolic");
    m_buttonZoomOut.set_tooltip_text("Zoom out");
    m_buttonZoomOut.get_style_context()->add_class("flat");
    m_buttonZoomOut.set_sensitive(false);
    m_buttonZoomOut.set_margin_top(8);
    m_buttonZoomOut.set_margin_bottom(8);
    m_buttonZoomOut.set_margin_left(8);
    m_buttonZoomIn.set_image_from_icon_name("zoom-in-symbolic");
    m_buttonZoomIn.set_tooltip_text("Zoom in");
    m_buttonZoomIn.get_style_context()->add_class("flat");
    m_buttonZoomIn.set_sensitive(true);
    m_buttonZoomIn.set_margin_top(8);
    m_buttonZoomIn.set_margin_bottom(8);
    m_buttonZoomIn.set_margin_right(8);
    m_boxZoom.pack_start(m_buttonZoomOut);
    m_boxZoom.pack_start(m_buttonZoomIn);
    m_boxZoom.get_style_context()->add_class("osd");
    m_boxZoom.set_valign(Gtk::ALIGN_END);
    m_boxZoom.set_halign(Gtk::ALIGN_END);
    m_boxZoom.set_margin_bottom(15);
    m_boxZoom.set_margin_right(15);

    m_scroller.add(*image);
    m_overlay.add(m_scroller);
    m_overlay.add_overlay(m_boxZoom);
    add(m_overlay);

    m_buttonZoomOut.signal_clicked().connect([this]() {
        decreaseZoomLevel();

        m_buttonZoomIn.set_sensitive(true);

        if (m_zoomLevel == ZoomLevel::small)
            m_buttonZoomOut.set_sensitive(false);
    });

    m_buttonZoomIn.signal_clicked().connect([this]() {
        increaseZoomLevel();

        m_buttonZoomOut.set_sensitive(true);

        if (m_zoomLevel == ZoomLevel::large)
            m_buttonZoomIn.set_sensitive(false);
    });

    m_signalZoomChanged.connect([this]() {
        Glib::RefPtr<Gdk::Pixbuf> pixbuf2 = m_page->renderPage(static_cast<int>(m_zoomLevel));
        auto image2 = Gtk::manage(new Gtk::Image);
        image2->set(pixbuf2);

        m_scroller.remove();
        m_scroller.add(*image2);
        image2->show();
    });

    auto screen = Gdk::Screen::get_default();
    auto provider = Gtk::CssProvider::create();
    provider->load_from_data(R"(
                             overlay > box {
                                border-radius: 8px;
                             }
                             )");
    Gtk::StyleContext::add_provider_for_screen(screen,
                                               provider,
                                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    show_all_children();
}

PreviewWindow::~PreviewWindow()
{
}

void PreviewWindow::increaseZoomLevel()
{
    switch (m_zoomLevel) {
    case ZoomLevel::small:
        m_zoomLevel = ZoomLevel::medium;
        break;

    case ZoomLevel::medium:
        m_zoomLevel = ZoomLevel::large;
        break;

    case ZoomLevel::large:;
    }

    m_signalZoomChanged.emit();
}

void PreviewWindow::decreaseZoomLevel()
{
    switch (m_zoomLevel) {
    case ZoomLevel::large:
        m_zoomLevel = ZoomLevel::medium;
        break;

    case ZoomLevel::medium:
        m_zoomLevel = ZoomLevel::small;
        break;

    case ZoomLevel::small:;
    }

    m_signalZoomChanged.emit();
}

} // namespace Slicer
