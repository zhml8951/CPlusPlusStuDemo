#pragma once
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifndef MAJOR_SIZE
#define MAJOR_SIZE 20  // #define C,C++ �꣬ Ԥ����ʱֱ�ӽ����ַ��滻��
#endif

const auto kNameSize = 10; // const��������ö��峣��������

constexpr auto kIdSize = 8; // constexpr C++11���峣���ĳ��÷���

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
