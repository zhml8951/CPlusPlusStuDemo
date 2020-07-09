#include <utility>
#include <iostream>
#include <string>
#include <tuple>

// stl 标准库的pair, tuple 工具简单了解。

namespace simple_demo
{
	void pair_demo01() // pair可以看成简单key_value, 包含一对值。
	{
		const auto f1 = [](const std::pair<int, const char*> p)-> void {
			std::cout << "pair.first<int>:  " << p.first << ",  pair.second:  " << p.second << "\n";
		};
		const auto f2 = [](const std::pair<const int, std::string> p)-> void {
			std::cout << "pair.first<int>:  " << p.first << ",  pair.second:  " << p.second << "\n";
		};

		std::pair<int, const char*> p(44, "hello");
		f1(p);
		f2(p);

		std::pair<int, int> point{ 2, 5 };
		auto point2 = std::make_pair(3, 5);
	}

	void tuple_demo01() // 名称同Python tuple.
	// pair只可以保存两个值，Key_Value,  tuple 就没有这个限止。
	{
		// point 是三维坐标， 则包括x,y,z;
		std::tuple<double, double, double> point3d_01{ 1.1, 2.2, 3.3 };
		auto point3d_02 = std::make_tuple(3.3, 4.4, 5.5);
		std::tuple<uint16_t, const char*, double> tuple_misc(101, "Name1", 4.85);
		auto tuple_misc02 = std::make_tuple(1002, "Misc02", 6.66);

		// 获取tuple元素个数。
		std::cout << "tuple_misc 元素个数:  " << std::tuple_size<decltype(tuple_misc)>::value << "\n";
		std::cout << std::tuple_size<decltype(point3d_01)>::value << "\n";
		auto point01_elem01 = std::get<0>(point3d_01);
		std::tuple_element<1, decltype(tuple_misc02)>::type misc_name = std::get<1>(tuple_misc02);
		std::cout << point01_elem01 << "\n";
		std::cout << misc_name << "\n";

		// tuple常用操作：
		// std::tuple_size<decltype(tuple_instance)>::value; 获取元素个数
		// std::tuple_element<0, decltype(tuple_instance)>::type elem_value = std::get<0>(tuple_instance);
		// auto elem_value = std::get<0>(tuple_instance);
	}
}

int main(int argc, char* argv[])
{
	simple_demo::pair_demo01();
	simple_demo::tuple_demo01();
}