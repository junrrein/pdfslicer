#ifndef DOCUMENT_HPP
#define DOCUMENT_HPP

#include "commandmanager.hpp"

namespace Slicer {

class Document {
public:
    Document(std::string filePath);
    ~Document();

    void saveDocument(const std::string& destinationPath) const;
    void removePage(int pageNumber);
    void removePages(const std::vector<unsigned int>& positions);
    void removePageRange(int first, int last);
    void undoCommand() { m_commandManager.undo(); }
    void redoCommand() { m_commandManager.redo(); }

    bool canUndo() const { return m_commandManager.canUndo(); }
    bool canRedo() const { return m_commandManager.canRedo(); }
    const Glib::RefPtr<Gio::ListStore<Page>>& pages() const { return m_pages; }

    sigc::signal<void>& commandExecuted() { return m_commandManager.commandExecuted(); }

private:
    PopplerDocument* m_popplerDocument;
    std::string m_sourcePath;
    Glib::RefPtr<Gio::ListStore<Page>> m_pages;
    CommandManager m_commandManager;
};
}

#endif // DOCUMENT_HPP
