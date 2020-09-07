#pragma once
#include <vector>
#include <atomic>
#include <thread>
#include <cassert>
#include <chrono>
#include <memory>
#include <functional>
#include <string>
#include <cstring>

#include "ScreenCapture.h"

namespace sc
{
	/*
	 *		绘图时左下角为原点(0,0), 而显示器里表示图像时的座标为:左上角为(0，0)
	 *			       top=y1
	 *	  (x1,y1)-------------------(x2,y1)		      (0.0)---------------> X轴
	 *			|					|                     |
	 *			|					|                     |
	 *  left=x1 |					| right=x2            |
	 *			|					|					  |
	 *			|					|					  v
	 *	  (x1,y2)-------------------(x2,y2)              Y轴
	 *			      bottom=y2
	 *
	 *	定义ImageRect时， left时X轴不变, Y轴变化，故left相当于左边的x轴值(x1)，同理top=y1, right=x2, bottom=y2;
	 *	使用left,top,right,bottom,容易误解成边长， 正确理解应为left表示左边轴位置，right左边轴位置...
	 *	后续height=top-bottom, width=right-left就好理解了。 左上角point=(left, top), 右下角point=(right, bottom);
	 */

	 // 图像边框, 包含4条边定位
	struct ImageRect
	{
		int left;
		int top;
		int right;
		int bottom;

		ImageRect() : ImageRect(0, 0, 0, 0) { };

		ImageRect(const int l, const int t, const int r, const int b) : left(l), top(t), right(r), bottom(b) { }

		/* 判断另一rect是否在image_rect内，图像以左上角为原点，图像left越小越靠左，top越小越靠上,这样图像越大，right越大越靠右，bottom越大越靠下，这样图像越大。 */
		bool Contains(const ImageRect& rect) const
		{
			return left <= rect.left && top <= rect.top && right >= rect.right &&
				bottom >= rect.bottom;
		}
	};

	/*
	 * Image类，由ImageRect(边框)， ImageBgra*(颜色data)
	 */
	struct Image
	{
		ImageRect bounds;
		int bytes_to_next_row = 0;
		bool is_contiguous = false;
		const ImageBgra* data = nullptr;
	};

	inline bool operator==(const ImageRect& img_rect1, const ImageRect& img_rect2)
	{
		return img_rect1.left == img_rect2.left && img_rect1.top == img_rect2.top &&
			img_rect1.right == img_rect2.right && img_rect1.bottom == img_rect2.bottom;
	}

	/* 行跨度, 获取行的象素(字节数)，不包含边框填充 */
	inline int RowStride(const Image& img) { return sizeof(ImageBgra) * Width(img); }

	// 展开源img到dst, TODO 这里到底是个啥用途,还不清楚?
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

	namespace chrono = std::chrono;
	using std::chrono::duration_cast;
	using std::chrono::duration;

	class Timer
	{
		using Clock = std::conditional<chrono::high_resolution_clock::is_steady,
			chrono::high_resolution_clock, chrono::steady_clock>::type;
		using MicroSec = chrono::microseconds;

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

		MicroSec Duration() const { return this->duration_; }

	private:
		chrono::microseconds duration_;
		Clock::time_point dead_line_;
	}; // end class Timer;

	// 清理无效的图像边框,判断条件是img.right<rect.right, img.bottom<rect.bottom;
	void SanitizeRects(std::vector<ImageRect>& rects, const Image& img);

	// 获取image边框矩形
	inline const ImageRect& Rect(const Image& img) { return img.bounds; }

	enum CodeReturn { SUCCESS_CODE = 0, ERROR_EXPECTED_CODE = 1, ERROR_UNEXPECTED_CODE = 2 };

	auto CreateMonitor(int index, int id, int h, int w, int ox, int oy, const std::string& n, float scale) -> Monitor;

	auto CreateMonitor(int index, int id, int adapter, int h,
	                   int w, int ox, int oy, const std::string& n, float scale) -> Monitor;

	bool IsMonitorInsideBounds(const std::vector<Monitor>& monitors, const Monitor& monitor);

	Image CreateImage(const ImageRect& img_rect, int row_padding, const ImageBgra* data);

	std::vector<ImageRect> GetDifs(const Image& old_img, const Image& new_img);

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

	template <typename F, typename T, typename C>
	void ProcessCapture(const F& data, T& base, const C& monitor, const unsigned char* start_src, int src_rows_stride)
	{
		ImageRect image_rect;
		image_rect.left = 0;
		image_rect.top = 0;
		image_rect.bottom = Height(monitor);
		image_rect.right = Width(monitor);

		const auto size_img_bgra = static_cast<int>(sizeof(ImageBgra));
		const auto start_img_src = reinterpret_cast<const ImageBgra*>(start_src);
		int dst_row_stride = size_img_bgra * Width(monitor);

		// Sc库这里做了很多抽象, 为后面的GDI, DX, X11共用

		if (data.on_new_frame) {
			auto whole_img = CreateImage(image_rect, src_rows_stride, start_img_src);
			whole_img.is_contiguous = dst_row_stride == src_rows_stride;
			data.on_new_frame(whole_img, monitor);
		}
		if (data.on_frame_changed) {
			if (base.first_run_) {
				auto whole_img = CreateImage(image_rect, src_rows_stride - dst_row_stride, start_img_src);
				whole_img.is_contiguous = (dst_row_stride == src_rows_stride);
				data.on_frame_changed(whole_img, monitor);
				base.first_run_ = false;
			}
			else {
				auto new_img = CreateImage(image_rect, src_rows_stride - dst_row_stride, start_img_src);
				auto old_img =
					CreateImage(image_rect, 0, reinterpret_cast<const ImageBgra*>(base.image_buffer_.get()));
				auto img_difs = GetDifs(old_img, new_img);

				for (auto& r : img_difs) {
					auto left_offset = r.left * size_img_bgra;
					auto this_start_src = start_src + left_offset + (r.top + src_rows_stride);
					auto dif_img = CreateImage(r, src_rows_stride, reinterpret_cast<const ImageBgra*>(this_start_src));
					dif_img.is_contiguous = false;
					data.on_frame_changed(dif_img, monitor);
				}
			}
			auto start_dst = base.image_buffer_.get();
			if (dst_row_stride == src_rows_stride) {
				memcpy(start_dst, start_src, dst_row_stride * Height(monitor));
			}
			else {
				for (auto i = 0; i < Height(monitor); i++) {
					memccpy(start_dst + (i * dst_row_stride), start_src + (i * src_rows_stride), dst_row_stride);
				}
			}
		}
	}
}
