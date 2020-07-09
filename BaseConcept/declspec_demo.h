#pragma once

#include <cstdint>

// vc 动态链接库导出与导入申明
// __declspec(dllexport)
// __declspec(dllimport)

// 采用 #if defined(WINDOW) || defined(WIN32) 这类宏，实现跨平台通用共用代码. 
// #define SC_LITE_EXTERN __declspec(dllexport)
// 这类使用cmake管理项目，做跨平台库时常用方法。

#if defined(WINDOWS)|| defined(WIN32)			//第一层判断，平台为win32 执行下面定义
#if defined(SC_LITE_DLL)						//由cmake定义了BUILD_SHARED_LIBS时执行,也就是使用动态库时，
#define SC_LITE_EXTERN __declspec(dllexport)    //set_target_properties PROPERTIES DEFINE_SYMBOL SC_LITE_DLL 
#else											// #define SC_LITE_EXTERN extern "C" __declspec(dllexport)
#define SC_LITE_EXTERN				// #define SC_LITE_EXTERN   ==> 如果使用静态库lib则 SC_LITE_EXTERN 空
#endif
#else
#define SC_LITE_EXTERN
#endif

// 静态库lib/*.a和动态库dll/so, 编译段是一样的， 在链接阶阶段将静态库一起打包，而动态库只在运行阶段才会被加载。
// 显示调用dll  
// HMODULE hDll = LoadLibrary("\\..\\dynamic.dll");
// ReSharper disable CppInconsistentNaming
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
	char Name[20]{0};
	float Scaling{1.0f};
};

SC_LITE_EXTERN int Index(const Monitor& monitor);
