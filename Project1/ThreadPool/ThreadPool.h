#pragma once
#include <iostream>
#include <queue>
#include <vector>
#include <mutex>
#include <future>
#include "./Thread.h"

class ThreadPool
{
public:
	using Task = std::function<void()>;

	ThreadPool(int num_threads);
	~ThreadPool();

	void addTask(Task task);
	void wait();
	int getThreadsCount();

private:
	std::vector<Task> tasks;
	std::vector<Thread*> threads;

	std::condition_variable cv;
	std::condition_variable cv_wait;

	std::mutex queue_mutex;
	std::mutex completed_tasks_mutex;
	std::mutex added_tasks_mutex;

	std::atomic<int> added_tasks = 0;
	std::atomic<int> completed_tasks = 0;

	void run(Thread *thread);
	bool isCompleted();
};

