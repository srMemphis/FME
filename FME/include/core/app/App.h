#pragma once

#include "core/threads/TPool.h"

namespace FME
{
	class App
	{
	public:

		App();
		virtual ~App();
		void Run();

		static App& GetInstance();

	protected:
		App(App const&) = delete;
		void operator=(App const&) = delete;

	private:
		static App* s_Instance;
		TPool m_threadPool;
	};

}
