/*
	Job queue for a thread pool to pull from
*/

#pragma once
#include <mutex>
#include <queue>

template <typename Job>
class JobQueue
{
private:
	std::mutex queue_mutex;
	std::queue<Job> queue;
	bool dead = false;
public:
	bool getJob(Job& jobOut) {
		std::unique_lock<std::mutex> lock(queue_mutex);
		if (queue.empty()) {
			return false;
		}
		jobOut = std::move(queue.front());
		queue.pop();
		return true;
	}
	void push(Job job) {
		std::unique_lock<std::mutex> lock(queue_mutex);
		queue.push(std::move(job)); //TODO just job?
	}
	bool isEmpty() {
		std::unique_lock<std::mutex> lock(queue_mutex);
		return queue.empty();
	}
	void clear() {
		std::unique_lock<std::mutex> lock(queue_mutex);
		while (!queue.empty())
		{
			queue.pop();
		}
	}
	void killQueue() { clear(); dead = true; };
	JobQueue() {};
	~JobQueue() { killQueue(); };
};

