#ifndef COMMANDMANAGER_HPP
#define COMMANDMANAGER_HPP

#include "page.hpp"
#include <stack>
#include <giomm/liststore.h>

namespace Slicer {

class Command {
public:
    virtual ~Command(){};

    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual void redo() = 0;
};

class RemovePageCommand : public Command {
public:
    RemovePageCommand(const Glib::RefPtr<Gio::ListStore<Page>> pages,
                      int position);

    void execute() override;
    void undo() override;
    void redo() override;

private:
    Glib::RefPtr<Gio::ListStore<Page>> m_pages;
    Glib::RefPtr<Page> m_removedPage;
    int m_position;
};

class RemovePageRangeCommand : public Command {
public:
    RemovePageRangeCommand(const Glib::RefPtr<Gio::ListStore<Page>> pages,
                           int first,
                           int last);

    void execute() override;
    void undo() override;
    void redo() override;

private:
    Glib::RefPtr<Gio::ListStore<Page>> m_pages;
    std::vector<Glib::RefPtr<Page>> m_removedPages;
    int m_first, m_last;
};

using CommandStack = std::stack<std::shared_ptr<Command>>;

class CommandManager {
public:
    CommandManager(){};

    void execute(std::shared_ptr<Command> command);
    void undo();
    void redo();

    bool canUndo() const;
    bool canRedo() const;

    sigc::signal<void>& commandExecuted() { return m_commandExecuted; };

private:
    CommandStack m_undoStack;
    CommandStack m_redoStack;

    sigc::signal<void> m_commandExecuted;
};
}
#endif // COMMANDMANAGER_HPP
