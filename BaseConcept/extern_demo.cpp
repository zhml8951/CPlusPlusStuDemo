#include "extern_demo.h"
#include <iostream>

const char* str01 = "First String for Extern";
char* str02 = const_cast<char*>("Second String no const for Extern");
std::string str03 = "Third std:string for Extern";
std::vector<std::string> vec01{"str01", "str02", "str03"};
int int01 = 888;

const char* int_to_string(const int num)
{
	using std::string;
	auto value = std::to_string(num);
	value += value;
	return value.c_str();
}

namespace extern_simple
{
	std::string str_share = "Can read in some file.";
	int value_c = 88;
	extern int value_a;

	const int kValue6 = 9999;

	//extern template int SumTy1<int, double>(int, double);

	//template double SumTy1<double, int>(double, int);

	void TestSum()
	{
		const auto rst_int = SumTy1<int, double>(88, 88.8);

		const auto rst_double = SumTy1<double, int>(99.9, 99);

		std::cout << "rst: " << rst_int + rst_double << "\n";
	}

	void Func01(int, int)
	{
		value_b = 188;
		std::cout << "static value_b: " << value_b << "\n";
	}

	void Func02(int, int) {}
	void Func04(int const x) { std::cout << "Func04.x: " << x << "\n"; }
}


//int main(int argc, char* argv[])
//{
//	using namespace extern_simple;
//	std::cout << kStrShare << "\n";
//	std::cout << kStrNoExtern << "\n";
//}
int sub(const int num1, const int num2)
{
	return num1 - num2;
}
