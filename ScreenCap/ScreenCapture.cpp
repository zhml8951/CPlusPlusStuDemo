#include "pch.h"
#include "ScreenCapture.h"

namespace sc
{
	bool IsMonitorInSideBounds(const std::vector<Monitor>& monitors, const Monitor& monitor)
	{
		auto total_width = 0;
		for (auto& m : monitors) {
			total_width += Width(m);
		}
		return false;
	}


	static bool screen_capture_manager_exists = false;


	class ScreenCaptureManager : public IScreenCaptureManager
	{
	public:
		std::shared_ptr<ThreadData> thread_data_;
		std::thread thread_;

		ScreenCaptureManager()
		{
			using namespace std::chrono;
			assert(!screen_capture_manager_exists);
			screen_capture_manager_exists =  true;
			thread_data_ = std::make_shared<ThreadData>();
			thread_data_->common_data.paused = false;
			thread_data_->screen_capture_data.frame_timer = std::make_shared<Timer>(100ms);
			thread_data_->screen_capture_data.mouse_timer = std::make_shared<Timer>(50ms);
			thread_data_->window_capture_data.frame_timer = std::make_shared<Timer>(100ms);
			thread_data_->window_capture_data.mouse_timer = std::make_shared<Timer>(50ms);
		}

		virtual ~ScreenCaptureManager()
		{
			thread_data_->common_data.terminate_thread_event = true;
			thread_data_->common_data.paused = false;
			if(thread_.get_id() == std::this_thread::get_id()) {
				thread_.detach();
			}
			else if (thread_.joinable()) {
				thread_.join();
			}
			screen_capture_manager_exists = false;
		}

		void Start()
		{
			thread_ = std::thread([&]()->void {
				// TODO ThreadManager.  module are not exist. to finish ThreadManager;
				//ThreadMana
			});
		}

		virtual void SetFrameChangeInterval(const std::shared_ptr<Timer>& timer) override { }

		virtual void SetMouseChangeInterval(const std::shared_ptr<Timer>& timer) override { }

		virtual void Pause() override { }

		virtual bool IsPaused() const override { }

		virtual void Resume() override { }
	};

	class ScreenCaptureConfiguration : public ICaptureConfiguration<ScreenCaptureCallback>
	{
	public:
		explicit ScreenCaptureConfiguration(const std::shared_ptr<ScreenCaptureManager>& impl): impl_(impl) {}

		virtual std::shared_ptr<ICaptureConfiguration<ScreenCaptureCallback>> OnNewFrame(
			const ScreenCaptureCallback& cb) override { }

		virtual std::shared_ptr<ICaptureConfiguration<ScreenCaptureCallback>> OnFrameChanged(
			const ScreenCaptureCallback& cb) override { }

		virtual std::shared_ptr<ICaptureConfiguration<ScreenCaptureCallback>> OnMouseChanged(const MouseCallback& cb)
		override { }

		virtual std::shared_ptr<IScreenCaptureManager> StartCapturing() override { }
	private:
		std::shared_ptr<ScreenCaptureManager> impl_;
	};

	class WindowCaptureConfiguration : public ICaptureConfiguration<WindowCaptureCallback>
	{
	public:
		explicit WindowCaptureConfiguration(const std::shared_ptr<ScreenCaptureManager>& impl): impl_(impl) {}

		virtual std::shared_ptr<ICaptureConfiguration<WindowCaptureCallback>> OnNewFrame(
			const WindowCaptureCallback& cb) override { }

		virtual std::shared_ptr<ICaptureConfiguration<WindowCaptureCallback>> OnFrameChanged(
			const WindowCaptureCallback& cb) override { }

		virtual std::shared_ptr<ICaptureConfiguration<WindowCaptureCallback>> OnMouseChanged(
			const WindowCaptureCallback& cb) override { }

		virtual std::shared_ptr<IScreenCaptureManager> StartCapturing() override { }

	private:
		std::shared_ptr<ScreenCaptureManager> impl_;
	};


		std::shared_ptr<ICaptureConfiguration<ScreenCaptureCallback>> CreateCaptureConfiguration(
		const MonitorCallback& monitor_to_capture)
	{
		return std::shared_ptr<ICaptureConfiguration<ScreenCaptureCallback>>();
	}

	//auto CreateCaptureConfiguration(const WindowCallback& window_to_capture) ->
	//std::shared_ptr<ICaptureConfiguration<WindowCaptureCallback>>
	//{
	//	return std::shared_ptr<ICaptureConfiguration<WindowCaptureCallback>>();
	//}
}
