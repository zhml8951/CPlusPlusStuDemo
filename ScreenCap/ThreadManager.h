#pragma once
#include "ScreenCapture.h"


namespace sc
{
	class ThreadManager
	{
		std::vector<std::thread> thread_handles_;
		std::shared_ptr<std::atomic_bool> terminate_threads_event_;

	public:
		ThreadManager();
		~ThreadManager();
		void Init(const std::shared_ptr<ThreadData>& data);
		void Join();
	};
}
