#include "Core/ThreadPool.h"

std::vector<std::thread> ThreadPool::pool;
int ThreadPool::threadsInPool = 0;
JobQueue < std::function<void()>> ThreadPool::jobs;
std::condition_variable ThreadPool::condition;
std::mutex ThreadPool::access_mutex;
bool ThreadPool::shutdown;

void ThreadPool::setThreadCount(int threadCount)
{
	pool.resize(threadCount);
	threadsInPool = threadCount;
	for (int i = 0; i < threadCount; i++) {
		pool[i] = std::thread(ThreadPool::work);
	}
}

void ThreadPool::work()
{
	std::function<void()> func;
	bool job;
	while (!shutdown) {
		{
			std::unique_lock<std::mutex> lock(access_mutex);
			if (jobs.isEmpty()) {
				condition.wait(lock);
			}
			job = jobs.getJob(func);
		}
		if (job) {
			func();
		}
	}
}

void ThreadPool::workToComplete()
{
	std::function<void()> func;
	bool job;
	while (!shutdown) {
		{
			std::unique_lock<std::mutex> lock(access_mutex);
			if (jobs.isEmpty()) {
				return;
			}
			job = jobs.getJob(func);
		}
		if (job) {
			func();
		}
	}
}

void ThreadPool::cleanup()
{
	shutdown = true;
	jobs.killQueue();
	condition.notify_all();
	joinThreads();
}

void ThreadPool::joinThreads()
{
	for (int i = 0; i < threadsInPool; i++) {
		pool[i].join();
	}
}