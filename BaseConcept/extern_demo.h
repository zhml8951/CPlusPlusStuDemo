#pragma once
#include <string>
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif
inline int add(const int num1, const int num2)
{
	return num1 + num2;
}

extern const char* str01;
extern char* str02;
extern std::string str03;
extern std::vector<std::string> vec01;
extern int int01;

const char* int_to_string(const int num);


// C++ 常用的条件编译时包裹方式 
#ifdef __cplusplus
}
#endif

// extern "C" {   // 主要原因是C++ 支持重载机制， 而C是不支持的。 C++在编译生成时会对函数名进行一些处理(函数名改编)，
// 根据函数名及参数和返回类型进行不同方式处理， 

/*
 * extern的理解比较麻烦，以下是基本概念：
 *  0. extern 表明函数和全局变量的作用范围(可见性)的关键字，表明声明的对象可以在本模块和其它模块中使用，通常在h文件中定义变量时为了防止多个模板引用时出现链接冲突，
 *		使用extern 定义全局变量可以其它模板使用，与extern对应的是static， static修饰的全局变量只能在本模板中使用。 
 *	1. C++中, 函数和变量, 定义只有一次, 但声明可以有很多次. 函数的声明和定义区别比较明显, 函数定义是带函数体,而声明没有函数体;
 *		而变量却不一样, 变量(全局变量)只有加了extern 才是纯声明, 局部变量是在运行时才在栈上分配内存, 不能使用extern.
 *		extern 
 *	2. extern 限定const对象的作用域，const对象默认作用域仅在本文件内, 如果我们需要在多个文件内共享const对象，需在在声明和定义时使用extern关键字
 *		其主要用途在于同extern普通全局变量一样，声明一个变量不定义， 相当于一个占位符， 具体值是在使用时才确定。 
 *	3. extern 限定template， 当多个cpp源文件使用相同的模板并且提供了相同的模板参数，避免实例化相同的模板。 
 *	4. extern "C"{...}, 这里有"C"表示一种编译链接规约，"C"表示符合C语言的编译链接规约的任何语言，当然主要指C语言。 
 */


// h 头文件最好只有声明, 不要有定义. 这是基本观念
/*
 *	C++ 最佳实践提示： 不在h文件中定义全局对象(函数和变量), 这种问题在单模块(单文件)编译链接时没有问题，实际项目中，会有很多模块，并且多个模块相互引用就有问题，会提示"重复定义";
 *	例： 在A.h中定义 int value_a; 这种看似没有赋值只是声明，实质上它同int value_a=0;是没有区别的。编译器会自动初始化；
 *	模块相互引用即： A.cpp中#include "A.h", B.cpp中#include "A.h" ... 这种情况导致多次#include就会多次定义，但C++对象重复声明，但不允许重复定义, 
 *	如果不需要多个模块重享全局变量，则可以直接使用static来定义，static将全局变量限定于本模块内，即：static int value_b;
 *	但有时项目中需要一些全局变量在所有模块中使用，那就需要在h文件中只声明，即extern int value_c; extern告诉编译器这里只是声明，定义在外面(extern), 大概明白为什么称extern了吧。
 *	如： 在A.h中声明extern int value_c; A.cpp中定义 int value_c = 88; 模块B.cpp中#include "A.h", 再次声明: extern int value_c; 这样就可B.cpp中使用A.cpp定义的值。
 *
 *	同样道理，在A.h直接定义函数Func03(int){...}, 多个文件#include后也会提示"multiple definition of.."; 
 *	解决方法则是A.h只声明，【extern】void Func03(int); 实现放在A.cpp中即可。 如果函数功能很小，则直接inline void Func03(int); 现在明白inline的作用了吧。。。。。。
 *	
 *	解决变量和函数在多文件时的作用域(可见性)问题的另一关键词static, 函数static void Func04(int);在A.h里声明为static，定义在放在A.cpp中，
 *	B.cpp里#include "A.h",调用失败，这就涉及static可限定对象作用域在本文件内；
 *	变量如： static int value_b; 程序运行时会初始化，B.cpp #include "A.h", 可以调用到value_b,并且链接不会出错，全局静态变量或静态常量只在程序启动初始化一次，
 *	总体来讲static 变量限定了作用域为本模块，这里与网上所说的"限定为本文件内" 是有不同的。 
 *	 
 */
namespace extern_simple
{
	//int value_a; // 是定义全局变量a, 会生产默认值, 全局变量在被多个文件引用时,编译没有问题,但链接时会出现"Already defined int ... "
	static int value_b = 85; // 定义全局变量b, 使用static限定value_b只在本模块可用
	extern int value_c; // 声明全局变量c, 这是只有声明;
	static const int kValueD = 86; // 这里定义静态常量，macro.cpp中#include "extern_demo.h"可使用。

	extern const int kValue6;

	//extern int value_d = 0; // 定义全局变量d, 并赋初始值0, 这样使用没有必要, g++ 直接警告;

	extern void Func01(int, int);
	void Func02(int, int); // 有没有extern 结果是一样的。  定义函数数时，没加extern时期默认行为也是extern的。 

	inline void Func03(const int x) {printf("x: %d", x); }

	static void Func04(int x);		// static 直接限定Func04只能在extern_demo中使用。 

	extern std::string str_share; 
	const std::string kStrNoExtern = "String in extern.h";

	template <typename Ty1, typename Ty2>
	Ty1 SumTy1(const Ty1 val1, const Ty2 val2)
	{
		return val1 + static_cast<Ty1>(val2);
	}

	/* C++中调用C代码：
	   
	// C语言c_header.h 中声明CPrint函数， 
		extern void CPrint(int i);
	// C语言c_impl.c 文件中定义(实现)CPrint
		void CPrint(int i){ printf("num: %d\n", i); }
	// C++语言 c++_ref.cpp 引入C的头文件
		extern "C" { #include "c_header.h" }
	// 这样就可以在C++中使用c中实现的函数

	*/

	/* C中调用C++代码：
	 
	// C++语言cpp_header.h声明CppPrint
		extern "C" void CppPrint(int i);
	// C++语言cpp_file.cpp中定义(实现)CppPrint
		void CppPrint(int i){std::cout << i << "\n"; }
	// C语言c_file.c 调用CppPrint;
		extern void CppPrint(int i); 然后就可以直接使用。
	 */
	/* C++ 引入C代码时的标准语法*/
#ifdef __cplusplus
	extern "C" {		// #ifdef...#endif 条件编译，

#endif	/* __cplusplus */

#ifdef __cplusplus
	}
#endif
}
