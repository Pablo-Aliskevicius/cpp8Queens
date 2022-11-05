#pragma once

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

/// <summary>
/// Queue of pointers to Whatever. Usually self-contained tasks.
/// </summary>
/// <typeparam name="T"></typeparam>
template <typename T>
class ThreadSafeQueue
{
	std::queue<T *> m_q;
	std::mutex m_mutex;
	std::condition_variable m_cv;

public: // Could use the Attorney idiom to make these visible to ThreadLoop only, but life is short.
	ThreadSafeQueue() = default;
	~ThreadSafeQueue() = default;
	ThreadSafeQueue(ThreadSafeQueue&& source) = delete;
	ThreadSafeQueue(const ThreadSafeQueue& source) = delete;
	ThreadSafeQueue& operator = (const ThreadSafeQueue& source) = delete;

	void push(T* pt)
	{
		std::lock_guard<std::mutex> guard(m_mutex);
		m_q.push(pt);
		m_cv.notify_one();
	}
	T* pop()
	{
		std::unique_lock<std::mutex> lk(m_mutex);
		m_cv.wait(lk, [this] { return !m_q.empty(); });
		T* ret = m_q.front();
		m_q.pop();
		return ret;
	}
}; // class ThreadSafeQueue

/// <summary>
/// The thread loop waits for the queue to become signaled.
/// If it contains a task, the task is executed.
/// If it contains null, the thread loop exits.
/// </summary>
/// <typeparam name="T">Functor, that defines operator () ()</typeparam>
template <typename T>
class ThreadLoop
{
	ThreadSafeQueue<T> m_queue; // should we have a queue per thread?
	std::atomic<bool> m_is_done = false;

public:
	ThreadLoop() = default;
	~ThreadLoop() = default;

	ThreadLoop(ThreadLoop&& source) = delete;
	ThreadLoop(const ThreadLoop& that) = delete;
	ThreadLoop& operator = (const ThreadLoop& that) = delete;


	void operator () ()
	{
		while (T* pFunc = m_queue.pop())
		{
			(*pFunc)();
		}
		m_is_done = true;
	}
	void push(T* t)
	{
		m_queue.push(t);
	}
	void join()
	{
		m_queue.push(nullptr);
		while (!m_is_done)
		{
			std::this_thread::yield();
		}
	}
}; // class ThreadLoop


/// <summary>
/// The actual thread pool. 
/// To minimize locking, each thread has a separate queue. 
/// </summary>
/// <typeparam name="T"></typeparam>
template <typename T>
class ThreadPool
{
	template <typename T>
	class Impl
	{
		std::vector<std::thread> m_threads;
		std::vector<ThreadLoop<T> *> m_loops;
		int current_thread = -1;

	public:
		Impl()
		{
			// Just member initialization.
			int n_threads = (int)std::thread::hardware_concurrency();
			//std::cout << n_threads << " concurrent threads are supported." << std::endl;
			if (n_threads < 2)
			{
				throw std::exception("Threads not supported");
			}
			m_threads.reserve(n_threads);
			m_loops.reserve(n_threads);
			for (int i = 0; i < n_threads; ++i)
			{
				auto *pLoop = new ThreadLoop<T>();
				m_loops.push_back(pLoop);
				std::thread t(std::ref(* pLoop));
				m_threads.push_back(std::move(t));
			}
		}
		~Impl()
		{
			for (auto& t : m_threads)
			{
				t.join();
			}
			for (auto* ptl : m_loops)
			{
				delete ptl;
			}
		}
		Impl(const Impl& that) = delete;
		Impl(Impl&& that) = delete;
		Impl& operator = (const Impl& that) = delete;

		size_t GetThreadsCount() const
		{
			return m_threads.size();
		}
		void join_all()
		{
			// TODO: Send null to all, then wait for all. 
			for (auto * pl : m_loops)
			{
				pl->join();
			}
		}
		void push(T* pt)
		{
			if (++current_thread >= m_loops.size())
			{
				current_thread = 0;
			}
			m_loops[current_thread]->push(pt);
		}
	};
	std::unique_ptr<Impl<T>> m_pImpl;
public:
	ThreadPool()
		: m_pImpl(new Impl<T>())
	{
		
	}
	~ThreadPool() = default;

	// Disable copying.
	ThreadPool(const ThreadPool& that) = delete;
	ThreadPool(ThreadPool&& that) = delete;
	bool operator == (const ThreadPool& that) = delete;


	void push(T* pt)
	{
		m_pImpl->push(pt);
	}

	void wait_all()
	{
		m_pImpl->join_all();
	}
};