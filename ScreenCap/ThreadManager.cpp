#include "pch.h"
#include "ThreadManager.h"

sc::ThreadManager::ThreadManager() {}
sc::ThreadManager::~ThreadManager() { Join(); }
void sc::ThreadManager::Init(const std::shared_ptr<ThreadData>& data)
{
	assert(this->thread_handles_.empty());
	if(data->screen_capture_data.get_things_to_watch) {
		auto monitors = data->screen_capture_data.get_things_to_watch();
		auto mons = GetMonitors();
		for(auto& m:  monitors) {
			assert(IsMonitorInSideBounds(mons, m));
		}
	}

}
void sc::ThreadManager::Join() {}
