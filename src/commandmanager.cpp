#include "commandmanager.hpp"

namespace Slicer {

RemovePageCommand::RemovePageCommand(const Glib::RefPtr<Gio::ListStore<Page>> pages,
                                     int position)
    : m_pages{pages}
    , m_position{position}
{
    m_removedPage = m_pages->get_item(static_cast<unsigned>(position));
}

void RemovePageCommand::execute()
{
    m_pages->remove(static_cast<unsigned>(m_position));
}

void RemovePageCommand::undo()
{
    m_pages->insert(static_cast<unsigned>(m_position), m_removedPage);
}

void RemovePageCommand::redo()
{
    execute();
}

RemovePageRangeCommand::RemovePageRangeCommand(const Glib::RefPtr<Gio::ListStore<Page>> pages,
                                               int first,
                                               int last)
    : m_pages{pages}
    , m_first{first}
    , m_last{last}
{
    // Store pages in reversed order, since Gio::ListStore::splice()
    // inserts them in reversed order.
    for (int i = last; i >= first; --i)
        m_removedPages.push_back(m_pages->get_item(static_cast<unsigned>(i)));
}

void RemovePageRangeCommand::execute()
{
    const int nElem = m_last - m_first + 1;

    m_pages->splice(static_cast<unsigned>(m_first),
                    static_cast<unsigned>(nElem),
                    {});
}

void RemovePageRangeCommand::undo()
{
    m_pages->splice(static_cast<unsigned>(m_first), 0, m_removedPages);
}

void RemovePageRangeCommand::redo()
{
    execute();
}

bool CommandManager::canUndo() const
{
    return !m_undoStack.empty();
}

bool CommandManager::canRedo() const
{
    return !m_redoStack.empty();
}

void CommandManager::execute(std::shared_ptr<Command> command)
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

Command::~Command()
{
}
}
