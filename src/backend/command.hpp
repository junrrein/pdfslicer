#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "page.hpp"
#include <giomm/liststore.h>

namespace Slicer {

class Command {
public:
    virtual ~Command() = default;

    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual void redo() = 0;
};

class RemovePageCommand : public Command {
public:
    RemovePageCommand(Glib::RefPtr<Gio::ListStore<Page>> pages,
                      int position);

    void execute() override;
    void undo() override;
    void redo() override;

private:
    Glib::RefPtr<Gio::ListStore<Page>> m_pages;
    Glib::RefPtr<Page> m_removedPage;
};

class RemovePagesCommand : public Command {
public:
    RemovePagesCommand(Glib::RefPtr<Gio::ListStore<Page>> pages,
                       const std::vector<unsigned int>& listPositions);

    void execute() override;
    void undo() override;
    void redo() override;

private:
    Glib::RefPtr<Gio::ListStore<Page>> m_pages;
    const std::vector<unsigned int> m_listPositions;
    std::vector<Glib::RefPtr<Page>> m_removedPages;
};

class RemovePageRangeCommand : public Command {
public:
    RemovePageRangeCommand(Glib::RefPtr<Gio::ListStore<Page>> pages,
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

} // namespace Slicer

#endif // COMMAND_HPP
