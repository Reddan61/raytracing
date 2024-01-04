#include "ThreadPool.h"

ThreadPool::ThreadPool(int num_threads)
{
	this->threads.reserve(num_threads);

	for (int i = 0; i < num_threads; i++) {
		Thread *thread = new Thread;

		thread->id = i;
		thread->thread = std::thread{ &ThreadPool::run, this, thread };

		this->threads.push_back(thread);
	}
}

ThreadPool::~ThreadPool()
{
}

void ThreadPool::addTask(Task task)
{
	{
		std::unique_lock<std::mutex> lock(this->queue_mutex);
		this->tasks.emplace_back(std::move(task));

		this->added_tasks_mutex.lock();
		++this->added_tasks;
		this->added_tasks_mutex.unlock();
	}

	this->cv.notify_one();
}

void ThreadPool::wait()
{
	std::unique_lock<std::mutex> lock(this->queue_mutex);

	this->cv_wait.wait(lock, [&] {
		return tasks.empty() && isCompleted();
	});
}

int ThreadPool::getThreadsCount()
{
	return this->threads.size();
}

void ThreadPool::run(Thread* thread)
{
	while (true) {
		Task task;
		{
			std::unique_lock<std::mutex> lock(this->queue_mutex);
			
			cv.wait(lock, [&] {
				return !tasks.empty();
			});

			task = std::move(this->tasks.back());
			this->tasks.pop_back();
		}

		task();

		this->completed_tasks_mutex.lock();

		++this->completed_tasks;

		this->completed_tasks_mutex.unlock();

		this->cv_wait.notify_one();
	}
}

bool ThreadPool::isCompleted()
{
	std::unique_lock<std::mutex> lock1(this->added_tasks_mutex);
	std::unique_lock<std::mutex> lock2(this->completed_tasks_mutex);


	return this->added_tasks == this->completed_tasks;
}

