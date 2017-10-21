#ifndef COMMANDMANAGER_HPP
#define COMMANDMANAGER_HPP

#include <stack>
#include <memory>
#include <sigc++/signal.h>

namespace Slicer {

class Command {
public:
    virtual ~Command(){};

    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual void redo() = 0;
};

using CommandStack = std::stack<std::shared_ptr<Command>>;

class CommandManager {
public:
    CommandManager(){};

    bool canUndo() const;
    bool canRedo() const;

    void execute(std::shared_ptr<Command> command);
    void undo();
    void redo();

    sigc::signal<void>& commandExecuted() { return m_commandExecuted; };

private:
    CommandStack m_undoStack;
    CommandStack m_redoStack;

    sigc::signal<void> m_commandExecuted;
};
}
#endif // COMMANDMANAGER_HPP
