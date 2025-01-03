#pragma once

#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>

namespace FME
{
	class TPool
	{
	public:

		TPool();
		void Init(size_t nThreads);
		~TPool();

		void Terminate();

		static uint32_t GetHardwareConcurrency();

		template <typename F, typename... Args, typename R = std::invoke_result_t<std::decay_t<F>, std::decay_t<Args>...>>
		std::future<R> AddJob(F&& f, Args&&... args)
		{
			using return_t = decltype(f(args...));
			using future_t = std::future<return_t>;
			using task_t = std::packaged_task<return_t()>;

			if (m_stop)
				throw std::runtime_error("The thread pool has been stopped.");

			auto bind_func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
			std::shared_ptr<task_t> job = std::make_shared<task_t>(std::move(bind_func));
			future_t fut = job->get_future();

			std::unique_lock<std::mutex> lock(m_mtx);
			m_jobs.emplace([job]() -> void { (*job)(); });
			lock.unlock();

			m_cv.notify_one();
			return fut;
		}

		TPool(const TPool&) = delete;
		TPool(TPool&&) = delete;
		TPool& operator=(const TPool&) = delete;
		TPool& operator=(TPool&&) = delete;

	private:
		void Work();
	private:
		std::vector<std::thread> m_workers;
		std::queue<std::function<void()>> m_jobs;

		std::mutex m_mtx;
		std::condition_variable m_cv;
		bool m_stop;
		bool m_terminated;
	};
}
