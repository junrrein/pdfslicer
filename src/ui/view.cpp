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
#include "previewwindow.hpp"
#include <range/v3/all.hpp>

namespace Slicer {

View::View(Gio::ActionMap& actionMap, BackgroundThread& backgroundThread)
    : m_document{nullptr}
    , m_actionMap{actionMap}
    , m_backgroundThread{backgroundThread}
{
    set_column_spacing(10);
    set_row_spacing(20);

    set_selection_mode(Gtk::SELECTION_MULTIPLE);
    set_activate_on_single_click(false);

    addActions();
    setupSignalHandlers();
}

View::~View()
{
    m_actionMap.remove_action("remove-selected");
    m_actionMap.remove_action("remove-previous");
    m_actionMap.remove_action("remove-next");
    m_actionMap.remove_action("rotate-right");
    m_actionMap.remove_action("rotate-left");
    m_actionMap.remove_action("preview-selected");
}

void View::setDocument(Document& document)
{
    m_document = &document;
}

void View::addActions()
{
    m_removeSelectedAction = m_actionMap.add_action("remove-selected", sigc::mem_fun(*this, &View::removeSelectedPages));
    m_removeUnselectedAction = m_actionMap.add_action("remove-unselected", sigc::mem_fun(*this, &View::removeUnselectedPages));
    m_removePreviousAction = m_actionMap.add_action("remove-previous", sigc::mem_fun(*this, &View::removePreviousPages));
    m_removeNextAction = m_actionMap.add_action("remove-next", sigc::mem_fun(*this, &View::removeNextPages));
    m_rotateRightAction = m_actionMap.add_action("rotate-right", sigc::mem_fun(*this, &View::rotatePagesRight));
    m_rotateLeftAction = m_actionMap.add_action("rotate-left", sigc::mem_fun(*this, &View::rotatePagesLeft));
    m_previewPageAction = m_actionMap.add_action("preview-selected", sigc::mem_fun(*this, &View::previewPage));
    m_cancelSelectionAction = m_actionMap.add_action("cancel-selection", sigc::mem_fun(*this, &View::onCancelSelection));

    m_removeSelectedAction->set_enabled(false);
    m_removeUnselectedAction->set_enabled(false);
    m_removePreviousAction->set_enabled(false);
    m_removeNextAction->set_enabled(false);
    m_rotateRightAction->set_enabled(false);
    m_rotateLeftAction->set_enabled(false);
    m_previewPageAction->set_enabled(false);
    m_cancelSelectionAction->set_enabled(false);
}

void View::setupSignalHandlers()
{
    signal_selected_children_changed().connect([this]() {
        manageActionsEnabledStates();
    });

    signal_child_activated().connect([this](Gtk::FlowBoxChild*) {
        m_previewPageAction->activate();
    });
}

void View::manageActionsEnabledStates()
{
    const unsigned long numSelected = get_selected_children().size();

    if (numSelected == 0) {
        m_removeSelectedAction->set_enabled(false);
        m_removeUnselectedAction->set_enabled(false);
        m_removePreviousAction->set_enabled(false);
        m_removeNextAction->set_enabled(false);
        m_rotateRightAction->set_enabled(false);
        m_rotateLeftAction->set_enabled(false);
        m_cancelSelectionAction->set_enabled(false);
    }
    else {
        m_removeSelectedAction->set_enabled();
        m_rotateRightAction->set_enabled();
        m_rotateLeftAction->set_enabled();
        m_removeUnselectedAction->set_enabled();
        m_cancelSelectionAction->set_enabled();
    }

    if (numSelected == 1) {
        m_previewPageAction->set_enabled();

        const int index = get_selected_children().at(0)->get_index();
        if (index == 0)
            m_removePreviousAction->set_enabled(false);
        else
            m_removePreviousAction->set_enabled();

        if (index == static_cast<int>(get_children().size()) - 1)
            m_removeNextAction->set_enabled(false);
        else
            m_removeNextAction->set_enabled();
    }
    else {
        m_previewPageAction->set_enabled(false);
    }

    if (numSelected > 1) {
        m_removePreviousAction->set_enabled(false);
        m_removeNextAction->set_enabled(false);
    }
}

void View::onCancelSelection()
{
    unselect_all();
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

void View::removeSelectedPages()
{
    m_document->removePages(getSelectedChildrenIndexes());
}

void View::removeUnselectedPages()
{
    m_document->removePages(getUnselectedChildrenIndexes());
}

void View::removePreviousPages()
{
    const int index = get_selected_children().at(0)->get_index();
    m_document->removePageRange(0, index - 1);
}

void View::removeNextPages()
{
    const int index = get_selected_children().at(0)->get_index();
    m_document->removePageRange(index + 1,
                                static_cast<int>(m_document->pages()->get_n_items()) - 1);
}

void View::rotatePagesRight()
{
    m_document->rotatePagesRight(getSelectedChildrenIndexes());
}

void View::rotatePagesLeft()
{
    m_document->rotatePagesLeft(getSelectedChildrenIndexes());
}

void View::previewPage()
{
    const int pageNumber = get_selected_children().at(0)->get_index();

    Glib::RefPtr<Slicer::Page> page
        = m_document->pages()->get_item(static_cast<unsigned>(pageNumber));

    (new Slicer::PreviewWindow{page, m_backgroundThread})->show();
}
}
