// PDF Slicer
// Copyright (C) 2019 Julián Unrrein

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

#ifndef SLICER_TASK_HPP
#define SLICER_TASK_HPP

#include <atomic>
#include <memory>
#include "pagerenderer.hpp"

namespace Slicer {

class Task {
public:
    [[nodiscard]] bool isCanceled() const;

	void cancel();
    virtual void execute() = 0;
    virtual void postExecute(){};

    virtual ~Task(){};

private:
	std::atomic_bool m_isCanceled = false;
};

template <typename T>
class RenderTask : public Task {
public:
    RenderTask(std::weak_ptr<T> weakWidget,
               int targetSize)
        : m_weakWidget{weakWidget}
        , m_targetSize{targetSize}
    {
    }

    void execute() override
    {
        auto widget = m_weakWidget.lock();

        if (widget == nullptr)
            return;

        m_renderedPage = PageRenderer{widget->page()}.render(m_targetSize);
    }

    void postExecute() override
    {
        auto widget = m_weakWidget.lock();

        if (widget == nullptr)
            return;

        widget->setImage(m_renderedPage);
        widget->showPage();
    }

    ~RenderTask() override{};

private:
    std::weak_ptr<T> m_weakWidget;
    const int m_targetSize;
    Glib::RefPtr<Gdk::Pixbuf> m_renderedPage;
};

} // namespace Slicer

#endif // SLICER_TASK_HPP
