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

#include "taskrunner.hpp"
#include <glibmm/main.h>

namespace Slicer {

static const int numThreads = 1;

TaskRunner::TaskRunner()
    : m_threadpool{numThreads}
{
}

TaskRunner::~TaskRunner()
{
    m_threadpool.wait();
}

void TaskRunner::queueBack(const std::shared_ptr<Task>& task)
{
    m_threadpool.push([this, task]() {
        runTask(task);
    });
}

void TaskRunner::queueFront(const std::shared_ptr<Task>& task)
{
    std::thread{[this, task]() {
        m_threadpool.apply_for(1, [this, task]() {
            runTask(task);
        });
    }}
        .detach();
}

void TaskRunner::runTask(const std::shared_ptr<Task>& task)
{
    if (task->isCanceled())
        return;

    task->execute();

    if (task->isCanceled())
        return;

    Glib::signal_idle().connect([task]() {
        if (!task->isCanceled())
            task->postExecute();

        return false;
    });
}

} // namespace Slicer
