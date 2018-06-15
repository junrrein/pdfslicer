#ifndef BACKGROUNDTHREAD_HPP
#define BACKGROUNDTHREAD_HPP

#include <ctpl_stl.h>

namespace Slicer {

class BackgroundThread
{
public:
    BackgroundThread();

	void push(const std::function<void()>& task);
	void killRemainingTasks();

private:
	std::unique_ptr<ctpl::thread_pool> m_threadPool;
};

} // namespace Slicer

#endif // BACKGROUNDTHREAD_HPP
