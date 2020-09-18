#pragma once

#ifndef MAJOR_SIZE
#define MAJOR_SIZE 20  // #define C,C++ 宏， 预编译时直接进行字符替换。
#endif

const auto kNameSize = 10; // const常量，最常用定义常量方法

constexpr auto kIdSize = 8; // constexpr C++11定义常量的常用方法

struct Student
{
	char m_str_name[kNameSize];
	char m_str_id[kIdSize];
	char m_char_sex;
	char m_str_major[MAJOR_SIZE];
};

namespace simple_demo
{
	char* str_link_float(const char* src, float num);

	// std::cin绑定至ofs，将console输入数据同步输出至file;
	void CinTieDemo();
	// ifstream 绑定至 ofstream 实现同步刷新；
	void IsfTieOsf();
}

void DemoMain();
