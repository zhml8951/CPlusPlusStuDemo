#ifndef MACRO_HPP_
#define MACRO_HPP_
//#pragma once

#include <iostream>
#include <functional>

// #ifndef ...  #define .... #endif 为了避免同一文件被多次#include;
// #pragma once		// 功能 #ifndef .... #define ....#endif;
/*
 * C++ 宏定义(预处理)
 */

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

// 结果是(p) 实质是什么都不做，主要目的是防止编译器警告。 即函数内如果定义变量没有使用则可以使用(var)方式去忽略警告;
#define UNREFERENCED_PARAM(p) (p)

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

		//设置private,禁止执行拷贝构造和拷贝赋值，等同于：  ScopeGuard(ScopeGuard const&)=delete;
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

	// C/C++ macro
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

	/*
	 *	typedef double FuncDecl(string&)，语句本来意思是声明一个函数，加typedef则变成定义了一个FuncDecl类型。
	 *	那么怎么使用呢？  如:  FuncP func01; 此句怎么理解呢; FuncP(func02) 又是什么？
	 *		其实很简单：   FuncP func01; ==> FuncP(func01); ==> double func01(string&); 仅仅是声明一个函数；
	 *	这样的定义实际用处不大, 只是在声明大量的同类型函数时,采用这种typedef定义一个这样类型，
	 *	* 这里需要注意 FuncDeclP 与 (*FuncPtr) 之间的区别, 前者是申请一种函数类型,后者则定义函数指针类型;
	 */
	typedef double FuncDeclP(std::string&); /// 定义一个函数类型,参数string, 返回类型double
	typedef double (*FuncPtr)(std::string&); // 定义函数指针类型FuncPtr,返回类型double, 参数string;

	typedef void FuncDeclQ(int* p, const std::string& s1, const std::string& s2, size_t size, bool is_true);
	/// 定义FuncQ

	FuncDeclP (func01); /// => double func01(std::string&); ==> FuncDeclP func01;
	FuncDeclP func02;

	FuncDeclQ (func03); /// => void func03(int* p,....);  ==> FuncDeclQ func04;
	FuncDeclQ func04;
	auto lambda_func01 = [](std::string& str) -> double { return atof(str.c_str()); };

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

	/* ---------------------------------------------------------------------------------------------------- */
	// 变参宏(...) VariadicMacros; 复杂的宏使用案例
	// 这里涉及多层次替换, 宏多层次替换过程中的逻辑理解比较困难, 如果试图用typedef代替#define是不太好处理的，至少目前是这样。

#define MACRO_ARGS(...) __VA_ARGS__

#define MACRO_EXPAND(arg_list) arg_list
#define APPLY_VARIADIC_MACRO(macro, tuple) macro tuple

#define ADD_REFERENCE(t) std::reference_wrapper<decltype(t)>(t)
#define ADD_REFERENCE_CONST(t) std::reference_wrapper<std::add_const_t<decltype(t)>>(t)

#define PAIR_OBJECT(t) std::make_pair(#t, ADD_REFERENCE(t))
#define PAIR_OBJECT_CONST(t) std::make_pair(#t, ADD_REFERENCE_CONST(t))

	typedef std::reference_wrapper<int> (WrapperInt);

#define MAKE_TUPLE(...) \
  auto Tuple() { return std::make_tuple(__VA_ARGS__); }
	// 定义函数, 此类函数代替

#define MAKE_TUPLE_CONST(...) \
  auto Tuple() const { return std::make_tuple(__VA_ARGS__); }

#define MAKE_ARG_LIST_1(op, arg, ...) op(arg)

#define MAKE_ARG_LIST_2(op, arg, ...) \
  op(arg), MACRO_EXPAND(MAKE_ARG_LIST_1(op, __VA_ARGS__))

#define MAKE_ARG_LIST_3(op, arg, ...) \
  op(arg), MACRO_EXPAND(MAKE_ARG_LIST_2(op, __VA_ARGS__))

#define MAKE_ARG_LIST_4(op, arg, ...) \
  op(arg), MACRO_EXPAND(MAKE_ARG_LIST_3(op, __VA_ARGS__))

#define MACRO_CONCAT1(A, B) A##_##B
#define MACRO_CONCAT(A, B) MACRO_CONCAT1(A, B)

	/*	MAKE_ARG_LIST(2, PAIR_OBJECT, a, b) 理解如下:
	 * MAKE_ARG_LIST 理解难点在于MACRO_CONCAT(MAKE_ARG_LIST, N), 这里进行拼接, 如N=2则组成MAKE_ARG_LIST_2; 则组成的语句为:
	 *	MAKE_ARG_LIST_2(op, arg, __VA_ARGS__); 这样就形成调用前面MAKE_ARG_LIST_2; 解开MAKE_ARG_LIST_2,则执行:
	 *		op(arg),  macro_expand(make_arg_list_1(op, __va_args__);
	 *		op(arg), make_arg_list_1(op, __va_args__)
	 */
