#include "pch.h"

#include "core/app/App.h"

namespace FME
{
	App* App::s_Instance = nullptr;

	App::App()
	{
		// There should be only one App
		FME_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		// everithing goes into scope so all of the timers will be destroyd before session ends
		FME_PROFILER_BEGIN_SESSION("startProfile.json");
		{
			FME_PROFILER_SCOPE("app init");

			// Init stuff
			FME::FMELog::Init("FME_LOG.log");

			m_threadPool.Init(4);
		}
		FME_PROFILER_END_SESSION();
	}

	App::~App()
	{
		FME_PROFILER_BEGIN_SESSION("endProfile.json");
		{
			m_threadPool.Terminate();
		}
		FME_PROFILER_END_SESSION();
	}

	void App::Run()
	{
		FME_PROFILER_BEGIN_SESSION("runProfile.json");
		{
			FME_PROFILER_FUNC();


			std::vector<std::future<void>> govno;
			for (int i = 0; i < 10; i++)
			{
				govno.push_back( m_threadPool.AddJob([]
					{	
						FME_PROFILER_SCOPE("thread task");
						FME_LOG_INFO("Sup bruh!");
						std::this_thread::sleep_for(std::chrono::milliseconds(500));
					}));
			}

			for (int i = 0; i < 10; i++)
			{
				govno[i].wait();
			}

		}
		FME_PROFILER_END_SESSION();
	}
	App& App::GetInstance()
	{
		return *s_Instance;
	}
}
