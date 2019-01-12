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

#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "document.hpp"

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
    RemovePageCommand(Document& document,
                      unsigned int position);

    void execute() override;
    void undo() override;
    void redo() override;

private:
    Document& m_document;
    const unsigned int m_position;
    Glib::RefPtr<Page> m_removedPage;
};

class RemovePagesCommand : public Command {
public:
    RemovePagesCommand(Document& document,
                       const std::vector<unsigned int>& listPositions);

    void execute() override;
    void undo() override;
    void redo() override;

private:
    Document& m_document;
    const std::vector<unsigned int> m_listPositions;
    std::vector<Glib::RefPtr<Page>> m_removedPages;
};

class RemovePageRangeCommand : public Command {
public:
    RemovePageRangeCommand(Document& document,
                           unsigned int first,
                           unsigned int last);

    void execute() override;
    void undo() override;
    void redo() override;

private:
    Document& m_document;
    std::vector<Glib::RefPtr<Page>> m_removedPages;
    const unsigned int m_first, m_last;
};

class RotatePagesRightCommand : public Command {
public:
    RotatePagesRightCommand(Document& document,
                            const std::vector<unsigned int>& pageNumbers);

    void execute() override;
    void undo() override;
    void redo() override;

private:
    Document& m_document;
    const std::vector<unsigned int> m_pageNumbers;
};

class RotatePagesLeftCommand : public Command {
public:
    RotatePagesLeftCommand(Document& document,
                           const std::vector<unsigned int>& pageNumbers);

    void execute() override;
    void undo() override;
    void redo() override;

private:
    Document& m_document;
    const std::vector<unsigned int> m_pageNumbers;
};

class MovePageCommand : public Command {
public:
    MovePageCommand(Document& document,
                    unsigned int indexToMove,
                    unsigned int indexDestination);

    void execute() override;
    void undo() override;
    void redo() override;

private:
    Document& m_document;
    const unsigned int m_indexToMove;
    const unsigned int m_indexDestination;
};

class MovePageRangeCommand : public Command {
public:
    MovePageRangeCommand(Document& document,
                         unsigned int indexFirst,
                         unsigned int indexLast,
                         unsigned int indexDestination);

    void execute() override;
    void undo() override;
    void redo() override;

private:
    Document& m_document;
    const unsigned int m_indexFirst;
    const unsigned int m_indexLast;
    const unsigned int m_indexDestination;
};

} // namespace Slicer

#endif // COMMAND_HPP
