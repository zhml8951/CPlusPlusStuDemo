#include "pch.h"
#include "ThreadManager.h"
#include "ScreenCapture.h"


sc::ThreadManager::ThreadManager() {}

sc::ThreadManager::~ThreadManager() { Join(); }

void sc::ThreadManager::Init(const std::shared_ptr<ThreadData>& data)
{
	assert(this->thread_handles_.empty());
	if (data->screen_capture_data.get_things_to_watch) {
		auto monitors = data->screen_capture_data.get_things_to_watch();
		auto mons = GetMonitors();
		for (auto& m : monitors) {
			assert(IsMonitorInSideBounds(mons, m));
		}

		this->thread_handles_.resize(monitors.size() + (data->screen_capture_data.on_mouse_changed ? 1 : 0));
		for (size_t i = 0; i < monitors.size(); ++i) {
			thread_handles_[i] = std::thread(&sc::RunCaptureMonitor, data, monitors[i]);
		}

		if (data->screen_capture_data.on_mouse_changed) {
			thread_handles_.back() = std::thread([data] {
				sc::RunCaptureMouse(data);
			});
		}
	}
	else if (data->window_capture_data.get_things_to_watch) {
		auto windows = data->window_capture_data.get_things_to_watch();
		this->thread_handles_.resize(windows.size() + (data->window_capture_data.on_mouse_changed ? 1 : 0));
		for(size_t i = 0; i < windows.size(); ++i) {
			thread_handles_[i] = std::thread(&sc::RunCaptureWindow, data, windows[i]);
		}
		if(data->window_capture_data.on_mouse_changed) {
			thread_handles_.back() = std::thread([data] {
				sc::RunCaptureMouse(data);
			});
		}
	}
}

void sc::ThreadManager::Join() {}
