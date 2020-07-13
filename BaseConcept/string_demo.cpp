#include <iostream>
#include <string>
#include <cstring>
#include <sstream>

// ReSharper disable CppUseAuto

// 操作字符串操作包括C的方式<const char* > 和C++的方式<std::string>

namespace string_demo
{
	void IntToString()
	// use sprintf(buf, buf_size, "format");
	{
		// char* str01 = "string with char*"; // 这样赋值在g++ 出错， ISO C++ forbids converting a string constant to 'char*'
		// 但cl编译可以通过。 这种写法在C中是合法的，但C++中，左边char* 是char类型指针，而右边的"..."是常量。 这里直接赋值会产生类型转换。
		char* str04 = const_cast<char*>("string with const_cast<char*>(...)");
		char str02[] = "string with[]";
		const char* str03 = "string with const char*";
		int num = 333;
		float b = 0.1314f;
		const auto length = strlen(str04) + sizeof(num) + 5;
		char* buf = new char[length];
		sprintf_s(buf, length, "%s__%d", str04, num);
		printf("rst: %s", buf);
		delete[] buf;
		// char* buf, sprintf_s(...) 此方法是纯C方式实现字符串拼接。性能最优，但稍显复杂。
	}

	void DoubleToString()
	{
		const std::string str = "string_c++";
		const auto db = 3.1415926;
		std::ostringstream oss;
		oss << str << "__" << db;
		const auto rst = oss.str();
		printf("rst: %s\n", rst.c_str());

		// c++ string+ to_string(...) 性能差一点儿，确实方便些。 
		printf_s("c++_string: %s\n", (str + std::to_string(db)).c_str());
	}
}


int main(int argc, char* argv[])
{
	string_demo::IntToString();
	printf_s("\n-------------------------------------\n");
	string_demo::DoubleToString();
}
