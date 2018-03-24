#include "window.hpp"
#include <glibmm/main.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/popovermenu.h>
#include <gtkmm/cssprovider.h>
#include <gtkmm/adjustment.h>

namespace Slicer {

AppWindow::AppWindow()
    : m_boxMenuRemoveOptions{Gtk::ORIENTATION_VERTICAL}
    , m_zoomLevel{ZoomLevel::small}
    , m_view{nullptr}
    , m_labelDone{"Saved!"}
{
    // Widget setupb
    set_titlebar(m_headerBar);
    set_size_request(500, 500);
    set_default_size(800, 600);

    m_headerBar.set_title("PDF Slicer");
    m_headerBar.set_show_close_button(true);

    m_buttonOpen.set_image_from_icon_name("document-open-symbolic");
    m_buttonOpen.set_tooltip_text("Open document...");
    m_headerBar.pack_start(m_buttonOpen);

    m_buttonSave.set_image_from_icon_name("document-save-symbolic");
    m_buttonSave.set_tooltip_text("Save as...");
    m_buttonSave.set_sensitive(false);
    m_headerBar.pack_start(m_buttonSave);

    m_buttonRemovePages.set_image_from_icon_name("edit-delete-symbolic");
    m_buttonRemovePages.set_tooltip_text("Remove the selected page");
    m_buttonRemovePages.set_sensitive(false);
    m_boxRemovePages.pack_start(m_buttonRemovePages);

    m_buttonRemovePrevious.set_label("Remove previous pages");
    m_buttonRemoveNext.set_label("Remove next pages");

    m_boxMenuRemoveOptions.pack_start(m_buttonRemovePrevious);
    m_boxMenuRemoveOptions.pack_start(m_buttonRemoveNext);
    m_boxMenuRemoveOptions.set_margin_top(10);
    m_boxMenuRemoveOptions.set_margin_bottom(10);
    m_boxMenuRemoveOptions.set_margin_left(10);
    m_boxMenuRemoveOptions.set_margin_right(10);

    auto menuRemoveOptions = Gtk::manage(new Gtk::PopoverMenu);
    menuRemoveOptions->add(m_boxMenuRemoveOptions);
    menuRemoveOptions->show_all_children();

    m_buttonRemoveOptions.set_tooltip_text("More removing options");
    m_buttonRemoveOptions.get_style_context()->add_class("pepino");
    m_buttonRemoveOptions.set_popover(*menuRemoveOptions);
    m_buttonRemoveOptions.set_sensitive(false);
    m_boxRemovePages.pack_start(m_buttonRemoveOptions);

    m_boxRemovePages.get_style_context()->add_class("linked");
    m_headerBar.pack_start(m_boxRemovePages);

    m_buttonUndo.set_image_from_icon_name("edit-undo-symbolic");
    m_buttonUndo.set_tooltip_text("Undo");
    m_buttonUndo.set_sensitive(false);
    m_buttonRedo.set_image_from_icon_name("edit-redo-symbolic");
    m_buttonRedo.set_tooltip_text("Redo");
    m_buttonRedo.set_sensitive(false);

    auto undoBox = Gtk::manage(new Gtk::Box);
    undoBox->get_style_context()->add_class("linked");
    undoBox->pack_start(m_buttonUndo);
    undoBox->pack_start(m_buttonRedo);
    m_headerBar.pack_start(*undoBox);

    m_buttonPreviewPage.set_image_from_icon_name("document-print-preview-symbolic");
    m_buttonPreviewPage.set_tooltip_text("Preview the selected page");
    m_buttonPreviewPage.set_sensitive(false);
    m_headerBar.pack_end(m_buttonPreviewPage);

    m_buttonZoomOut.set_image_from_icon_name("zoom-out-symbolic");
    m_buttonZoomOut.set_tooltip_text("Zoom out");
    m_buttonZoomOut.set_sensitive(false);
    m_buttonZoomIn.set_image_from_icon_name("zoom-in-symbolic");
    m_buttonZoomIn.set_tooltip_text("Zoom in");
    m_buttonZoomIn.set_sensitive(false);
    m_boxZoom.pack_start(m_buttonZoomOut);
    m_boxZoom.pack_start(m_buttonZoomIn);
    m_boxZoom.get_style_context()->add_class("linked");
    m_headerBar.pack_end(m_boxZoom);

    m_labelDone.set_margin_top(10);
    m_labelDone.set_margin_bottom(10);
    m_labelDone.set_margin_left(15);
    m_labelDone.set_margin_right(7);
    m_buttonCloseDone.set_image_from_icon_name("window-close-symbolic");
    m_buttonCloseDone.get_style_context()->add_class("flat");
    m_buttonCloseDone.set_margin_top(5);
    m_buttonCloseDone.set_margin_bottom(5);
    m_buttonCloseDone.set_margin_right(5);
    m_boxDone.pack_start(m_labelDone, true, true);
    m_boxDone.pack_start(m_buttonCloseDone, false, false);
    m_boxDone.get_style_context()->add_class("osd");
    m_boxDone.set_size_request(1, 35);
    m_revealerDone.add(m_boxDone);
    m_revealerDone.set_halign(Gtk::ALIGN_CENTER);
    m_revealerDone.set_valign(Gtk::ALIGN_START);

    auto welcomeIcon = Gtk::manage(new Gtk::Image);
    welcomeIcon->set_from_icon_name("edit-cut-symbolic", Gtk::ICON_SIZE_BUTTON);
    welcomeIcon->set_pixel_size(256);
    auto welcomeLabel = Gtk::manage(new Gtk::Label{"Open a document to start slicing!"});
    welcomeLabel->get_style_context()->add_class("dim-label");
    welcomeLabel->get_style_context()->add_class("welcome-label");
    auto welcomeBox1 = Gtk::manage(new Gtk::Box{Gtk::ORIENTATION_VERTICAL});
    welcomeBox1->pack_start(*welcomeIcon, false, false, 10);
    welcomeBox1->pack_start(*welcomeLabel, false, false, 10);
    welcomeBox1->set_valign(Gtk::ALIGN_CENTER);
    auto welcomeBox2 = Gtk::manage(new Gtk::Box{Gtk::ORIENTATION_VERTICAL});
    welcomeBox2->pack_start(*welcomeBox1);
    m_scroller.add(*welcomeBox2);

    m_overlay.add(m_scroller);
    m_overlay.add_overlay(m_revealerDone);

    add(m_overlay);

    // Signal handlers
    m_buttonOpen.signal_clicked().connect([this]() {
        onOpenAction();
    });

    m_buttonSave.signal_clicked().connect([this]() {
        onSaveAction();
    });

    m_buttonRemovePages.signal_clicked().connect([this]() {
        removeSelectedPage();
    });

    m_buttonRemovePrevious.signal_clicked().connect([this]() {
        removePreviousPages();
        m_scroller.get_vadjustment()->set_value(0);
    });

    m_buttonRemoveNext.signal_clicked().connect([this]() {
        removeNextPages();
    });

    m_buttonUndo.signal_clicked().connect([this]() {
        m_view->waitForRenderCompletion();
        m_document->undoCommand();
    });

    m_buttonRedo.signal_clicked().connect([this]() {
        m_view->waitForRenderCompletion();
        m_document->redoCommand();
    });

    m_buttonPreviewPage.signal_clicked().connect([this]() {
        const int index = m_view->get_selected_children().at(0)->get_index();
        previewPage(index);
    });

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

    m_buttonCloseDone.signal_clicked().connect([this]() {
        m_revealerDone.set_reveal_child(false);
    });

    m_signalSaved.connect([this]() {
        // Stop any previous hiding animation started
        if (m_connectionSaved.connected())
            m_connectionSaved.disconnect();

        // Show notification and automatically hide it later
        m_revealerDone.set_reveal_child(true);
        m_connectionSaved = Glib::signal_timeout().connect([this]() {
            m_revealerDone.set_reveal_child(false);
            return false;
        },
                                                           5000);
    });

    m_signalZoomChanged.connect([this]() {
        buildView();
    });

    // Load custom CSS
    auto screen = Gdk::Screen::get_default();
    auto provider = Gtk::CssProvider::create();
    provider->load_from_data(R"(
        overlay > revealer > box {
            border-radius: 0px 0px 11px 11px;
        }

        .welcome-label {
            font-size: 19px;
            font-weight: bold;
        }

        .pepino {
            padding: 3px;
        }
    )");
    Gtk::StyleContext::add_provider_for_screen(screen,
                                               provider,
                                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    show_all_children();
}

AppWindow::~AppWindow()
{
}

void AppWindow::openDocument(const Glib::RefPtr<Gio::File>& file)
{
    std::string filePath = file->get_path();
    m_document = std::make_unique<Slicer::Document>(filePath);

    buildView();

    m_headerBar.set_subtitle(file->get_basename());
    m_buttonUndo.set_sensitive(false);
    m_buttonRedo.set_sensitive(false);

    m_document->commandExecuted().connect([this]() {
        if (m_document->canUndo())
            m_buttonUndo.set_sensitive(true);
        else
            m_buttonUndo.set_sensitive(false);

        if (m_document->canRedo())
            m_buttonRedo.set_sensitive(true);
        else
            m_buttonRedo.set_sensitive(false);
    });
}

void AppWindow::removeSelectedPage()
{
    auto child = m_view->get_selected_children().at(0);
    const int index = child->get_index();
    m_document->removePage(index);
}

void AppWindow::removePreviousPages()
{
    auto selected = m_view->get_selected_children();

    if (selected.size() != 1)
        throw std::runtime_error(
            "Tried to remove previous pages with more "
            "than one page selected. This should never happen!");

    const int index = selected.at(0)->get_index();

    m_document->removePageRange(0, index - 1);
}

void AppWindow::removeNextPages()
{
    auto selected = m_view->get_selected_children();

    if (selected.size() != 1)
        throw std::runtime_error(
            "Tried to remove next pages with more "
            "than one page selected. This should never happen!");

    const int index = selected.at(0)->get_index();

    m_document->removePageRange(index + 1,
                                static_cast<int>(m_document->pages()->get_n_items()) - 1);
}

Glib::RefPtr<Gtk::FileFilter> pdfFilter()
{
    auto filter = Gtk::FileFilter::create();
    filter->set_name("PDF documents");
    filter->add_mime_type("application/pdf");

    return filter;
}

void AppWindow::previewPage(int pageNumber)
{
    // We need to wait till the thumbnails finish rendering
    // before rendering a big preview, to prevent crashes.
    // Poppler isn't designed for rendering many pages of
    // the same document in different threads.
    m_view->waitForRenderCompletion();

    auto page = m_document->pages()->get_item(static_cast<unsigned>(pageNumber));
    m_previewWindow = std::make_unique<Slicer::PreviewWindow>(page);

    m_previewWindow->set_modal();
    m_previewWindow->set_transient_for(*this);
    m_previewWindow->show();
}

void AppWindow::increaseZoomLevel()
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

void AppWindow::decreaseZoomLevel()
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

void AppWindow::buildView()
{
    if (m_view == nullptr)
        m_buttonZoomIn.set_sensitive(true);

    m_scroller.remove();

    // It is necessary to invoke an empty reset() as a separate command.
    // reset() erases the old object it owns as the last thing it does.
    // So it may be the case that the new View starts rendering pages while the
    // old one is still rendering pages.
    // Poppler doesn't like two threads rendering pages from the same PDF, so this
    // would crash the program.
    m_view.reset();
    m_view.reset(new Slicer::View{*m_document, static_cast<int>(m_zoomLevel)});

    m_scroller.add(*m_view);
    m_scroller.show_all_children();

    m_buttonSave.set_sensitive(true);

    m_view->signal_selected_children_changed().connect([this]() {
        const unsigned long numSelected = m_view->get_selected_children().size();

        if (numSelected == 0)
            m_buttonRemovePages.set_sensitive(false);
        else
            m_buttonRemovePages.set_sensitive(true);

        if (numSelected == 1) {
            m_buttonRemoveOptions.set_sensitive(true);
            m_buttonPreviewPage.set_sensitive(true);

            const int index = m_view->get_selected_children().at(0)->get_index();
            if (index == 0)
                m_buttonRemovePrevious.set_sensitive(false);
            else
                m_buttonRemovePrevious.set_sensitive(true);

            if (index == static_cast<int>(m_view->get_children().size()) - 1)
                m_buttonRemoveNext.set_sensitive(false);
            else
                m_buttonRemoveNext.set_sensitive(true);
        }
        else {
            m_buttonRemoveOptions.set_sensitive(false);
            m_buttonPreviewPage.set_sensitive(false);
        }
    });

    m_view->signal_child_activated().connect([this](Gtk::FlowBoxChild* child) {
        const int pageIndex = child->get_index();
        previewPage(pageIndex);
    });
}

void AppWindow::onSaveAction()
{
    Gtk::FileChooserDialog dialog{*this,
                                  "Save document as",
                                  Gtk::FILE_CHOOSER_ACTION_SAVE};
    dialog.set_transient_for(*this);
    dialog.set_current_name("Untitled document");
    dialog.set_filter(pdfFilter());

    dialog.add_button("Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("Save", Gtk::RESPONSE_OK);

    const int result = dialog.run();

    if (result == Gtk::RESPONSE_OK) {
        std::string filePath = dialog.get_filename();
        const Glib::ustring baseName = dialog.get_file()->get_basename();

        if (baseName.find(".pdf") != baseName.size() - 4)
            filePath += ".pdf";

        m_document->saveDocument(filePath);
        m_signalSaved.emit();
    }
}

void AppWindow::onOpenAction()
{
    Gtk::FileChooserDialog dialog{*this,
                                  "Open document",
                                  Gtk::FILE_CHOOSER_ACTION_OPEN};
    dialog.set_transient_for(*this);
    dialog.set_select_multiple(false);
    dialog.set_filter(pdfFilter());

    dialog.add_button("Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("Open", Gtk::RESPONSE_OK);

    const int result = dialog.run();

    if (result == Gtk::RESPONSE_OK) {
        openDocument(dialog.get_file());
    }
}
}
