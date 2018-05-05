#ifndef FILECHOOSERNATIVE_HPP
#define FILECHOOSERNATIVE_HPP

#include <gtkmm.h>

namespace Slicer {

// Wrapper class around GtkFileChooserNative

class FileChooserNative {
public:
    FileChooserNative(Gtk::Window& parent,
                      const Glib::ustring& title,
                      GtkFileChooserAction action,
                      const Glib::ustring& accept_label,
                      const Glib::ustring& cancel_label);
    ~FileChooserNative();

    void set_select_multiple(bool select_multiple);
    void set_current_name(const Glib::ustring& name);
    void set_filter(const Glib::RefPtr<Gtk::FileFilter>& filter);
    void set_do_overwrite_confirmation(bool do_overwrite_confirmation);
    std::string get_filename() const;
    Glib::RefPtr<Gio::File> get_file() const;
    gint run();

private:
    GtkFileChooserNative* m_fileChooser;
};

} // namespace Slicer

#endif // FILECHOOSERNATIVE_HPP
