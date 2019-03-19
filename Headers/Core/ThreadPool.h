/*
	Based on: https://github.com/mtrebi/thread-pool
.*/

#pragma once
#include "JobQueue.h"
#include <vector>
#include <thread>
#include <mutex>
#include <future>

class ThreadPool
{
private:
	static std::vector<std::thread> pool;
	static int threadsInPool;

	static JobQueue < std::function<void()>> jobs;

	static std::condition_variable condition;
	static std::mutex access_mutex;

	static bool shutdown;
public:
	static void setThreadCount(int threadCount);

	template<typename Func, typename...Args>
	static auto submitJob(Func&& f, Args&&... args)->std::future<decltype(f(args...))>;
	static void work();
	static void workToComplete();

	static void cleanup();
	static void joinThreads();
};

template<typename Func, typename ...Args>
inline auto ThreadPool::submitJob(Func && f, Args && ...args) -> std::future<decltype(f(args ...))>
{
	std::function<decltype(f(args...))()> func = std::bind(std::forward<Func>(f), std::forward<Args>(args)...);
	auto task_ptr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);
	std::function<void()> wrapper_func = [task_ptr]() {
		(*task_ptr)();
	};
	jobs.push(wrapper_func);
	condition.notify_one();

	return task_ptr->get_future();
}