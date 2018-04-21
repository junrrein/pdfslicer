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

    m_commandExecuted.emit();
}

void CommandManager::undo()
{
    m_undoStack.top()->undo();
    m_redoStack.push(m_undoStack.top());
    m_undoStack.pop();

    m_commandExecuted.emit();
}

void CommandManager::redo()
{
    m_redoStack.top()->redo();
    m_undoStack.push(m_redoStack.top());
    m_redoStack.pop();

    m_commandExecuted.emit();
}
}
