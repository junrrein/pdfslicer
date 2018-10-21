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
#include <giomm/file.h>

namespace Slicer {

class Document {
public:
    Document(const Glib::RefPtr<Gio::File>& sourceFile);

    void removePage(int pageNumber);
    void removePages(const std::vector<unsigned int>& positions);
    void removePageRange(int first, int last);
    void rotatePagesRight(const std::vector<unsigned int>& pageNumbers);
    void rotatePagesLeft(const std::vector<unsigned int>& pageNumbers);
    void undoCommand() { m_commandManager.undo(); }
    void redoCommand() { m_commandManager.redo(); }

    bool canUndo() const { return m_commandManager.canUndo(); }
    bool canRedo() const { return m_commandManager.canRedo(); }
    const Glib::RefPtr<Gio::ListStore<Page>>& pages() const { return m_pages; }
    std::string basename() const { return m_basename; }
    std::string filePath() const { return m_sourceFile->get_path(); }

    sigc::signal<void>& commandExecuted() { return m_commandManager.commandExecuted; }
    sigc::signal<void, std::vector<unsigned int>> pagesRotated;

private:
    using PopplerDocumentPointer = std::unique_ptr<PopplerDocument, decltype(&g_object_unref)>;

    PopplerDocumentPointer m_popplerDocument;
    Glib::RefPtr<Gio::File> m_sourceFile;
    Glib::RefPtr<Gio::ListStore<Page>> m_pages;
    CommandManager m_commandManager;
    std::string m_basename;

    void loadDocument();
};
}

#endif // DOCUMENT_HPP
