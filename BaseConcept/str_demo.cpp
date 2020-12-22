#include "str_demo.h"
#include <iostream>
#include <string>
#include <cstring>

namespace string_stu
{
	void FindTest01()
	{
		using namespace std;
		string s1("dada4994befb934493e8f6f70128c0df");
		string flag = "e8";
		auto pos = s1.find(flag);
		if (pos != string::npos) {
			cout << "find flags at:  " << pos << "\n";
		}
		else {
			cout << "Not found the flag(e8)";
		}
		flag = string("cf");
		pos = s1.find(flag);
		cout << "find flag(cf) at: " << pos << "\n";
		std::cout << "pos==string::npos:  " << std::boolalpha << (pos == string::npos) << "\n";

		flag = { "a" };
		auto i{ 0 };
		pos = 0;
		while ((pos = s1.find_first_of(flag, pos)) != string::npos) {
			cout << "position: " << i << pos << "\n";
			pos++;
			++i;
		}
	}

	void FindFirstDemo()
	{
		using std::string;
		string str("look for non-alphabetic characters.....");
		string str2{ "469903 string character." };
		const string find_str{ "abcedfghijklmnopqrstuvwxyz" };
		auto found = str.find_first_not_of(find_str);
		printf("str:  %s", str.data());
		if (found != string::npos) {
			//str.replace(found, 1, "*");
			std::cout << "found pos:  " << found << "\n";
			str[found] = '*';
			std::cout << ", found : " << str[found] << "\n";
			std::cout << str << "\n";
		}

		found = str2.find_first_of(find_str);
		printf("str2_data:  %s", str2.data());
		if (found != string::npos) {
			std::cout << "Found pos: " << found << "\n";
			//str2.replace(found, 1, "x");
			str2.insert(found - 1, "X");
			printf("str2.replaced:  %s\n", str2.c_str());
		}
	}

	void StrModify()
	{
		// 这里注意，rvalue是const char* 类型， lvalue是char*, 也就是类型不匹配。 g++ 直接出错，cl可以编译，直接将str变成const char* ;
		//char* src2 = "char* test2";
		//src2[0] = 'C';
		const auto src = "Test char string first.";
		printf("src: %s", src);
	}

	void string_copy()
	{
		char str01[20];
		char str02[5];

		std::cout << "Input your favorite lines(20):  ";
		std::cin.getline(str01, 20);
		strcpy_s(str02, 20, str01);
		std::cout << "str01:  " << str01 << "\n";
		std::cout << "str02:  " << str02 << "\n";

		char a[100] = "aaaaaabbbbbbbbbbbccccccccccddddddddd";
		char b[] = "Test string: hello,.";
		strcpy_s(a, sizeof(b), b);
		std::cout << "a: " << a << "\n" << "b: " << b << "\n";
		char a1[100];
		char b1[] = "Test string:,.";
		strncpy_s(a1, sizeof(b1), b1, sizeof(a1));
		std::cout << "a1:  " << a1 << "\n" << "b1: " << b1 << "\n";
	}

	void string_sizeof()
	{
		std::string str01 = "#";
		// 由于string内部由相关指针string的sizeof并不能反映string真实值，这里sizeof的值与编译器有关，cl显示24，g++ 显示32；
		std::cout << "str01:  [std::string type]:  \n";
		std::cout << "sizeof(string): " << sizeof(str01) << "\n";
		// string的长度：  str.length 同 str.size()是相同的。
		std::cout << "str1.size():  " << str01.size() << "\n";
		std::cout << "str1.length: " << str01.length() << "\n";

		char str02[] = "#";
		std::cout << "str02:  [C_style, char[]] type: \n";
		// char [] 数组可能通过sizeof得到元素个数，包括结尾'\n'; 而strlen计算到'\n'结束，故char[]字符串长度使用sizeof时要减1；
		std::cout << "sizeof(str02):  " << sizeof(str02) << "\n";
		std::cout << "str_len(str02):  " << strlen(str02) << "\n";

		char* str03 = const_cast<char*>("#");
		std::cout << "str03: [char* type]:  \n";
		// char* 指针， sizeof(...) 直接显示指针的字节数。并不能反映字符串大小。
		std::cout << "sizeof(str03):  " << sizeof(str03) << "\n";
		std::cout << "str_len(str03):  " << std::strlen(str03) << "\n";

		std::string str04(str02); // = std::string(str02);
		std::cout << "str04: [string(char*) type]:  \n";
		// 
		std::cout << "sizeof(str04):  " << sizeof(str04) << "\n";
		std::cout << "str_len(str04):  " << strlen(str04.data()) << "\n";
	}
}

int StrDemoMain()
{
	//string_stu::FindTest01();
	//string_stu::FindFirstDemo();
	//string_stu::string_copy();
	string_stu::string_sizeof();
	return 0;
}

int main(int argc, char* argv[])
{
	int a = 880;
	int b = 999;
	a > b? printf("max: %d", a):printf("max: %d", b);
	StrDemoMain();
}