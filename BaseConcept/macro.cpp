#include <functional>
#ifndef MACRO_HPP_
#define MACRO_HPP_
// #ifndef ...  #define .... #endif 为了避免同一文件被多次#include;
// #pragma once		// 功能 #ifndef .... #define ....#endif;
/*
 * C++ 宏定义(预处理)
 *
 */
#include <iostream>

 // 简单字符串替换
#define PI 3.1415
#define NAME "arnni"
#define AREA(r) PI*r*r
#define STR_PTR const char*
#define P_CHAR char*

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
		auto area = AREA(r); // ==> PI*r*r ==> 3.1415 * 3.3 * 3.3;
		printf("r: %.2f,  area: %.2f\n", r, area);
		auto b = 4.4f;
		printf("max(a,b): %f\n", MAX(b, r));
		std::cout << "date: " << __DATE__ << ", file:  " << __FILE__ << "\n";
		std::cout << "func:  " << __FUNCTION__ << ",  line:  " << __LINE__ << "\n";
		std::cout << "timestamp: " __TIMESTAMP__ << ", time: " << __TIME__ << "\n";
	}

	void Demo02()
	{
		const char* sz = "user";
		const char* zh = "Zh";

		const auto LINK_NAME(user) = "define";
		std::cout << LINK_NAME(user) << "\n";
		OUT_VAL(user);
		OUT("zh");
		std::cout << "str:  " << TO_STR(use_STR) << "\n";
	}

	void Demo03()
	{
#define FILE(a) {a, #a}
		enum IDD { OPEN, CLOSE };
		typedef struct MSG
		{
			IDD id;
			const char* msg;
		} Msg;

		MSG msg_arr[] = { FILE(OPEN), FILE(CLOSE) }; //展开后:  msg_arr = { {OPEN, "OPEN"}, {CLOSE, "CLOSE"} };
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
		static char FILE_NAMES[] = GET_FILE_NAME(__FILE__);

		// 得到一个数组类型所对应的字符串缓冲大小
#define TYPE_BUF_SIZE1(type) sizeof #type
#define TYPE_BUF_SIZE(type) TYPE_BUF_SIZE1(type)
		char buf[TYPE_BUF_SIZE(INT_MAX)];
	}

	// 这里使用内置宏__LINE__; 一级宏不会展__LINE__实际值， 通过一层宏再调用二级宏展开__LINE__值。

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

	private: // 禁止执行拷贝构造和拷贝赋值。
		ScopeGuard(ScopeGuard const&);
		ScopeGuard& operator=(ScopeGuard const&);
	};

	void Demo05()
	{
		auto cb = [&]()-> void { printf("ok"); };
		ScopeGuard sc01([&] { printf("Ok"); });
		ScopeGuard sc02([&]()-> void { printf("ScopeGuard"); });

		ON_SCOPE_EXIT([&] {printf("ON_Scope_exit"); }); // ScopeGuard EXIT_140([&]{printf("ON_Scope_exit");});
		ON_SCOPE_EXIT(cb); //ScopeGuard EXIT_141(cb);
	}
}
#endif

// g++ -E macro.cpp -o macro.i 不运行编译，只进行宏展开命令;

int main(int argc, char* argv[])
{
	macro_demo::Demo01();
	printf("\n--------------------------\n");
	macro_demo::Demo02();
	return 1;
}