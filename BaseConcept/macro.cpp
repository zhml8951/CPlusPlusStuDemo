#ifndef MACRO_HPP_
#define MACRO_HPP_
//#pragma once

#include <iostream>
#include <functional>

// #ifndef ...  #define .... #endif 为了避免同一文件被多次#include;
// #pragma once		// 功能 #ifndef .... #define ....#endif;
/*
 * C++ 宏定义(预处理)
 *
 */
#include <iostream>

// 简单字符串替换
// 对于简单的字符串替换, C++推荐的方法是使用using, typedef, 或者常量constexpr代替
#define PI 3.1415	// constexpr auto PI = 3.1415;
#define NAME "arnni"
#define AREA(r) PI*r*r
#define STR_PTR const char*
#define CHAR_PTR char*	// ==> typedef char* CHAR_PTR;
using CharP = char*;
typedef char* CharP1;
#define DOUBLE_QUOTE(a) #a
#define LINK(a,b) a##\ b
#define INT_NUM(name) (int #name)

// 带参数宏；
#define MUL(a,b) a*b
#define MAX(a,b) ((a)>(b)?(a):(b))

// #define #exp 一个 # 自动变成字符串 ==>  "exp"
#define TO_STR(user) #user			// ==> "user"
#define OUT(exp) std::cout << "User: "#exp << std::endl;

// #define #@c 变成字符， 加单引号 ==> 'c'
//#define TO_CHAR(s) #@s

// #define ## 字符连接作用
#define LINK_NAME(name) sz_##name
#define OUT_VAL(name) std::cout << "sz_"#name" = " << LINK_NAME(name) << "\n";

/*
 * 常见内置宏： (以 __ 起头 属C++内置，  以 _ 起头则表示 编译器内置)
 *		__FILE__,   __DATE__,  __FUNCTION__, __LINE__,  __TIME__, __TIMESTAMP__;
 */

namespace macro_demo
{
	void Demo01()
	{
		auto r = 3.3f;
		const auto area = AREA(r); // ==> PI*r*r ==> 3.1415 * 3.3 * 3.3;
		printf("r: %.2f,  area: %.2f\n", r, area);
		auto b = 4.4f;
		printf("max(a,b): %f\n", MAX(b, r));
		std::cout << "date: " << __DATE__ << ", file:  " << __FILE__ << "\n";
		std::cout << "func:  " << __FUNCTION__ << ",  line:  " << __LINE__ << "\n";
		std::cout << "timestamp: " __TIMESTAMP__ << ", time: " << __TIME__ << "\n";
	}

	void Demo02()
	{
		auto sz = "user";
		auto zh = "Zh";

		const auto LINK_NAME(user) = "define";
		std::cout << LINK_NAME(user) << "\n";
		OUT_VAL(user);
		OUT("zh");
		std::cout << "str:  " << TO_STR(use_STR) << "\n";
	}

	void Demo03()
	{
#define FILE(a) {a, #a}
		enum Idd { OPEN, CLOSE };
		typedef struct Msg
		{
			Idd id;
			const char* msg;
		} Msg;

		Msg msg_arr[] = {FILE(OPEN), FILE(CLOSE)}; //展开后:  msg_arr = { {OPEN, "OPEN"}, {CLOSE, "CLOSE"} };
		std::cout << msg_arr << "\n";
	}

	void Demo04()
	{
		// 这里都用到一个中间转换宏。 涉及多层展开；

		// 创建合并匿名变量
#define ANONYMOUS2(type, var, line) type var##line
#define ANONYMOUS1(type, line) ANONYMOUS2(type, anonymous_, line)
#define ANONYMOUS(type) ANONYMOUS1(type, __LINE__)

		ANONYMOUS(int); // 即：  int anonymous_80 // 80行号；
		ANONYMOUS(double); // double anonymous_81;
		ANONYMOUS(const char*); // const char* anonymous_82

		/*
		 * ANONYMOUS(const char*)
		 *      ---> ANONYMOUS1(const char*, __LINE)
		 *      ---> ANONYMOUS2(const char*, anonymous, 82)
		 *      ---> const char* anonymous_82;
		 *           经过3层解开，得到最终的匿名变量，类型const char* 变量名 anonymous_82：
		 */

		// 记录文件名 *** 在宏引用宏定义或内置宏，必要经过多层展开，才能得最终值；
#define GET_FILE_NAME1(f) #f
#define GET_FILE_NAME(f) GET_FILE_NAME1(f)

		static char file_names[] = GET_FILE_NAME(__FILE__);

		// 得到一个数组类型所对应的字符串缓冲大小
#define TYPE_BUF_SIZE1(type) sizeof #type
#define TYPE_BUF_SIZE(type) TYPE_BUF_SIZE1(type)
		char buf[TYPE_BUF_SIZE(INT_MAX)];
	}

