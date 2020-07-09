#pragma once
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifndef MAJOR_SIZE
#define MAJOR_SIZE 20  // #define C,C++ 宏， 预编译时直接进行字符替换。
#endif

const auto kNameSize = 10; // const常量，最常用定义常量方法。

constexpr auto kIdSize = 8; // constexpr C++11定义常量的常用方法

struct Student
{
	char m_str_name[kNameSize];
	char m_str_id[kIdSize];
	char m_char_sex;
	char m_str_major[MAJOR_SIZE];
};

namespace string_demo
{
	char* str_link_float(const char* src, float num);
}


void stu_main();
