#pragma once
#include <cstdint>
#include <cstdlib>
#include "ScCommon.h"

#ifdef __cplusplus
#if defined(WINDOWS) || defined(WIN32)
#define SC_EXTERN __declspec(dllexport)
#else
#define SC_EXTERN
#endif  // end WINDOWS
#else
#define SC_EXTERN extern "C" __declspec(dllexport)
#endif // end __cplusplus

namespace sc
{
	struct SC_EXTERN Point
	{
		int x_i;
		int y_i;
	};

	struct SC_EXTERN MousePoint
	{
		Point position_point;
		Point hot_spot_point;
	};

	struct SC_EXTERN Window
	{
		size_t handle_size;
		Point position_point;
		Point size_point;
		char name_c[128]{0};
	};

	struct SC_EXTERN Monitor
	{
		int id_i = INT32_MAX;
		int index_i = INT32_MAX;
		int adapter_i = INT32_MAX;
		int height_i = 0;
		int width_i = 0;
		int original_height_i = 0;
		int original_width_i = 0;
		int offset_x_i = 0;
		int offset_y_i = 0;
		int original_offset_x_i = 0;
		int original_offset_y_i = 0;
		char name_c[128]{0};
		float scaling_x{1.0f};
	};

	struct SC_EXTERN Image;

	struct SC_EXTERN ImageBgra
	{
		unsigned char b, g, r, a;
	};


	inline SC_EXTERN int index(const Monitor& monitor) { return monitor.index_i; }

	inline SC_EXTERN int id(const Monitor& monitor) { return monitor.id_i; }

	inline SC_EXTERN int adapter(const Monitor& monitor) { return monitor.adapter_i; }

	inline SC_EXTERN int offset_x(const Monitor& monitor) { return monitor.offset_x_i; }
	inline SC_EXTERN int offset_y(const Monitor& monitor) { return monitor.offset_y_i; }
	inline SC_EXTERN int offset_x(const Window& window) { return window.position_point.x_i; }
	inline SC_EXTERN int offset_y(const Window& window) { return window.position_point.y_i; }
	inline SC_EXTERN void offset_x(Monitor& monitor, const int x_i) { monitor.offset_x_i = x_i; }
	inline SC_EXTERN void offset_y(Monitor& monitor, const int y_i) { monitor.offset_y_i = y_i; }
	inline SC_EXTERN void offset_x(Window& window, const int x_i) { window.position_point.x_i = x_i; }
	inline SC_EXTERN void offset_y(Window& window, const int y_i) { window.position_point.y_i = y_i; }

	inline SC_EXTERN const char* name(const Monitor& monitor) { return monitor.name_c; }
	inline SC_EXTERN const char* name(const Window& window) { return window.name_c; }

	inline SC_EXTERN int height(const Monitor& monitor) { return monitor.height_i; }
	inline SC_EXTERN int width(const Monitor& monitor) { return monitor.width_i; }
	inline SC_EXTERN int height(const Window& window) { return window.size_point.y_i; }
	inline SC_EXTERN int width(const Window& window) { return window.size_point.x_i; }
	inline SC_EXTERN void height(Monitor& monitor, const int h_i) { monitor.height_i = h_i; }
	inline SC_EXTERN void width(Monitor& monitor, const int w_i) { monitor.width_i = w_i; }
	inline SC_EXTERN void height(Window& window, const int h_i) { window.size_point.y_i = h_i; }
	inline SC_EXTERN void width(Window& window, const int w_i) { window.size_point.x_i = w_i; }
	inline SC_EXTERN int height(const ImageRect& rect) { return rect.top_i - rect.bottom_i; }
	inline SC_EXTERN int width(const ImageRect& rect) { return rect.right_i - rect.left_i; }
	inline SC_EXTERN int height(const Image& img) { return height(img.bounds); }
	inline SC_EXTERN int width(const Image& img) { return width(img.bounds); }

	inline SC_EXTERN int x(const Point& point) { return point.x_i; }
	inline SC_EXTERN int y(const Point& point) { return point.y_i; }
	inline SC_EXTERN const ImageBgra* start_src(const Image& img) { return img.data; }

	inline SC_EXTERN bool is_data_contiguous(const Image& img) { return img.is_contiguous; }

	inline SC_EXTERN const ImageBgra* goto_next_row(const Image& img, const ImageBgra* current)
	{
		const auto c = reinterpret_cast<const unsigned char*>(current);
		return reinterpret_cast<const ImageBgra*>(c + img.bytes_to_next_row);
	}
}
