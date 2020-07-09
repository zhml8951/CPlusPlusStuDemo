#include <utility>
#include <iostream>
#include <string>
#include <tuple>

// stl ��׼���pair, tuple ���߼��˽⡣

namespace simple_demo
{
	void pair_demo01() // pair���Կ��ɼ�key_value, ����һ��ֵ��
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

	void tuple_demo01() // ����ͬPython tuple.
	// pairֻ���Ա�������ֵ��Key_Value,  tuple ��û�������ֹ��
	{
		// point ����ά���꣬ �����x,y,z;
		std::tuple<double, double, double> point3d_01{ 1.1, 2.2, 3.3 };
		auto point3d_02 = std::make_tuple(3.3, 4.4, 5.5);
		std::tuple<uint16_t, const char*, double> tuple_misc(101, "Name1", 4.85);
		auto tuple_misc02 = std::make_tuple(1002, "Misc02", 6.66);

		// ��ȡtupleԪ�ظ�����
		std::cout << "tuple_misc Ԫ�ظ���:  " << std::tuple_size<decltype(tuple_misc)>::value << "\n";
		std::cout << std::tuple_size<decltype(point3d_01)>::value << "\n";
		auto point01_elem01 = std::get<0>(point3d_01);
		std::tuple_element<1, decltype(tuple_misc02)>::type misc_name = std::get<1>(tuple_misc02);
		std::cout << point01_elem01 << "\n";
		std::cout << misc_name << "\n";

		// tuple���ò�����
		// std::tuple_size<decltype(tuple_instance)>::value; ��ȡԪ�ظ���
		// std::tuple_element<0, decltype(tuple_instance)>::type elem_value = std::get<0>(tuple_instance);
		// auto elem_value = std::get<0>(tuple_instance);
	}
}

int main(int argc, char* argv[])
{
	simple_demo::pair_demo01();
	simple_demo::tuple_demo01();
}