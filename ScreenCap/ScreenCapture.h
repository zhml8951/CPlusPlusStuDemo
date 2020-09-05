#pragma once
#include <cstdint>
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
		int x;
		int y;
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
		int id = INT32_MAX;
		int index = INT32_MAX;
		int adapter = INT32_MAX;
		int height = 0;
		int width = 0;
		int original_height = 0;
		int original_width = 0;
		int offset_x = 0;
		int offset_y = 0;
		int original_offset_x = 0;
		int original_offset_y = 0;
		char name[128]{0};
		float scaling{1.0f};
	};

	struct SC_EXTERN Image;

	struct SC_EXTERN ImageBgra
	{
		unsigned char b, g, r, a;
	};

	inline SC_EXTERN int Index(const Monitor& monitor) { return monitor.index; }

	inline SC_EXTERN int Id(const Monitor& monitor) { return monitor.id; }

	inline SC_EXTERN int Adapter(const Monitor& monitor) { return monitor.adapter; }

	inline SC_EXTERN int OffsetX(const Monitor& monitor) { return monitor.offset_x; }
	inline SC_EXTERN int OffsetY(const Monitor& monitor) { return monitor.offset_y; }
	inline SC_EXTERN int OffsetX(const Window& window) { return window.position_point.x; }
	inline SC_EXTERN int OffsetY(const Window& window) { return window.position_point.y; }
	inline SC_EXTERN void OffsetX(Monitor& monitor, const int x_i) { monitor.offset_x = x_i; }
	inline SC_EXTERN void OffsetY(Monitor& monitor, const int y_i) { monitor.offset_y = y_i; }
	inline SC_EXTERN void OffsetX(Window& window, const int x_i) { window.position_point.x = x_i; }
	inline SC_EXTERN void OffsetY(Window& window, const int y_i) { window.position_point.y = y_i; }

	inline SC_EXTERN const char* Name(const Monitor& monitor) { return monitor.name; }
	inline SC_EXTERN const char* Name(const Window& window) { return window.name_c; }

	inline SC_EXTERN int Height(const Monitor& monitor) { return monitor.height; }
	inline SC_EXTERN int Width(const Monitor& monitor) { return monitor.width; }
	inline SC_EXTERN int Height(const Window& window) { return window.size_point.y; }
	inline SC_EXTERN int Width(const Window& window) { return window.size_point.x; }
	inline SC_EXTERN void Height(Monitor& monitor, const int h_i) { monitor.height = h_i; }
	inline SC_EXTERN void Width(Monitor& monitor, const int w_i) { monitor.width = w_i; }
	inline SC_EXTERN void Height(Window& window, const int h_i) { window.size_point.y = h_i; }
	inline SC_EXTERN void Width(Window& window, const int w_i) { window.size_point.x = w_i; }
	inline SC_EXTERN int Height(const ImageRect& rect) { return rect.top - rect.left; }
	inline SC_EXTERN int Width(const ImageRect& rect) { return rect.right - rect.top; }
	inline SC_EXTERN int Height(const Image& img) { return Height(img.bounds); }
	inline SC_EXTERN int Width(const Image& img) { return Width(img.bounds); }

	inline SC_EXTERN int X(const Point& point) { return point.x; }
	inline SC_EXTERN int Y(const Point& point) { return point.y; }

	//获取image的rgba信息 
	inline SC_EXTERN const ImageBgra* StartSrc(const Image& img) { return img.data; }

	inline SC_EXTERN bool IsDataContiguous(const Image& img) { return img.is_contiguous; }

	inline SC_EXTERN const ImageBgra* GotoNextRow(const Image& img, const ImageBgra* current)
	{
		const auto c = reinterpret_cast<const unsigned char*>(current);
		return reinterpret_cast<const ImageBgra*>(c + img.bytes_to_next_row);
	}

	bool IsMonitorInSideBounds(const std::vector<Monitor>& monitors, const Monitor& monitor);

	std::vector<Monitor> GetMonitors();

	std::vector<Window> GetWindows();

	static bool screen_capture_manager_exists = false;

	class SC_EXTERN IScreenCaptureManager
	{
	public:
		virtual ~IScreenCaptureManager() {}

		template <typename Rep, typename Period>
		void SetFrameChangeInterval(const std::chrono::duration<Rep, Period>& real_time)
		{
			SetFrameChangeInterval(std::make_shared<Timer>(real_time));
		}

		virtual void SetFrameChangeInterval(const std::shared_ptr<Timer>& timer) = 0;

		template <typename Rep, typename Period>
		void SetMouseChangeInterval(const std::chrono::duration<Rep, Period>& real_time)
		{
			SetMouseChangeInterval(std::make_shared<Timer>(real_time));
		}

		virtual void SetMouseChangeInterval(const std::shared_ptr<Timer>& timer) = 0;
	};

	//class ScreenCaptureManager: public { }
}
