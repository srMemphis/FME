#include "pch.h"

#include "core/threads/TPool.h"

namespace FME
{

	TPool::TPool()
		: m_stop(true), m_terminated(true)
	{
	}

	void TPool::Init(size_t nThreads)
	{
		FME_PROFILER_FUNC();

		m_terminated = false;

		m_workers.reserve(nThreads);
		for (int i = 0; i < nThreads; i++)
		{
			m_workers.emplace_back(std::thread(std::bind(&TPool::Work, this)));
		}

		m_stop = false;
	}

	TPool::~TPool()
	{
		if (!m_terminated)
			Terminate();
	}

	void TPool::Terminate()
	{
		FME_PROFILER_FUNC();
		// stop thread pool, and notify all threads to finish the remained tasks
		{
			std::unique_lock<std::mutex> lock(m_mtx);
			m_stop = true;
		}
		m_cv.notify_all();
		for (auto& worker : m_workers)
			worker.join();

		m_workers.clear();
		m_terminated = true;
	}
	
	uint32_t TPool::GetHardwareConcurrency()
	{
		return std::thread::hardware_concurrency();
	}

	void TPool::Work()
	{
		while (true)
		{
			std::function<void()> task;
			// pop a task from queue, and execute it
			{
				std::unique_lock<std::mutex> lock(m_mtx);
				m_cv.wait(lock, [this]() { return m_stop || !m_jobs.empty(); });
				if (m_stop && m_jobs.empty())
					return;

				// even if stop = true excucte until tasks queue becomes empty
				task = std::move(m_jobs.front());
				m_jobs.pop();
			}
			task();
		}
	}
}
