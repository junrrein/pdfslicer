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