	// 这里使用内置宏__LINE__; 一级宏不会展__LINE__实际值， 通过一层宏再调用二级宏展开__LINE__值。
	// 这里涉及宏内部部分拼接，ON_SCOPE_EXIT(callback)  ==> ScopeGuard cls_obj_name(...)
#define SCOPE_GUARD_LINE_NAME_CAT(name, line) name_##line
#define SCOPE_GUARD_LINE_NAME(name, line) SCOPE_GUARD_LINE_NAME_CAT(name, line)
#define ON_SCOPE_EXIT(callback) ScopeGuard SCOPE_GUARD_LINE_NAME(EXIT, __LINE__)(callback)

	class ScopeGuard
	{
	public:
		explicit ScopeGuard(const std::function<void()> onexit_t) : on_exit_scope_(onexit_t), dismissed_(false) {}

		~ScopeGuard()
		{
			if (!dismissed_) on_exit_scope_();
		}

		void Dismiss() { dismissed_ = true; }

	private:
		std::function<void()> on_exit_scope_;
		bool dismissed_;

	private: //设置private,禁止执行拷贝构造和拷贝赋值，等同于：  ScopeGuard(ScopeGuard const&)=delete;
		ScopeGuard(ScopeGuard const&); // 拷贝构造
		ScopeGuard& operator=(ScopeGuard const&) = delete; // 拷贝赋值,设置成private，后delete可去掉。
	};

	void Demo05()
	{
		const auto cb = [&]()-> void { printf("ok"); };
		ScopeGuard sc01([&] { printf("Ok"); });
		ScopeGuard sc02([&]()-> void { printf("ScopeGuard"); });

		ON_SCOPE_EXIT([&] {printf("ON_Scope_exit"); }); // ScopeGuard EXIT_140([&]{printf("ON_Scope_exit");});
		ON_SCOPE_EXIT(cb); //ScopeGuard EXIT_141(cb);
	}

	// C++/C macro
	void Demo06()
	{
		using std::cout;
#if defined(MACOS)
		cout << "Standard Clib: " << __STDC_HOSTED__ << "\n";
		cout << "Standard C: " << __STDC__ << "\n";
#endif
#if defined(WIN32)
		cout << "WIN32: " << WIN32 << "\n";
#endif
#if defined(WIN64)
		cout << "WIN64: " << WIN64 << "\n";
#endif
#if defined(WINDOWS) || defined(_WINDOWS)
		cout << "WINDOWS: " << WINDOWS << '\n';
#endif
		// __cplusplus 是C++ 内置预定义宏, 用来判断是否为C++;
#if defined(__cplusplus)
		cout << "__cplusplus: " << __cplusplus << "\n";
#endif
	}

	// 判断C++， extern "C" 按C的方式来编译。
#if defined(__cplusplus)
	extern "C" {
#endif
	// some extern c++ code.....
#if defined(__cplusplus)
	}
#endif

	/*
	 * #define只是文本替换工具，typedef可替代大部分#define作用，且功能更强大,
	 *	typedef [originType] [newType]
	 *	任何声明变量的语句前加上typedef之后，原来是变量的都变成一种类型，不管这个声明中的标识符号出现在中间还是最后.
	 */

	typedef int Num;
	typedef int* PtrInt; /// 定义int* 类型
	typedef int IntArray[]; /// 定义int array 类型

	typedef struct
	{
		int a;
	} IntStruct; /// 定义struct类型

	typedef double FuncP(std::string&); /// 定义一个函数类型,参数string, 返回类型double

	typedef void FuncQ(int* p, const std::string& s1, const std::string& s2, size_t size, bool is_true);
	/// 定义FuncQ

	FuncP (func01); /// => double func01(std::string&); ==> FuncP func01;
	FuncP func02;

	FuncQ (func03); /// => void func03(int* p,....);  ==> FuncQ func04;
	FuncQ func04;

