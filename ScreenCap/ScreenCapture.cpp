#include "pch.h"
#include "ScreenCapture.h"
#include "ScCommon.h"

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

	std::vector<Monitor> GetMonitors() {}
	std::vector<Window> GetWindows() {}
}
