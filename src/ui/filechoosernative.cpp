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

#include "filechoosernative.hpp"

namespace Slicer {

FileChooserNative::FileChooserNative(Gtk::Window& parent,
                                     const Glib::ustring& title,
                                     GtkFileChooserAction action,
                                     const Glib::ustring& accept_label,
                                     const Glib::ustring& cancel_label)
{
    m_fileChooser = gtk_file_chooser_native_new(title.c_str(),
                                                parent.gobj(),
                                                action,
                                                accept_label.c_str(),
                                                cancel_label.c_str());
}

FileChooserNative::~FileChooserNative()
{
    g_object_unref(m_fileChooser);
}

void FileChooserNative::set_select_multiple(bool select_multiple)
{
    gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(m_fileChooser),
                                         select_multiple);
}

void FileChooserNative::set_current_name(const Glib::ustring& name)
{
    gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(m_fileChooser),
                                      name.c_str());
}

void FileChooserNative::add_filter(Glib::RefPtr<Gtk::FileFilter> filter)
{
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(m_fileChooser),
                                filter->gobj());
}

void FileChooserNative::set_do_overwrite_confirmation(bool do_overwrite_confirmation)
{
    gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(m_fileChooser),
                                                   do_overwrite_confirmation);
}

std::string FileChooserNative::get_filename() const
{
    return std::string{gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(m_fileChooser))};
}

Glib::RefPtr<Gio::File> FileChooserNative::get_file() const
{
    return Glib::wrap(gtk_file_chooser_get_file(GTK_FILE_CHOOSER(m_fileChooser)));
}

gint FileChooserNative::run()
{
    return gtk_native_dialog_run(GTK_NATIVE_DIALOG(m_fileChooser));
}

} // namespace Slicer
