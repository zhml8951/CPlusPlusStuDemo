#include <utility>
#include <iostream>
#include <string>
#include <tuple>

/* stl 标准库的pair, tuple
 *	std::pair 由两个数组组成, 标准的key, value组成，map， multimap中的元素就是pair; 它由两个元素first, second组成,pair是一种泛化的struct;
 *	tuple可以看成pair的泛化，可以容纳最多10种不同的类型的数据；std::get<size_t>(tuple), get<TypeT>(tuple);
 *	tuple 里有tie的使用：
 *		auto tuple1 = std::tie(88, "name1", "store");	将多个元素合并成一个tuple;
 *				
 */

namespace simple_demo
{
	// ReSharper disable CppUseAuto

	void PairDemo01() // pair可以看成简单key_value, 包含一对值。
	{
		const auto f1 = [](const std::pair<int, const char*> p)-> void {
			std::cout << "pair.first<int>:  " << p.first << ",  pair.second:  " << p.second << "\n";
		};
		const auto f2 = [](const std::pair<const int, std::string> p)-> void {
			std::cout << "pair.first<int>:  " << p.first << ",  pair.second:  " << p.second << "\n";
		};

		std::pair<int, const char*> const p{44, "hello"};
		f1(p);
		f2(p);

		const std::pair<int, int> point{2, 5};
		auto point2 = std::make_pair(3, 5);
		std::cout << point.first << point.second << "\n";
		std::cout << (&point2)->first << typeid(point2.second).name() << "\n";
	}

	void TupleDemo01() // 名称同Python tuple.
	// pair只可以保存两个值，Key_Value,  tuple 就没有这个限止, 如果没tuple，则可以使用struct定义结构体，而tuple则可简单将各种类型的值组合起来
	{
		// point 是三维坐标， 则包括x,y,z;
		std::tuple<double, double, double> point3d_01{1.1, 2.2, 3.3};

		// 如果不使用tuple则可以定义一个结构体。这也是C风格标准做法
		typedef struct
		{
			double x;
			double y;
			double z;
		} PointTy, *PointPtr;
		PointTy point3d_s{33, 44, 44};
		PointPtr point3d_p = &point3d_s;

		auto point3d_02 = std::make_tuple(3.3, 4.4, 5.5);
		std::tuple<uint16_t, const char*, double> tuple_misc(101, "Name1", 4.85);
		auto tuple_misc02 = std::make_tuple(1002, "Misc02", 6.66);

		// 获取tuple元素个数。
		std::cout << "tuple_misc 元素个数:  " << std::tuple_size<decltype(tuple_misc)>::value << "\n";
		std::cout << std::tuple_size<decltype(point3d_01)>::value << "\n";
		const auto point01_elem01 = std::get<0>(point3d_01);
		std::tuple_element<1, decltype(tuple_misc02)>::type const misc_name = std::get<1>(tuple_misc02);
		std::cout << point01_elem01 << "\n";
		std::cout << misc_name << "\n";

		// tuple常用操作：
		// std::tuple_size<decltype(tuple_instance)>::value; 获取元素个数
		// std::tuple_element<0, decltype(tuple_instance)>::type elem_value = std::get<0>(tuple_instance);
		// auto elem_value = std::get<0>(tuple_instance);
	}

	struct Person
	{
		std::string first_name;
		std::string last_name;
		int age;
	};

	auto tie_members(const Person& rh) noexcept
	{
		auto rst = std::tie(rh.first_name, rh.last_name, rh.age);
		auto name = std::get<0>(rst) + std::get<1>(rst);
		std::cout << "name: " << name << "\n";

		return rst;
	}

	bool operator<(const Person& lhs, const Person& rhs) noexcept
	{
		auto rst = tie_members(lhs);
		return tie_members(lhs) < tie_members(rhs);
	}

}

int main(int argc, char* argv[])
{
	simple_demo::PairDemo01();
	simple_demo::TupleDemo01();
	simple_demo::tie_members(simple_demo::Person{"Jack","Do", 88});
}