#define MAKE_ARG_LIST(N, op, arg, ...) \
  MACRO_CONCAT(MAKE_ARG_LIST, N)(op, arg, __VA_ARGS__)

#define EMBED_TUPLE(N, ...)                              \
  MAKE_TUPLE(MAKE_ARG_LIST(N, PAIR_OBJECT, __VA_ARGS__)) \
  MAKE_TUPLE_CONST(MAKE_ARG_LIST(N, PAIR_OBJECT_CONST, __VA_ARGS__))

#define META(...) EMBED_TUPLE(GET_ARG_COUNT(__VA_ARGS__), __VA_ARGS__)
	/*
	 *	META(...) 变参宏(...) 需要使用__VA_ARGS__逐层向内传递。
	 *	I. 第一个参数使用 GET_ARG_COUNT(__VA_ARGS__)获取参数个数N; 传递给后续EMBED_TUPLE(N, ...)
	 *	I. 第二个参数使用__VA_ARGS__ 将所有参数全部传递给一层MAKE_TUPLE(...)
	 *	 II. MAKE_TUPLE(...) 展开后为: auto Tuple(){return std::make_tuple(__VA_ARGS__);} 这里首先需要由内向外展开 __VA_ARGS__;
	 *	   III. MAKE_ARG_LIST(N, PAIR_OBJECT, __) 这里make_arg_list是最关键点， MACRO_CONCAT(MAKE...,N)(op, arg, __);
	 *	    拼接(MAKE_ARG_LIST,N)(op, arg,...) 接接语句相当于执行switch N case用法。 这里定义最大为4个参数，则前面必需定义MAKE_LIST为
	 *	    MAKE_ARG_LIST_(1,2,3,4), 层层调用,实现嵌套拼接,组合起来理解确实难,可实现一些骚操作
	 *	   III. MACRO_CONCAT(MAKE_ARG_LIST, N)调用相应N对应的语句; 将参数op, arg, __VA_ARGS__传递过去。 
	 *	   III. op 这里传入的PAIR_OBJECT, 展开后: make_pair("arg", reference_wrapper<decltype(arg)>(arg) 
	 */

	// make_arg_list_x 两种写法: #define,  template<typename ...> ; 可变参数宏类似于template的参数包，但简单很多
#define MAKE_ARG_LIST_X(op, arg, ...) op(arg)

	template <typename T1, typename T2, typename... T3>
	constexpr auto MakeArgListX(T1 op, T2 arg, T3 ...) { return op(arg); }

	// 从上面的种两种定义方式来看,#define语句简化程度高很多的。

#define MY_PRINT(...) printf("[lch]:FILE:%s, Line:%d, Function:%s", __FILE__,__LINE__, __FUNCTION__);
#define APPLY_VARIADIC_MACRO(macro, tuple) macro tuple

	/* 使用宏 计算宏参数个数。 原理:
	 *	技术要点: 可变参宏(...) ==> __VA_ARGS__  例： #define MACRO_ARGS(...) __VA_ARGS__ 直接使用__VA_ARGS__;
	 *	ARG_N(_1, _2, N,...) N 这是计算参数个数的关键, 相当于补码方式, 如一个参数:
	 *		arg_n("str",4,3,2,1,0) 这里的N前面有_1,_2,_3,_4; 那么str落在_1, 4落在_2,3落在_3,2落在_4, 则N落在1；故返回1；
	 *	ARG_EXT 解决vc++展开参数问题
	 *  使用可变参数的关键是 (...) 转换为__va_args__; 很多时候需要多次转换, 层层替代;
	 */
#define ARG_EXT(arg_lists) arg_lists		// 参数展开
#define NUM_SEQUENCE() 4, 3, 2, 1, 0 // 10个参数,则需要反向定义序列
#define ARG_N(_0, _1, _2, _3, _4, N,...) N	// _1,_2... 参数定位, 确定N的位置
#define ARG_COUNT_INNER(...) ARG_EXT(ARG_N(__VA_ARGS__))	// 转换后展开参数
#define GET_ARG_COUNT(...) ARG_COUNT_INNER(0, ##__VA_ARGS__, NUM_SEQUENCE())	//需要多个数据,则需要多次代入参数

	/* ---------------------------------------------------------------------------------------------------- */
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

	struct Test
	{
		int a;
		char b;
		META(a, b)
	};
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
