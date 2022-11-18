#pragma once
#include <sstream>
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

    explicit ConcurrentQueue() = default;
    ~ConcurrentQueue() = default;

    explicit ConcurrentQueue<T>(const ConcurrentQueue<T>& rhs) noexcept {
        std::lock_guard<std::mutex> mlock(mutex_);
        queue_ = rhs.queue_;
    }

    ConcurrentQueue<T>& operator=(const ConcurrentQueue<T>& rhs) noexcept {
        std::lock_guard<std::mutex> mlock(mutex_);
        queue_ = rhs.queue_;
        return *this;
    }

    explicit ConcurrentQueue<T>(ConcurrentQueue<T>&& rhs) noexcept {
        std::lock_guard<std::mutex> mlock(mutex_);
        queue_ = std::move(rhs.queue_);
    }

    ConcurrentQueue<T>& operator=(ConcurrentQueue<T>&& rhs) noexcept {
        std::lock_guard<std::mutex> mlock(mutex_);
        queue_ = std::move(rhs.queue_);
        return *this;
    }

    T pop() {
		std::unique_lock<std::mutex> mlock(mutex_);
		while (queue_.empty()) {
			queue_empty_.wait(mlock);	// block
		}
		auto item = queue_.front();
		queue_.pop();
		return item;
	}

	void push(const T&& item) noexcept {
		{
			std::lock_guard<std::mutex> mlock(mutex_);
			queue_.push(std::move(item));
		}
		queue_empty_.notify_one();
	}

	inline size_t size() noexcept {
		std::lock_guard<std::mutex> mlock(mutex_);
		return queue_.size();
	}

	inline bool empty() noexcept {
		std::lock_guard<std::mutex> mlock(mutex_);
		return queue_.empty();
	}

private:
	std::queue<T> queue_;
	std::mutex mutex_;
	std::condition_variable queue_empty_;	// blocks when the queue is empty
};
