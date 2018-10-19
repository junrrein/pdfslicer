#include "backgroundthread.hpp"

namespace Slicer {

static const int numThreads = 1;

BackgroundThread::BackgroundThread()
{
    m_threadpool = std::make_unique<astp::ThreadPool>(numThreads);
}

void BackgroundThread::pushBack(const std::function<void()>& task)
{
    m_threadpool->push([task]() {
		task();
	});
}

void BackgroundThread::killRemainingTasks()
{
    m_threadpool->stop();
    m_threadpool = std::make_unique<astp::ThreadPool>(numThreads);
}

} // namespace Slicer
