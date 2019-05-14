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

#include "command.hpp"

namespace Slicer {

RemovePageCommand::RemovePageCommand(Document& document,
                                     unsigned int position)
    : m_document{document}
    , m_position{position}
{
}

void RemovePageCommand::execute()
{
    m_removedPage = m_document.removePage(m_position);
}

void RemovePageCommand::undo()
{
    m_document.insertPage(m_removedPage);
}

void RemovePageCommand::redo()
{
    m_document.removePage(m_position);
}

RemovePagesCommand::RemovePagesCommand(Document& document,
                                       const std::vector<unsigned int>& listPositions)
    : m_document{document}
    , m_listPositions{listPositions}
{
}

void RemovePagesCommand::execute()
{
    m_removedPages = m_document.removePages(m_listPositions);
}

void RemovePagesCommand::undo()
{
    m_document.insertPages(m_removedPages);
}

void RemovePagesCommand::redo()
{
    m_document.removePages(m_listPositions);
}

RemovePageRangeCommand::RemovePageRangeCommand(Document& document,
                                               unsigned int first,
                                               unsigned int last)
    : m_document{document}
    , m_first{first}
    , m_last{last}
{
}

void RemovePageRangeCommand::execute()
{
    m_removedPages = m_document.removePageRange(m_first, m_last);
}

void RemovePageRangeCommand::undo()
{
    m_document.insertPageRange(m_removedPages, m_first);
}

void RemovePageRangeCommand::redo()
{
    m_document.removePageRange(m_first, m_last);
}

RotatePagesRightCommand::RotatePagesRightCommand(Document& document,
                                                 const std::vector<unsigned int>& pageNumbers)
    : m_document{document}
    , m_pageNumbers{pageNumbers}
{
}

void RotatePagesRightCommand::execute()
{
    m_document.rotatePagesRight(m_pageNumbers);
}

void RotatePagesRightCommand::undo()
{
    m_document.rotatePagesLeft(m_pageNumbers);
}

void RotatePagesRightCommand::redo()
{
    execute();
}

RotatePagesLeftCommand::RotatePagesLeftCommand(Document& document,
                                               const std::vector<unsigned int>& pageNumbers)
    : m_document{document}
    , m_pageNumbers{pageNumbers}
{
}

void RotatePagesLeftCommand::execute()
{
    m_document.rotatePagesLeft(m_pageNumbers);
}

void RotatePagesLeftCommand::undo()
{
    m_document.rotatePagesRight(m_pageNumbers);
}

void RotatePagesLeftCommand::redo()
{
    execute();
}

MovePageCommand::MovePageCommand(Document& document,
                                 unsigned int indexToMove,
                                 unsigned int indexDestination)
    : m_document{document}
    , m_indexToMove{indexToMove}
    , m_indexDestination{indexDestination}
{
}

void MovePageCommand::execute()
{
    m_document.movePage(m_indexToMove, m_indexDestination);
}

void MovePageCommand::undo()
{
    m_document.movePage(m_indexDestination, m_indexToMove);
}

void MovePageCommand::redo()
{
    execute();
}

MovePageRangeCommand::MovePageRangeCommand(Document& document,
                                           unsigned int indexFirst,
                                           unsigned int indexLast,
                                           unsigned int indexDestination)
    : m_document{document}
    , m_indexFirst{indexFirst}
    , m_indexLast{indexLast}
    , m_indexDestination{indexDestination}
{
}

void MovePageRangeCommand::execute()
{
    m_document.movePageRange(m_indexFirst, m_indexLast, m_indexDestination);
}

void MovePageRangeCommand::undo()
{
    const unsigned int numberOfPages = m_indexLast - m_indexFirst + 1;

    m_document.movePageRange(m_indexDestination,
                             m_indexDestination + numberOfPages - 1,
                             m_indexFirst);
}

void MovePageRangeCommand::redo()
{
    execute();
}

AddDocumentCommand::AddDocumentCommand(Document& document,
                               const Glib::RefPtr<Gio::File>& file,
                               unsigned int position)
    : m_document{document}
    , m_file{file}
    , m_position{position}
{
}

void AddDocumentCommand::execute()
{
    m_numberOfAddedPages = m_document.addFile(m_file, m_position);
}

void AddDocumentCommand::undo()
{
    m_addedPages = m_document.removePageRange(m_position, m_position + m_numberOfAddedPages - 1);
}

void AddDocumentCommand::redo()
{
    m_document.insertPageRange(m_addedPages, m_position);
}

} // namespace Slicer
