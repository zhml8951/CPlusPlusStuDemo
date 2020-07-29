#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <memory>

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

	void StringCpy()
	{
		const char* src = "Take a const char* string.";
		/* const char* 与 char*。 在一些编译器上，char* src = "..."可以通过编译，但大多数C++编译器是不可以的。
		 原因：  "Tak....." 是常量即const char* ， 而左值为char*, C语言里是可以使用， 相当于将一个char*指针指向了常量内存区。
		 而C++中 char* lv = const char* rv 涉及到类型转换。会提示如下警告：
		 warning: ISO C++ forbids converting a string constant to 'char*'
		*/

		// 从C++内存分配方面来看， "string" 是常量，分配在常量区，是不可修改的。

		//char* src00 = "Take a char* string.";

		const std::unique_ptr<char[]> dst = std::make_unique<char[]>(std::strlen(src) + 1);
		strcpy_s(dst.get(), strlen(src) + 1, src);
		dst[0] = 't';
		printf("src: %s,  dst: %s\n", src, dst.get());

		const auto src02 = const_cast<char*>(src);
		const auto dst02 = std::make_unique<char*>(new char(std::strlen(src) + 1));
		strcpy_s(*(dst02.get()), strlen(src) + 1, src02);
		(*dst02)[0] = 'M';
		printf("dst2: %s\n", *(dst02.get()));
	}

	void TestReturnPoint()
		/// 这里重点关注： 函数返回数组(即指针时)事项。
	{
		auto char_test1 = []()-> char* {
			char str[] = "char[] string."; // char str[] 数组， 在大部份使用时效果同指针相同，但数据保存在栈上， 返回时栈会被清理，故数据可能是乱码；
			return str;
		};

		auto char_test2 = []() -> char* // 正确的写法： -> const char*

		{
			char* str = "char* string in test2"; // char[] 与char* 效果基本相同，但这里却可以正常返回，关键点在于C的内存模型理解。
			// str是指针类型，保存在栈上，函数返回后会被清理，但右值"char*...."是字符串常量，保存在常量区，不会补清理，故可以正常返回
			// C++ 编译器会抛出警告，原因在于： 右值"char*..."字符串是常量即const char* 而左值char* 类型不匹配。 而早期C/C++是可以运行的。
			return str;
		};

		auto char_test03 = []() -> const char* {
			const char* str = "const char* string in test3";
			return str;
		};

		auto char_test04 = []() -> char* {
			// 这在C++里使用其实是不建议的，返回指针虽然可行，但最好使用传入对象指针，修改对象然后由调用者自行清理。
			char* str_ptr = static_cast<char*>(malloc(sizeof(char) * 100));
			//std::strcpy(str_ptr, "char* use malloc in test04");
			strcpy_s(str_ptr, 100, "String use malloc in test04");
			return str_ptr;
		};

		auto char_test05 = [](char* str_ptr) -> void {
			strcpy_s(str_ptr, 100, "String use input string.");
		};

		auto char_test06 = [](const char* str_ptr) -> void
			// 这里调用看似改变了形参str_ptr的指向，然而调用时，实参的指向并没有改变。说明： 指针传递也是传值，传参时会对实参进行拷贝，而改变实参指向的内容是可行的。
		{
			str_ptr = "String in char_test06";
		}; // 这些地方的纠结，终于明白了为什么C++/C 的指针有多么坑了。

		auto char_test061 = [](const char** str_ptr) -> void
		{
			*str_ptr = "String in char_test06B, modify by char**";
		};// 需要改变指针指向，就需传指针的指针。 也就是**ptr，那就*ptr = "改变指针"；

		auto char_test07 = [](char& str_ref) -> const char*
		{
			const char* str = "const char* string, in test07";
			str_ref = *str;
			return str;
		};

		char* ch_str_ptr;
		ch_str_ptr = char_test1();
		auto str02 = char_test2(); //这里 str02可正常接收返回值。
		auto str03 = char_test03();
		std::cout << ch_str_ptr << std::endl;
		auto str04 = char_test04();
		std::cout << str04 << std::endl;
		free(str04);
		const char* str_07 = nullptr;
		//const char* str_out_07 = char_test07(*(&str_07));
	}
}

int main(int argc, char* argv[])
{
	string_demo::IntToString();
	printf_s("\n-------------------------------------\n");
	string_demo::DoubleToString();
	printf_s("\n-------------------------------------\n");
	string_demo::StringCpy();
}