	//// 再看几个复杂的例子:
	typedef int*(*A[10])(int, char*); /// 从右向左理解： A是一个包含10个函数指针的数组，这些函数的参数列表是(int, char*) 返回值是int*;

	typedef int (*(* Fb())[10])();
	/// 从Fb开始，从右向左，()说明Fb是函数，左边*说明返回值指针，往外层右边[10]说明函数返回值是指向数组的指针，向左*说明数组元素是指针，
	/// 再向外右边()说明无参参数列表，说明数组中包含的元素是函数指针，这些函数没的参数，返回值是int;
	/// 总括：Fb是一个返回指针的函数，指针指向含有10个函数指针的数组，这些函数不接受参数，返回值是int;

#ifndef STRICT
#define DECLARE_HANDLE(name) struct name##__{int unused;}; typedef struct name##__ *name
#else
	typedef void* HANDLE;
#define DECLARE_HANDLE(name) typedef HANDLE name
#endif
	void declare_handle_demo()
	{
		DECLARE_HANDLE(MUser); /*	==>  struct MUser__{int unused; }; typedef struct MUser__ *MUser; */
		MUser user01; // user01 相当于一个struct指针， 指向struct MUser__{int unused; };
		DECLARE_HANDLE(Mouse);
		Mouse mouse01;
		DECLARE_HANDLE(Monitor);
	}

	/*
	 * 在Linux信号量里有一个奇怪的typedef的用法， typedef void (*Handler_t)(int);
	 */
	void (* signal1(int sig, void (*handle)(int)))(int) = delete;
	// 分析这类复杂声明式，首先确认哪个是关键变量名， 这里从左很一个*signal; 为了简化式，handle采用typedef来简化；

	typedef void (*HandleTy)(int); // 简化后如下：
	HandleTy signal2(int sig, HandleTy handle) = delete;

	// 为什么可以这样理解呢?  ===>
	// 1. void(*handle)(int) 可以很简单看出来，对应于typedef void(*H)(int); 
	// 2. 声明式总体看 void (*sig(...))(int) 也对对应于typedef的Handle，直接代换过去；就成了 H S(int, H);
	// H S（int, H) 样式看着简单，但真正解析后是什么意思呢？ 只看声明是不明白的，加上定义，则比较明了， 即返回类型H， 参数(int, H); 如下：
	HandleTy Signal3(int sig, const HandleTy handle)
	{
		handle(88); // 传函数指针， 相当于回调
		const HandleTy rst = [](int x) -> void { std::cout << "int lambda."; };
		return rst;
	};
}
#endif

// g++ -E macro.cpp -o macro.i 不运行编译，只进行宏展开命令;
// g++ 多文件编译: 
// g++ -c macro.cpp extern_demo.cpp -std=c++11
// g++ macro.o extern_demo. -o A.exe; 

// 测试关键词extern; 这里引入extern_demo;
#include "extern_demo.h"

namespace extern_simple
{
	extern const std::string kStrShare = "string in macro.";
	extern std::string str_share;
	extern int value_c;

	// 模板在 extern_demo.h 定义, 在 extern_demo.cpp实例化过<int, double>, 这里使用extern避免重复实例化。 
	//extern template int SumTy1<int, double>(int, double);
	// 现代编译器已经能够很好对于此类问题进行相应的优化， 故extern template 声明外部template, 或者显式实例化template SumTy1, 也是一样的， 而且用处不大
	//template double SumTy1<double, int>(double, int);

	void TestSum(int)
	{
		auto rst = SumTy1(89, 89.9);
		rst += SumTy1(99.9, 88);
		std::cout << "Rst: " << rst << "\n";
		std::cout << "value_c: " << value_c << "\n";
		std::cout << "str_share: " << str_share << "\n";
		std::cout << "macro.cpp static value_b: " << value_b << "\n";
		std::cout << "global const kValueD: " << kValueD << "\n";
		std::cout << "defined value6 A.cpp, Called in B.cpp: " << kValue6 << "\n";
	}
}

int main(int argc, char* argv[])
{
	macro_demo::Demo01();
	printf("\n--------------------------\n");
	macro_demo::Demo02();
	printf("\n--------------------------\n");
	macro_demo::Demo03();
	printf("\n--------------------------\n");
	macro_demo::Demo06();
	printf("\n--------------------------\n");
	extern_simple::TestSum(8);
	extern_simple::Func01(8, 8);
	//extern_simple::Func04(8);
	printf("\n--------------------------\n");
	return 1;
}
