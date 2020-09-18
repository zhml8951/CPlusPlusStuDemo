#include "io_stu.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>

char* simple_demo::str_link_float(const char* src, float num)
{
	const auto str_len = strlen(src) + sizeof(num) + 1;
	const auto result = new char[str_len];
	sprintf_s(result, str_len, "%s_%0.4f", src, num);
	return result;
}

namespace simple_demo
{
	void Simple2()
	{
		std::printf("\n");
		const std::string s = "dong";
		const auto a = 520;
		const auto buf = new char[20];
		// ReSharper disable once CppDeprecatedEntity
		_itoa(a, buf, 20);
		std::printf("demo02_test: %s", (s + buf + " | ").c_str());
		//std::cout << s + buf << " | ";
	}

	// tie 在标准库里有两种, ios::tie; 和tuple::tie
	void CinTieDemo()
	{
		// 绑定指针流tie, tie流是输出流，主要作用控制同步，刷新缓充区
		using namespace std;
		ofstream ofs;
		ofs.open("d:\\temp\\ofs_test.txt");

		//cin绑定到ofs; 目的是将cin数据同步到ofs,实现控制台输入数据，实时同步至文件。不使用cin.tie,则输入结束(Ctrl+d或Ctrl+z)才输出至文件
		cin.tie(&ofs);

		// cin.tie() 取出已经绑定到cin的ofs的指针，这里*解引用。 准确理解为*(cin.tie()) <<  默认cin 绑定在cout, cerr;
		*cin.tie() << "String use *cin.tie()"; // *cin.tie() ==>  *(cin.tie()) ==> ofs << ...

		char c;
		while (cin >> c) {
			ofs << c;
		}
		ofs.close();
	}

	void IsfTieOsf()
	{
		std::ofstream osf("d:\\temp\\osf_test.txt");
		std::ifstream isf("d:\\temp\\osf_test.txt");
		std::string str("0");

		osf << "First Second";
		isf >> str;
		std::cout << "Result before tie(): " << str << "\n";
		isf.clear();
		// isf 连接至 osf, 实现osf同步刷新至isf
		isf.tie(&osf);
		isf >> str;
		std::cout << "Result after tie(): " << str << "\n";
	}
}

void DemoMain()
{
	const auto str_01 = simple_demo::str_link_float("TestDemo", 0.888);
	std::printf("demo01_text: %s", str_01);
}