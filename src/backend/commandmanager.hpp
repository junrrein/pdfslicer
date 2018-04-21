#ifndef COMMANDMANAGER_HPP
#define COMMANDMANAGER_HPP

#include "command.hpp"
#include <stack>

namespace Slicer {

using CommandStack = std::stack<std::shared_ptr<Command>>;

class CommandManager {
public:
    CommandManager() {}

    void execute(const std::shared_ptr<Command>& command);
    void undo();
    void redo();

    bool canUndo() const;
    bool canRedo() const;

    sigc::signal<void>& commandExecuted() { return m_commandExecuted; }

private:
    CommandStack m_undoStack;
    CommandStack m_redoStack;

    sigc::signal<void> m_commandExecuted;
};
}
#endif // COMMANDMANAGER_HPP
