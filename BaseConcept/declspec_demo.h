#pragma once

#include <cstdint>

// vc ��̬���ӿ⵼���뵼������
// __declspec(dllexport)
// __declspec(dllimport)

// ���� #if defined(WINDOW) || defined(WIN32) ����꣬ʵ�ֿ�ƽ̨ͨ�ù��ô���. 
// #define SC_LITE_EXTERN __declspec(dllexport)
// ����ʹ��cmake������Ŀ������ƽ̨��ʱ���÷�����

#if defined(WINDOWS)|| defined(WIN32)			//��һ���жϣ�ƽ̨Ϊwin32 ִ�����涨��
#if defined(SC_LITE_DLL)						//��cmake������BUILD_SHARED_LIBSʱִ��,Ҳ����ʹ�ö�̬��ʱ��
#define SC_LITE_EXTERN __declspec(dllexport)    //set_target_properties PROPERTIES DEFINE_SYMBOL SC_LITE_DLL 
#else											// #define SC_LITE_EXTERN extern "C" __declspec(dllexport)
#define SC_LITE_EXTERN				// #define SC_LITE_EXTERN   ==> ���ʹ�þ�̬��lib�� SC_LITE_EXTERN ��
#endif
#else
#define SC_LITE_EXTERN
#endif

// ��̬��lib/*.a�Ͷ�̬��dll/so, �������һ���ģ� �����ӽ׽׶ν���̬��һ����������̬��ֻ�����н׶βŻᱻ���ء�
// ��ʾ����dll  
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
