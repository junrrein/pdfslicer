#ifndef DOCUMENT_HPP
#define DOCUMENT_HPP

#include "page.hpp"
#include "commandmanager.hpp"
#include <giomm/liststore.h>
#include <string>

namespace Slicer {

class Document {
public:
    // Constructor and destructor
    Document(std::string filePath);

    // Interface
    void saveDocument(std::string filePath) const;
    bool canUndo() const;
    bool canRedo() const;

    void removePage(int pageNumber);
    void removePageRange(int first, int last);
    void undoCommand();
    void redoCommand();

    // Member element access
    const Glib::RefPtr<Gio::ListStore<Page>>& pages() const { return m_pages; };
    sigc::signal<void>& commandExecuted() { return m_commandManager.commandExecuted(); };

private:
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

    // Member objects
    std::string m_sourcePath;
    Glib::RefPtr<Gio::ListStore<Page>> m_pages;
    CommandManager m_commandManager;
};
}

#endif // DOCUMENT_HPP
