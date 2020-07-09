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

// 在函数定义时，默认样式如上， 其实真实样子如下：
void __cdecl func_03(const int x, const int y) { std::cout << "x:" << x << ",y:" << y; };
int __cdecl func_04() { return 0; }
// 真实情况是我们省略了 __cdecl 而编译器会自动按C++ 函数默认调用__cdecl来处理函数。
// 查看windows api 时经常看到， 如下：


#define WINAPI __stdcall
#define APIENTRY WINAPI

// windows下常见这类形式，这是为库适配不同情况而定义的宏。
int APIENTRY winMain(_In_ HINSTANCE hInstance) { return 0; };
// 等同于：
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
