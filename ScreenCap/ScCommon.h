#pragma once

#include <vector>
#include <atomic>
#include <thread>
#include <cassert>
#include <chrono>

#include "ScreenCapture.h"

namespace sc
{
	struct ImageRect
	{
		int left_bottom;
		int left_top;
		int right_top;
		int right_bottom;

		ImageRect() : ImageRect(0, 0, 0, 0) { };

		ImageRect(const int l_b, const int l_t, const int r_t, const int r_b) : left_bottom(l_b), left_top(l_t),
		                                                                        right_top(r_t), right_bottom(r_b) { }

		bool contains(const ImageRect& rect) const
		{
			return left_bottom <= rect.left_bottom && left_top <= rect.left_top && right_top >= rect.right_top &&
				right_bottom >= rect.right_bottom;
		}
	};

	struct Image
	{
		ImageRect bounds;
		int bytes_to_next_row = 0;
		bool is_contiguous = false;
		const ImageBgra* data = nullptr;
	};

	inline bool operator==(const ImageRect& img_rect1, const ImageRect& img_rect2)
	{
		return img_rect1.left_bottom == img_rect2.left_bottom && img_rect1.left_top == img_rect2.left_top &&
			img_rect1.right_top == img_rect2.right_top && img_rect1.right_bottom == img_rect2.right_bottom;
	}

	inline int RowStride(const Image& img) { return sizeof(ImageBgra) * Width(img); }

	inline void Extract(const Image& img, unsigned char* dst, size_t dst_size)
	{
		auto img_size = Width(img) * Height(img) * sizeof(ImageBgra);
		assert(dst_size >= static_cast<size_t>(img_size));
		auto dst_start = dst;
		auto src_start = StartSrc(img);
		if (IsDataContiguous(img)) {
			memcpy(dst_start, src_start, img_size);
		}
		else {
			img_size = sizeof(ImageBgra) * Width(img);
			for (auto i = 0; i < Height(img); i++) {
				memcpy_s(dst_start, img_size, src_start, img_size);
				dst_start += img_size;
				src_start = GotoNextRow(img, src_start);
			}
		}
	}


	using std::chrono::duration;
	using std::chrono::duration_cast;

	class Timer
	{
		using Clock = std::conditional<std::chrono::high_resolution_clock::is_steady,
		                               std::chrono::high_resolution_clock, std::chrono::steady_clock>::type;
		using MicroSec = std::chrono::microseconds;

	public:
		template <typename Rep, typename Period>
		explicit Timer(const duration<Rep, Period>& durations) : duration_(duration_cast<MicroSec>(durations)),
		                                                         dead_line_(Clock::now() + duration_) { }

		void Start() { this->dead_line_ = Clock::now() + duration_; }

		void Wait() const
		{
			const auto now = Clock::now();
			if (now < dead_line_) {
				std::this_thread::sleep_for(dead_line_ - now);
			}
		}

		MicroSec Duration() const { return this->Duration(); }

	private:
		std::chrono::microseconds duration_;
		Clock::time_point dead_line_;
	}; // end class Timer;

	void SanitizeRects(std::vector<ImageRect>& rects, const Image& img);

	inline const ImageRect& Rect(const Image& img) { return img.bounds; }

	template <typename F, typename M, typename W>
	struct CaptureData
	{
		std::shared_ptr<Timer> frame_timer;
		F on_new_frame;
		F on_frame_changed;
		std::shared_ptr<Timer> mouse_timer;
		M on_mouse_changed;
		M get_things_to_watch;
	};

	struct CommonData
	{
		std::atomic<bool> unexpected_error_event;
		std::atomic<bool> expected_error_event;
		std::atomic<bool> terminate_thread_event;
		std::atomic<bool> paused;
	};

	typedef std::function<void(const Image&, const Window&)> WindowCaptureCallback;
	typedef std::function<void(const Image&, const Monitor&)> ScreenCaptureCallback;
	typedef std::function<void(const Image&, const MousePoint&)> MouseCallback;
	typedef std::function<std::vector<Monitor>()> MonitorCallback;
	typedef std::function<std::vector<Window>()> WindowCallback;

	struct ThreadData
	{
		CaptureData<ScreenCaptureCallback, MouseCallback, MonitorCallback> screen_capture_data;
		CaptureData<WindowCaptureCallback, MouseCallback, WindowCallback> window_capture_data;
		CommonData common_data;
	};

	class BaseFrameProcessor
	{
	public:
		std::shared_ptr<ThreadData> data_;
		std::unique_ptr<unsigned char[]> image_buffer_;
		int image_buffer_size_ = 0;
		bool first_run_ = true;
	};

	enum CodeReturn { SUCCESS_CODE = 0, ERROR_EXPECTED_CODE = 1, ERROR_UNEXPECTED_CODE = 2 };

	Monitor CreateMonitor(int index, int id, int h, int w, int ox, int oy, const std::string& n, float scale);
	Monitor CreateMonitor(int index, int id, int adapter, int h,
	                      int w, int ox, int oy, const std::string& n, float scale);

	bool IsMonitorInsideBounds(const std::vector<Monitor>& monitors, const Monitor& monitor);

	Image CreateImage(const ImageRect& img_rect, int row_padding, const ImageBgra* data);

	std::vector<ImageRect> GetDifs(const Image& old_img, const Image& new_img);

	template <typename F, typename T, typename C>
	void ProcessCapture(const F& data, T& base, const C& monitor, const unsigned char* start_src, int src_rows_stride)
	{
		ImageRect image_rect;
		image_rect.left_bottom = 0;
		image_rect.left_top = 0;
		image_rect.right_bottom = Height(monitor);
		image_rect.right_top = Width(monitor);

		const auto size_img_bgra = static_cast<int>(sizeof(ImageBgra));
		const auto start_img_src = reinterpret_cast<const ImageBgra*>(start_src);
		auto dst_row_stride = size_img_bgra * Width(monitor);

		if(data.OnNewFrame) {
			// TODO 自动推导, 这里成员方法怎么确定?......	
		}
	}
}
