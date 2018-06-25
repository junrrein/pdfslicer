// PDF Slicer
// Copyright (C) 2017-2018 Julián Unrrein

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

#include "view.hpp"
#include <range/v3/all.hpp>

namespace Slicer {

View::View()
{
    set_column_spacing(10);
    set_row_spacing(20);

    set_selection_mode(Gtk::SELECTION_NONE);
}

std::vector<unsigned int> View::getSelectedChildrenIndexes()
{
    const std::vector<Gtk::FlowBoxChild*> children = get_selected_children();
    std::vector<unsigned int> selectedIndexes;
    ranges::transform(children,
                      ranges::back_inserter(selectedIndexes),
                      std::mem_fn(&Gtk::FlowBoxChild::get_index));

    return selectedIndexes;
}

std::vector<unsigned int> View::getUnselectedChildrenIndexes()
{
    const std::vector<unsigned int> selectedIndexes = getSelectedChildrenIndexes();
    std::vector<unsigned int> unselectedIndexes;
    ranges::set_difference(ranges::view::iota(0, get_children().size()),
                           selectedIndexes,
                           ranges::back_inserter(unselectedIndexes));

    return unselectedIndexes;
}

int View::getSelectedChildIndex()
{
    return get_selected_children().at(0)->get_index();
}
}
