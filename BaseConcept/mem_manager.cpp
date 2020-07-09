#include "mem_manager.h"
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>

void alloc_demo01()
{
	using std::vector;
	using std::allocator;
	allocator<int> alloc_int;
	std::cout << alloc_int.max_size() << "\n";
}

int test_allocator_1()
{
	using namespace std;
	allocator<string> alloc;
	const auto n{ 5 };

	const auto p = alloc.allocate(n);
	auto q = p;

	alloc.construct(q++, 3, '1');
	alloc.construct(q++, 10, 'c');
	alloc.construct(q++, "hi");

	cout << *p << "\n";
	cout << p[0] << "\n";
	cout << p[1] << "\n";
	cout << p[2] << "\n";

	cout << "sizeof(p): " << sizeof(p) << '\n';
	cout << "sizeof(*p): " << sizeof(*p) << "\n";

	while (q != p) {
		alloc.destroy(--q);
	}
	alloc.deallocate(p, n);

	return 0;
}

int test_allocator_2()
{
	std::vector<int> vec01{ 1, 2, 3, 4, 5 };
	std::allocator<int> alloc;
	auto p = alloc.allocate(vec01.size() * 2);
	auto q = std::uninitialized_copy(vec01.begin(), vec01.end(), p);
	std::uninitialized_fill_n(q, vec01.size(), 42);
	return 0;
}

void test_allocator_3()
{
	std::printf("\n");
	std::allocator<int> alloc_int;
	std::cout << "alloc_int.max_size(): " << alloc_int.max_size() << "\n";

	const auto array_int = alloc_int.allocate(100);
	std::cout << "array_int[4]: " << array_int[4] << "\n"; // array_int内存由allocator实例alloc_int分配， 但没值没有初始化。
	array_int[4] = 4444; // 初始化方式可直接赋值 array_int[4] = 444
	std::cout << "array_int[4]: " << array_int[4] << "\n";

	// 对象销毁，对于简单内置对象(int, double, float, long, char)这类对象，也可不经过destroy， 直接 alloc_int.deallocate(array_int, 100);
	for (auto i = 0; i < 100; i++) {
		alloc_int.destroy(array_int + i);
	}
	alloc_int.deallocate(array_int, 100);
	std::cout << std::endl;

	std::allocator<double> alloc_double; // 定义 double类型allocator,
	std::cout << "alloc_double.max_size(): " << alloc_double.max_size() << "\n";
	auto const array_double = alloc_double.allocate(10); // 分配空间10，支持double类型。相当于double array_double[] = new double(10)

	alloc_double.construct(array_double, 8.88);
	std::printf("array_double[0]: %.3lf,  initialize by [alloc_double.construct].\n", array_double[0]);
	array_double[3] = 3.33;
	std::printf("array_double[3]: %.3lf,  initialize by [array_double[3] = 3.33]. \n", array_double[3]);
}

void mem_manager_main()
{
	alloc_demo01();
	test_allocator_1();
	test_allocator_3();
	auto array_double = new double[10];

	std::cout << "sizeof(array_double): " << sizeof(array_double) << '\n';
	std::cout << "sizeof(*array_double): " << sizeof(*array_double) << '\n';

	std::printf("[0]: %lf\n[1]: %lf\n\n[2]: %lf\n", array_double[0], array_double[1], array_double[2]);
	//delete array_double;
	std::printf("\n");
	delete[] array_double;

	auto array_float = new float{ 10.0 };
	array_float[100] = 100.100;
	std::printf("[0]: %lf\n[1]: %lf\n\n[100]: %lf\n", array_float[0], array_float[1], array_float[100]);
	std::printf("\n");
	std::printf("[0]: %p\n[1]: %p\n[100]: %p\n", array_float, array_float + 1, array_float + 100);
	delete array_float;

	auto array_long = new long(1024);
	array_long[2048] = 20482048;
	std::printf("array_long:  \n");
	std::printf("[0]:  %ld\n[1]:  %ld\n[2048]:  %ld", array_long[0], array_long[1], array_long[2048]);
	delete array_long;
}

namespace mem_operator
{
	void demo01()
		// void* memchr(const void* buf, int ch, size_t count);
	{
		char str[] = "Example String. ";
		// 从字符串里查找某个字符。
		auto pch = static_cast<char*>(memchr(str, 'S', strlen(str)));
		if (pch != nullptr)
			printf_s("memchr found '%s' .  member at position %d\n", pch, static_cast<int>(pch - str + 1));
		else
			printf("Not found\n");
	}

	void demo02()
		// int memcmp(const void* buf1, const void* buf2, unsigned int count);
		// memcmp 两个string或数组比较，相同返回0， buf1<buf2返回<0; buf1>buf2 返回>0;
	{
		char buf1[] = "String1_name_buf1, compare element buf1. ";
		char buf2[] = "String2_name_buf2, compare element buf2. ";
		auto cmp_ret = memcmp(buf1, buf2, sizeof(buf1));

		printf("compare result num:  %d", cmp_ret);
	}

	void demo03()
		// memcpy 将字节值从源指向的位置直接复制到目标指向的内存块， 返回目的地址。
		// auto memcpy(void* destination, const void* source, size_t num) -> void* {....}
	{
		struct
		{
			char name[40];
			int age;
		} person, person_cpy;

		char first_name[] = "First String name. [first_name].";
		memcpy(person.name, first_name, strlen(first_name) + 1);
		person.age = 88;

		printf("person.name: %s, person.age: %d\n", person.name, person.age);

		// memcpy 将指定&src， 复制到&dst, 大小 sizeof(src);
		// memcpy C语言POD操作，只要知道内存地址和大小，可复制比较等任何数据。

		memcpy(&person_cpy, &person, sizeof(person));
		printf("person_cpy.name: %s, person_cpy.age: %d\n", person_cpy.name, person_cpy.age);
	}

	void demo04()
		// auto memset(void* ptr, int value, size_t num) -> void* {.... }
		// memset 将ptr指向的某一块内存的前num个字节全部设置成value制成的ASCII值， 块大小由第三参数决定，此函数通常为新申请的内存做初始工作。
	{
		char str[] = "almost every programmer should know memset.";
		printf("Original str:  %s\n", str);
		memset(str, '-', 6);
		printf("After memset str: %s\n", str);
		// memset 通常用于初始化某段内存区域。 
		char buf[80];
		memset(buf, 0, sizeof(buf));
		printf("buf: %s", buf);
	}
}

int main(int argc, char* argv[])
{
	mem_operator::demo01();
	printf("\n\n");

	mem_operator::demo02();
	printf("\n\n");

	mem_operator::demo03();
	printf("\n\n");

	mem_operator::demo04();


}