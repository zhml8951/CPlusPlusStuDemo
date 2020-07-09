#include "io_stu.h"

char* string_demo::str_link_float(const char* src, float num)
{
	const auto result = new char[strlen(src) + sizeof(num) + 1];
	// ReSharper disable once CppDeprecatedEntity
	sprintf(result, "%s_%0.4f", src, num);
	return result;
}

void stu_main()
{
	const auto str_01 = string_demo::str_link_float("TestDemo", 0.888);
	std::printf("demo01_text: %s", str_01);
}

void stu_main02()

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
