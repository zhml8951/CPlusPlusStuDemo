#include <iostream>
#include <functional>
#include <string>
#include <vector>
#include <algorithm>

// std::bind
// std::function 是一种多态，通用的函数封装。

namespace simple_demo
{
	// template<class R, class... Args>
	// ReSharper disable CppMemberFunctionMayBeStatic

	void demo01()
	{
		const struct // C++ 函数内部不能再嵌套函数， 这里采用结构体重载()实现函数功能
		{
			bool operator()(const int a, const int b) const
			{
				return a > b;
			}
		} compare_struct;

		class // C++ 函数内部嵌套class, 重载operator()实现class function. 即<仿函数>
		{
		public:
			bool operator()(const int a, const int b) const
			{
				return a > b;
			}
		} compare_class_func;

		class CompareClass
		{
		public:
			auto compare_member(const int a, const int b) const -> bool
			{
				return a > b;
			}

			static bool compare_static_member(const int a, const int b)
			{
				return a > b;
			}
		};

		auto compare_lambda = [](const int a, const int b) { return a > b; };

		std::function<bool(int, int)> func = compare_struct;
		auto rst = func(88, 99);
		std::cout << "func(88,99): " << std::boolalpha << rst << "\n";

		func = compare_lambda;
		rst = func(66, 44);
		std::cout << "func(88,99): " << std::boolalpha << rst << "\n";

		func = compare_class_func;
		rst = func(88, 99);
		std::cout << "func(88,99): " << std::boolalpha << rst << "\n";

		func = CompareClass::compare_static_member;
		rst = func(88, 99);
		std::cout << "func(88,99): " << std::boolalpha << rst << "\n";

		/*  这里不能直接使用class 成员函数， 需要下面的bind来绑定。
				const CompareClass temp;
				func = temp.compare_member;
				rst = func(88, 99);
				std::cout << "func(88,99): " << std::boolalpha << rst << "\n";
		*/

		// 直接使用std::function<>也可以实现成员函数包装，需要传入参数对象
		CompareClass temp;
		const std::function<bool(CompareClass&, int, int)> func_obj = &CompareClass::compare_member;
		rst = func_obj(temp, 88, 99);
		std::cout << "Use function<bool(Comp&, int, int)> func_obj=&Comp::member_func. \n";
		std::cout << "func(88,99): " << std::boolalpha << rst << "\n";

		func = std::bind(&CompareClass::compare_member, temp, std::placeholders::_1, std::placeholders::_2);
		rst = func(88, 99);
		std::cout << "func(88,99): " << std::boolalpha << rst << "\n";
	}

	void stu02()
	{
		struct Person
		{
			int age;
			std::string name;
		};

		struct PersonFilterFunc : public std::binary_function<Person, std::string, bool>
		{
			bool operator()(const Person& person, const std::string& key) const
			{
				return (person.name.find(key) != std::string::npos);
			}
		};
		// std::bind1st, std::bind2nd 将二元函数转换为一元函数， 有很多库需要一元函数
		const auto display = [](const int val)-> void { std::cout << val << "  "; };
		auto display_v = [](const Person& p) -> void { std::cout << p.age << ", " << p.name << std::endl; };

		int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		using namespace std;
		vector<int> vec;
		copy_if(begin(arr), end(arr), back_inserter(vec), std::bind1st(less<int>(), 6));
		for_each(vec.begin(), vec.end(), display);
		printf("\n");

		vec.clear();
		copy_if(begin(arr), end(arr), back_inserter(vec), bind2nd(less<int>(), 6));
		for_each(vec.begin(), vec.end(), display);
		printf("\n");

		sort(begin(arr), end(arr), greater<int>());
		printf("\n");
		for_each(begin(arr), end(arr), display);

		// 这里测试了内建仿函数， greater, less, 使用过程中实际上创建了匿名对象，调用operator() 来进行比较。
		auto g = std::greater<int>();
		std::cout << std::boolalpha << g(101, 99) << "\n";
		std::cout << std::boolalpha << less<int>()(202, 203) << "\n";

		// 内建仿函数， 根据参数个数不同， 常见两种 unary_function， binary_function;
		//std::unary_function<class Arg, class Result>
		//std::binary_function<class arg1, class arg2, class result>;
		std::vector<Person> vec_person;
		vec_person.push_back(Person{ 1, "Jack" });
		vec_person.push_back(Person{ 2, "Rose" });
		vec_person.push_back(Person{ 3, "Jane" });
		vec_person.push_back(Person{ 4, "Sony" });

		std::vector<Person> vec_ret;
		std::copy_if(vec_person.begin(), vec_person.end(), std::back_inserter(vec_ret),
			std::bind2nd(PersonFilterFunc(), "Ja"));
		std::for_each(vec_ret.begin(), vec_ret.end(), display_v);
	}

	// 自定义仿函数， 比较两个数大小。这个同标准库std::greater相似的。
	template <typename T>
	struct Comp
	{
		bool operator()(T in1, T in2) const
		{
			return (in1 > in2);
		}
	};

	void stu03()
	{
		const Comp<int> comp_int;
		using namespace std;

		cout << "compare int by Comp(88, 99):  " << std::boolalpha << comp_int(88, 99) << "\n";
		cout << "compare by greater(88, 99):  " << greater<int>()(88, 99) << "\n";
		cout << "compare by comp Anonymous function:  " << Comp<double>()(88.9, 88.8) << "\n";
		// greater<int>()(...) 创建了匿名对象，调用匿名对象的operator(), 即实现仿函数的调用。
	}
}

int main(int argc, char* argv[])
{
	simple_demo::demo01();
	simple_demo::stu02();
}

/*
 * STL标准库主要分为六大模块：
 * 容器(Container)
 * 算法(Algorithm)
 * 迭代(Iterator)
 * 仿函(Functor)
 * 适配(Adapter)
 * 配置(Allocator)
 */

 /*
  * 仿函数(Functor) 根据参数不同，分为两大类｛ unary_function(两个参数),  binary_function(一个参数)
  */