#include <iostream>
#include <functional>
#include <string>
#include <vector>
#include <algorithm>

// std::bind
// std::function ��һ�ֶ�̬��ͨ�õĺ�����װ��

namespace simple_demo
{
	// template<class R, class... Args>
	// ReSharper disable CppMemberFunctionMayBeStatic

	void demo01()
	{
		const struct // C++ �����ڲ�������Ƕ�׺����� ������ýṹ������()ʵ�ֺ�������
		{
			bool operator()(const int a, const int b) const
			{
				return a > b;
			}
		} compare_struct;

		class // C++ �����ڲ�Ƕ��class, ����operator()ʵ��class function. ��<�º���>
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

		/*  ���ﲻ��ֱ��ʹ��class ��Ա������ ��Ҫ�����bind���󶨡�
				const CompareClass temp;
				func = temp.compare_member;
				rst = func(88, 99);
				std::cout << "func(88,99): " << std::boolalpha << rst << "\n";
		*/

		// ֱ��ʹ��std::function<>Ҳ����ʵ�ֳ�Ա������װ����Ҫ�����������
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
		// std::bind1st, std::bind2nd ����Ԫ����ת��ΪһԪ������ �кܶ����ҪһԪ����
		const auto display = [](const int val)-> void { std::cout << val << "  "; };
		auto display_v = [](const Person& p) -> void { std::cout << p.age << ", " << p.name << std::endl; };

		int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
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

		// ����������ڽ��º����� greater, less, ʹ�ù�����ʵ���ϴ������������󣬵���operator() �����бȽϡ�
		auto g = std::greater<int>();
		std::cout << std::boolalpha << g(101, 99) << "\n";
		std::cout << std::boolalpha << less<int>()(202, 203) << "\n";

		// �ڽ��º����� ���ݲ���������ͬ�� �������� unary_function�� binary_function;
		//std::unary_function<class Arg, class Result>
		//std::binary_function<class arg1, class arg2, class result>;
		std::vector<Person> vec_person;
		vec_person.push_back(Person{1, "Jack"});
		vec_person.push_back(Person{2, "Rose"});
		vec_person.push_back(Person{3, "Jane"});
		vec_person.push_back(Person{4, "Sony"});

		std::vector<Person> vec_ret;
		std::copy_if(vec_person.begin(), vec_person.end(), std::back_inserter(vec_ret),
		             std::bind2nd(PersonFilterFunc(), "Ja"));
		std::for_each(vec_ret.begin(), vec_ret.end(), display_v);
	}

	// �Զ���º����� �Ƚ���������С�����ͬ��׼��std::greater���Ƶġ� 
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
		// greater<int>()(...) �������������󣬵������������operator(), ��ʵ�ַº����ĵ��á�
	}
}

int main(int argc, char* argv[])
{
	simple_demo::demo01();
	simple_demo::stu02();
}

/*
 * STL��׼����Ҫ��Ϊ����ģ�飺
 * ����(Container)
 * �㷨(Algorithm)
 * ����(Iterator)
 * �º�(Functor)
 * ����(Adapter)
 * ����(Allocator)
 */

/*
 * �º���(Functor) ���ݲ�����ͬ����Ϊ������� unary_function(��������),  binary_function(һ������)
 */
