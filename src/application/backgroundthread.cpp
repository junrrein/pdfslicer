#include "backgroundthread.hpp"

namespace Slicer {

static const int numThreads = 1;

BackgroundThread::BackgroundThread()
{
    m_threadPool = std::make_unique<ctpl::thread_pool>(numThreads);
}

BackgroundThread::~BackgroundThread()
{
    m_threadPool->stop(false);
}

void BackgroundThread::push(const std::function<void()>& task)
{
	m_threadPool->push([task](int) {
		task();
	});
}

void BackgroundThread::killRemainingTasks()
{
	m_threadPool->stop(false);
	m_threadPool = std::make_unique<ctpl::thread_pool>(numThreads);
}

} // namespace Slicer
