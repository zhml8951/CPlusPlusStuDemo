#include <iostream>

/*
 * C++ 类型推导主要有auto, decltype；
 *		auto 忽略顶层const, decltype保留顶层const; 
 *		对引用操作，auto推导出原有类型，decltype推断出引用 
 *		解引用操作， auto推展出原有类型，decltype推断出引用 
 *		auto推导时会实际执行， decltype不会执行，只做分析 
 *	decltype主用于模板操作。模板函数的操作类型依赖于其模板参数类型。
 *		decltype在模板使用过程中经常结合declval
 */


namespace type_demo
{
	void OfficialDemo01()
	{
		auto fn01 = [](int)-> int {
			return int();
		};
		typedef int (&FnRef)(int);
		typedef int (*FnPtr)(int);

		struct FnClass
		{
			int operator()(const int i) const { return i; }
		};

		typedef std::result_of<decltype(fn01)&(int)>::type Ty1;
		Ty1 var_a;

		std::result_of<decltype(fn01)&(int)>::type var_b;

		using Ty2 = std::result_of<FnRef(int)>::type;
		Ty2 var_c;

		std::result_of<FnClass(int)>::type var_d;

		Ty1 var_x;
		std::result_of<decltype(fn01)(int)>::type(var_x);

		std::cout << std::boolalpha;
		std::cout << "typedefs of int: " << "\n";
		std::cout << "a: " << std::is_same<int, decltype(var_a)>::value << "\n";
		std::cout << "b: " << std::is_same_v<int, decltype(var_b)> << "\n";
		std::cout << "c: " << std::is_same<int, Ty2>::value << "\n";
		std::cout << "d: " << std::is_same<int, decltype(var_d)>::value << "\n";
	}
}

int main(int argc, char* argv[])
{
	type_demo::OfficialDemo01();
}

