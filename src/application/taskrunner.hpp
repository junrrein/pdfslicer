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

#ifndef SLICER_TASKRUNNER_HPP
#define SLICER_TASKRUNNER_HPP

#include "task.hpp"
#include <threadpool.hpp>

namespace Slicer {

class TaskRunner {
public:
	TaskRunner();

    TaskRunner(const TaskRunner&) = delete;
    TaskRunner& operator=(const TaskRunner&) = delete;
    TaskRunner(TaskRunner&&) = delete;
    TaskRunner& operator=(TaskRunner&& src) = delete;

	~TaskRunner();

	void queueBack(const std::shared_ptr<Task>& task);
	void queueFront(const std::shared_ptr<Task>& task);

private:
    static void runTask(const std::shared_ptr<Task>& task);

	astp::ThreadPool m_threadpool;
};

} // namespace Slicer

#endif // SLICER_TASKRUNNER_HPP
