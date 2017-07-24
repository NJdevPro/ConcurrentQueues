#ifndef _CONCURRENTQUEUE_H_
# define _CONCURRENTQUEUE_H_

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

/*
* Thread safe unbounded concurrent queue
* It's a FIFO: items are popped out in the order they've been pushed in.
* The queue length is limited by available memory only.
* For a bounded queue, use ConcurrentBlockingQueue.
*/
template <typename T>
class ConcurrentQueue
{
public:

	T pop()
	{
		std::unique_lock<std::mutex> mlock(mutex_);
		while (queue_.empty())
		{
			queue_empty_.wait(mlock);	// block
		}
		auto item = queue_.front();
		queue_.pop();
		return item;
	}

	void pop(T& item)
	{
		std::unique_lock<std::mutex> mlock(mutex_);
		while (queue_.empty())
		{
			queue_empty_.wait(mlock);	// block
		}
		item = queue_.front();
		queue_.pop();
	}

	void push(const T& item)
	{
		{
			std::unique_lock<std::mutex> mlock(mutex_);
			queue_.push(item);
		}
		queue_empty_.notify_one();
	}

	void push(T&& item)
	{
		{
			std::unique_lock<std::mutex> mlock(mutex_);
			queue_.push(std::move(item));
		}
		queue_empty_.notify_one();
	}

	inline size_t size() 
	{
		std::unique_lock<std::mutex> mlock(mutex_);
		return queue_.size();
	}

	inline bool empty() 
	{
		std::unique_lock<std::mutex> mlock(mutex_);
		return queue_.empty();
	}

private:
	std::queue<T> queue_;
	std::mutex mutex_;
	std::condition_variable queue_empty_;	// blocks when the queue is empty
};


#endif /* !_CONCURRENTQUEUE_H_ */

