#include <iostream>

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
 * 
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


int main(int argc, char* argv[])
{
}
