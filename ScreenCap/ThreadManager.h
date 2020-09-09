#pragma once
#include "ScCommon.h"

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

	template <typename T, typename F, typename... E>
	bool TryCaptureMouse(const F& data, E ... args)
	{
		return {};
	}

	void RunCaptureMonitor(std::shared_ptr<ThreadData> data, Monitor monitor);

	void RunCaptureWindow(std::shared_ptr<ThreadData> data, Window window);

	void RunCaptureMouse(std::shared_ptr<ThreadData> data);
}
