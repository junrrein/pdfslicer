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

#ifndef COMMANDMANAGER_HPP
#define COMMANDMANAGER_HPP

#include "command.hpp"
#include <stack>

namespace Slicer {

class CommandManager {
public:
    CommandManager() = default;

    void execute(const std::shared_ptr<Command>& command);
    void undo();
    void redo();
    void reset();

    bool canUndo() const;
    bool canRedo() const;

    sigc::signal<void> commandExecuted; // NOLINT

private:
    using CommandStack = std::stack<std::shared_ptr<Command>>;

    CommandStack m_undoStack;
    CommandStack m_redoStack;
};
}
#endif // COMMANDMANAGER_HPP
