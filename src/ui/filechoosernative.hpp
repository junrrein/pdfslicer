// PDF Slicer
// Copyright (C) 2018 Julián Unrrein

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

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
    void add_filter(Glib::RefPtr<Gtk::FileFilter> filter);
    void set_do_overwrite_confirmation(bool do_overwrite_confirmation);
    Glib::RefPtr<Gio::File> get_file() const;
    gint run();

private:
    GtkFileChooserNative* m_fileChooser;
};

} // namespace Slicer

#endif // FILECHOOSERNATIVE_HPP
