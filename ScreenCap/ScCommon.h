#pragma once

#include <vector>
#include <atomic>
#include <thread>
#include "ScreenCapture.h"
#include <cassert>
#include <chrono>

namespace sc
{
	struct ImageRect
	{
		int left_i;
		int top_i;
		int right_i;
		int bottom_i;

		ImageRect() : ImageRect(0, 0, 0, 0)
		{
		};

		ImageRect(const int l_i, const int t_i, const int r_i, const int b_i) :
			left_i(l_i), top_i(t_i), right_i(r_i), bottom_i(b_i)
		{
		}

		bool contains(const ImageRect& rect) const
		{
			return left_i <= rect.left_i && top_i <= rect.top_i && right_i >= rect.right_i && bottom_i >= rect.bottom_i;
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
		return img_rect1.left_i == img_rect2.left_i && img_rect1.top_i == img_rect2.top_i &&
			img_rect1.right_i == img_rect2.right_i && img_rect1.bottom_i == img_rect2.bottom_i;
	}

	inline int row_stride(const Image& img) { return sizeof(ImageBgra) * width(img); }

	inline void extract(const Image& img, unsigned char* dst, size_t dst_size)
	{
		auto img_size = width(img) * height(img) * sizeof(ImageBgra);
		assert(dst_size >= static_cast<size_t>(img_size));
		auto dst_start = dst;
		auto src_start = start_src(img);
		if (is_data_contiguous(img)) {
			memcpy(dst_start, src_start, img_size);
		}
		else {
			img_size = sizeof(ImageBgra) * width(img);
			for (auto i = 0; i < height(img); i++) {
				memcpy_s(dst_start, img_size, src_start, img_size);
				dst_start += img_size;
				src_start = goto_next_row(img, src_start);
			}
		}
	}

	class Timer
	{
		using Clock = std::conditional<std::chrono::high_resolution_clock::is_steady,
		                               std::chrono::high_resolution_clock, std::chrono::steady_clock>::type;
		using MicroSec = std::chrono::microseconds;

	public:
		template <typename Rep, typename Period>
		explicit Timer(const std::chrono::duration<Rep, Period>& duration):
			duration_(std::chrono::duration_cast<MicroSec>(duration)), dead_line_(Clock::now() + duration_)
		{
		}

		void start() { this->dead_line_ = Clock::now() + duration_; }

		void wait() const
		{
			const auto now = Clock::now();
			if (now < dead_line_) {
				std::this_thread::sleep_for(dead_line_ - now);
			}
		}

		MicroSec duration() const { return this->duration(); }

	private:
		std::chrono::microseconds duration_;
		Clock::time_point dead_line_;
	}; // end class Timer;

	void sanitize_rects(std::vector<ImageRect>& rects, const Image& img);

	inline const ImageRect& rect(const Image& img) { return img.bounds; }

	template <typename F, typename M, typename W>
	struct CaptureData
	{
		std::shared_ptr<Timer> FrameTimer;
	};


	class SC_EXTERN IScreenCaptureManager
	{
	public:
		virtual ~IScreenCaptureManager()
		{
		}

		//template<typename Rep, typename Period>
		//void set_
	};
}
