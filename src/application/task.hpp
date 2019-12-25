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
#include <functional>

namespace Slicer {

class Task {
public:
	Task(const std::function<void()>& funcExecute,
		 const std::function<void()>& funcPostExecute);

	bool isCanceled() const;

	void cancel();
	void execute();
	void postExecute();

private:
	std::atomic_bool m_isCanceled = false;
	std::function<void()> m_funcExecute;
	std::function<void()> m_funcPostExecute;
};

} // namespace Slicer

#endif // SLICER_TASK_HPP
