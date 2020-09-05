#pragma once
#include <cstdint>

#if defined(WINDOWS) || defined(WIN32)
#if defined(SC_LITE_DLL)
#define SC_LITE_EXTERN __declspec(dllexport)
#else
#define SC_LITE_EXTERN
#endif
#else
#define SC_LITE_EXTERN
#endif

// 静态库lib/*.a和动态库dll/so, 编译段是一样的， 在链接阶阶段将静态库一起打包，而动态库只在运行阶段才会被加载。
// 显示调用dll
// HMODULE hDll = LoadLibrary("\\..\\dynamic.dll");
// ReSharper disable CppInconsistentNaming

namespace type_auto
{
	struct SC_LITE_EXTERN Monitor
	{
		int Id = INT32_MAX;
		int Index = INT32_MAX;
		int Adapter = INT32_MAX;
		int Height = 0;
		int Width = 0;
		int OriginalHeight = 0;
		int OriginalWidth = 0;
		int OffsetX = 0;
		int OffsetY = 0;
		int OriginalOffsetX = 0;
		int OriginalOffsetY = 0;
		char Name[20] = {0};
		float Scaling{1.0f};
	};

	SC_LITE_EXTERN inline int Index(const Monitor& monitor) { return 1; }
}
