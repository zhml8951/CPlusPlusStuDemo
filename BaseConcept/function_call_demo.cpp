#include <iostream>
#include <Windows.h>

#ifdef APIENTRY
#undef APIENTRY
#endif
#ifdef WINAPI
#undef WINAPI
#endif


void func_01(const int x, const int y) { std::cout << "x:" << x << ",y:" << y; };
int func_02() { return 0; }

// �ں�������ʱ��Ĭ����ʽ���ϣ� ��ʵ��ʵ�������£�
void __cdecl func_03(const int x, const int y) { std::cout << "x:" << x << ",y:" << y; };
int __cdecl func_04() { return 0; }
// ��ʵ���������ʡ���� __cdecl �����������Զ���C++ ����Ĭ�ϵ���__cdecl����������
// �鿴windows api ʱ���������� ���£�


#define WINAPI __stdcall
#define APIENTRY WINAPI

// windows�³���������ʽ������Ϊ�����䲻ͬ���������ĺꡣ
int APIENTRY winMain(_In_ HINSTANCE hInstance) { return 0; };
// ��ͬ�ڣ�
int __stdcall win_main() { return 0; };
void __stdcall func_05() { printf("func_05"); }

#ifdef DECLARE_HANDLE
#undef DECLARE_HANDLE
#endif

#ifdef HINSTANCE__
#undef HINSTANCE__
#endif


#define DECLARE_HANDLE(name) struct name##__{int unused;}; typedef struct name##__ *name


//void demo_main()
//{
//}

int main(int argc, char* argv[])
{
	printf("ok");
}
