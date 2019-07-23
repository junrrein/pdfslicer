// PDF Slicer
// Copyright (C) 2019 Julián Unrrein

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

#include "addfiledialog.hpp"
#include "pdffilter.hpp"
#include <glibmm/i18n.h>

namespace Slicer {

AddFileDialog::AddFileDialog(Gtk::Window& window,
                             const std::string& folderPath,
                             bool enableAfterSelected)
    : Gtk::FileChooserDialog{window,
                             _("Select document to add"),
                             Gtk::FILE_CHOOSER_ACTION_OPEN}
{
    add_button(_("Add"), Gtk::RESPONSE_ACCEPT);
    add_button(_("Cancel"), Gtk::RESPONSE_CANCEL);
    set_select_multiple(false);
    add_filter(pdfFilter());
    set_current_folder(folderPath);

    setupOptionWidget(enableAfterSelected);
}

AddFileDialog::InsertPosition AddFileDialog::insertPosition() const
{
    if (m_radioButtonBeggining.get_active())
        return InsertPosition::beginning;

    if (m_radioButtonEnd.get_active())
        return InsertPosition::end;

    if (m_radioButtonAfterSelected.get_active())
        return InsertPosition::afterSelected;

    throw std::runtime_error("No alternative selected");
}

void AddFileDialog::setupOptionWidget(bool enableAfterSelected)
{
    m_insertPositionLabel.set_label(_("Insertion position:"));
    m_radioButtonBeggining.set_label(_("Beginning"));
    m_radioButtonEnd.set_label(_("End"));
    m_radioButtonAfterSelected.set_label(_("After the selected page"));

    m_radioButtonEnd.join_group(m_radioButtonBeggining);
    m_radioButtonAfterSelected.join_group(m_radioButtonBeggining);

    m_radioButtonsBox.set_orientation(Gtk::ORIENTATION_VERTICAL);
    m_radioButtonsBox.pack_start(m_radioButtonBeggining);
    m_radioButtonsBox.pack_start(m_radioButtonEnd);
    m_radioButtonsBox.pack_start(m_radioButtonAfterSelected);

    m_insertOptionBox.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    m_insertOptionBox.pack_start(m_insertPositionLabel);
    m_insertOptionBox.pack_start(m_radioButtonsBox);

    m_radioButtonBeggining.set_active();

    if (!enableAfterSelected)
        m_radioButtonAfterSelected.set_sensitive(false);

    set_extra_widget(m_insertOptionBox);
    m_insertOptionBox.show_all();
}

} // namespace Slicer
