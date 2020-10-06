#include <iostream>
#include <functional>
#include <string>
#include <map>
#include <vector>

namespace closure_callback
{
	using namespace std;

	auto MakeSimpleClosure(int x) -> std::function<int()>
	{
		// 返回lambda函数， 通过std::function<int()>将函数变成function对象
		return [x]() mutable -> int {
			// mutable 对Capture对象有效，这里指x; 当采用值传递时，增加mutable，可实现在lambda内部修改值x，但不影响外部值
			++x;
			return x;
		};
	}

	std::map<int, std::function<void()>> free_map;

	// 仿函数, 重载operator(); 也是对象函数的一种

	class AddressBook
	{
	public:
		template <typename Func>
		auto FindMatchingAddress(Func func) -> vector<string>
		{
			// 这里需要Func进行说明，否则调用者无法知道如何调用：function<bool(string&)>;
			vector<string> rst;
			for (auto it = this->addresses_.begin(); it != addresses_.end(); ++it) {
				if (func(*it)) {
					rst.push_back(*it);
				}
			}
			return rst;
		}

	private:
		vector<string> addresses_;
	};

	void test_main()
	{
		auto func = MakeSimpleClosure(88);
		std::cout << "func,result: " << func() << "\n";
		std::cout << "func,result: " << func() << "\n";
		std::cout << "func,result: " << func() << "\n";
		func = MakeSimpleClosure(11);
		std::cout << "func,result: " << func() << "\n";
	}
}

int main(int argc, char* argv[])
{
	closure_callback::test_main();
	
}
#define MACRO_EXPAND(...) __VA_ARGS__

#define MAKE_ARG_LIST(op, arg, ...) op(arg)
#define MAKE_ARG_LIST_1(op, arg, ...) op(arg), MACRO_EXPAND(MAKE_ARG_LIST(op, __VA__ARGS__))

template<typename T1, typename T2, typename T3>
constexpr auto MakeArgListX(T1 op, T2  arg, T3  ...) { return op(arg); }

template<typename T1, typename T2, typename T3>
constexpr auto MakeArgListX1(T1 op, T2  arg, T3  ...) { return op(arg), MACRO_EXPAND(MAKE_ARG_LIST(op, __VA__ARGS__)); }

/*
 * C++ 闭包(Closure)即 带上下文的函数, 也就是带状态的函数, 也就是带有类功能的函数; 闭包 = 函数 + 上下文
 *	函数是代码, 状态是一级变量, 将代码和一组变量bind就形成闭包(Closure), 闭包的状态绑定是发生在运行时.
 *	闭包最初由JS推广出来, 函数作为返回值, 调用者取得函数,见JS代码:
 *		// ----------------------------
 *		// 定义外部函数
 *		function make_func(){
 *		  var name = "name01";
 *		  function show_name(){	// 定义闭包函数
 *		    alert(name);
 *		  }
 *		  return show_name;
 *		}
 *
 *		var func = make_func();		// 获取闭包函数, 外部函数内的局部变量name也随函数一起带出来, 外部调用者并不知道确定值.
 *		func();	// 调用函数, 相当于执行了make_func()内的show_name();  这就是闭包在js的基本用法。
 *		// ----------------------------
 *
 *		而C++闭包最常见于lambda; 简单点说:包闭即将函数当成对象作为返回值来使用。 闭包有一重要作用即擦除对象类型
 *		实现闭包C++主要依赖于函数指针和std::function<void(int,)> 及上下文绑定的std::bind;
 *		闭包(Closure)和回调(Callback)是相附的, Callback是将函数做为参数作为传递到函数，
 *		闭包和回调都需将函数转成对象，引入lambda的函数式编程，使用代码简洁，贴近人的思维方式
 *
 *
 */

/*
 *	类型擦除：将原有类型消除或隐藏；主要能提高程序扩展性，消除耦合及一些重复行为。
 *		1. 通过多态擦除类型(通过类派生，用基类的对象调子类行为，这样就不用管子类的具体类型)
 *		2. 通过模板擦除类型(前提是提供类型)
 *		3. 通过某种容器擦除类型（）
 *		4. 通过某种通用类型来擦除（boost.variant类似于java中的object对象）
 *		6. 通过闭包擦除类型
 */
