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

#include "commandmanager.hpp"

namespace Slicer {

bool CommandManager::canUndo() const
{
    return !m_undoStack.empty();
}

bool CommandManager::canRedo() const
{
    return !m_redoStack.empty();
}

void CommandManager::execute(const std::shared_ptr<Command>& command)
{
    m_redoStack = CommandStack{};
    command->execute();
    m_undoStack.push(command);

    commandExecuted.emit();
}

void CommandManager::undo()
{
    m_undoStack.top()->undo();
    m_redoStack.push(m_undoStack.top());
    m_undoStack.pop();

    commandExecuted.emit();
}

void CommandManager::redo()
{
    m_redoStack.top()->redo();
    m_undoStack.push(m_redoStack.top());
    m_redoStack.pop();

    commandExecuted.emit();
}

void CommandManager::reset()
{
    m_undoStack = {};
    m_redoStack = {};

    commandExecuted.emit();
}
}
