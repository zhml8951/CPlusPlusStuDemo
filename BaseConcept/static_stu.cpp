#include <iostream>

/*
 * C++ 存储说明符: auto, register, extern, static
 *	auto, register => 自动存储期: 在进入声明变量的程序块时自动建立,它在程序块活动时存在,退出时自动撤销; 
 *		(auto) 已经不是过去auto，不作为存储说明符来使用；当前C++11以上auto用于类型推导使用; 当前声明和定义变量默认即auto; 
 *		(register) 主用于将数据存储于寄存器，但实际使用则由编译器决定，故基本不使用。 
 *	extern, static => 静态存储期: 具有静态存储持续期，虽然它的值在调用之间就存在，但可见性仍限制于局部域内；
 *		静态函数会被限止于文件内使用，会分配在一个一直存在的存储区，直到退出应用程序，避免调用函数的压栈和出栈，提升性能;
 */
/*
 * 静态变量， 存储在静态存储区， 整个周期只保存一份拷贝。
 * 静态全局变量，并且只在本文件内有效。如果要在外部文件使用，必须声明为  extern static string kStr;
 * 类的静态成员必须在类内声明，在类外初始化，静态常量可以在类内初始化。 static const int kNum = 1;
 * 静态成员属于整个类而不属于某个对象。 
 */

/*
 * static 静态变量， 一个最重要的功能是隐藏， 当程序由多个源程序组成时，所有未加static的全局变量和函数都具有全局可见性。
 * 其它文件是可以访问的。对于普通函数， static作用就在于隐藏，其它源文件不可访问，而普通变量和函数没有加static，是可以访问的。
 * 静态存储持续性变量提供3种外部链接性： 外部链接性(其它文件可访问)， 内部链接性(只在当前文件可访问)， 无链接性(只在函数或代码块内访问)
 */

int global_num = 1000;
extern int ext_num = 100; // 定义变量，同上句意义相同
extern int static_num;

static int file_inline_num = 2000;

namespace static_demo
{
	int namespace_num = 3000;

	void func()
	{
		static int func_num_static = 4000; //局部静态变量虽然只在函数内使用，但存储在静态区，多次调用函数，只初始化一次，后续对其修改都会保持。
		int num = 5000;
	}
}


int main(int argc, char* argv[]) {}
