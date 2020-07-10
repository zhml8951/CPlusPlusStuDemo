#include <iostream>

//指针相关操作记录

double func01(const int num)
{
	printf_s("in func01, num: %d\n", num);
	return static_cast<double>(num);
}

// 函数指针与指针函数的区别 ==>
double* func02(int) = delete;
double (*func03)(int); // double* fun(int)  返回指针类型的函数即<指针函数>。 double (*func)(int) 指向函数的指针即<函数指针>

void test_function_pointer()
{
	// 定义函数指针，参数为int, 返回值double.
	// ReSharper disable CppJoinDeclarationAndAssignment
	double (*p_func01)(int);
	p_func01 = func01;

	printf_s("p_func01 output: %.2lf\n", (*p_func01)(8));

	// 上面的double (*p_func01)(int) =  func01; 可直接auto p_func01 = func01, 即可。
	// 调用方式 （*p_func01)(8) 也可以直接 p_func011(8). 但为了与普通函数区别，采用(*p_func01)(8)比较明了

	const auto p_func011 = func01;
	printf_s("p_func011 output: %.2lf\n", p_func011(8));
}

namespace csdn_demo01
// 函数指针直接使用是看不出本来用途的，只有当定义函数指针作形参，传递函数作实参时可实现回调效果，感觉比js回调还要自然。这才是函数指针的直正用途。
{
	// Begin demo01

	double cal_m1(const int lines)
	{
		std::cout << "called function name:  " << __FUNCTION__ << "\n";
		return 0.05 * lines;
	}

	double cal_m2(const int lines)
	{
		std::cout << "called function name:  " << __FUNCTION__ << "\n";
		return 0.5 * lines;
	}

	void estimate(const int line_num, double (*pf)(int))
	{
		auto ret = (*pf)(line_num);
		std::cout << "num:  " << line_num << ",  need times is: " << ret << std::endl;
	}

	void test()
	{
		auto lines = 10;
		// 函数指针做形参， 通过传不同函数，实现不同结果， 也就是实现函数回调功能。
		estimate(lines, cal_m1);
		estimate(lines, cal_m2);
	}
	// End demo01

	/* ---------------------------------------------------------------------------------------------*/

	// Begin Demo02

	const double* func01(const double arr[], int n);
	const double* func02(const double [], int);
	const double* func03(const double*, int);

	void demo02_test()
	{
		// 采用typedef 自定义类型， C 宏定义类似操作。
		// 这里将函数指针和指针函数结合在一起。

		typedef const double* (*Pf)(const double*, int);
		// ReSharper disable CppUseAuto
		const double* (*func01_p)(const double*, int) = func01;
		// 两种定义方式一样，auto可精简代码，实际编译时会自动转换为 const double* (*func02_p)(const double*, int);
		auto func02_p = func02;
		Pf func03_p = func03;
		// ReSharper restore CppUseAuto

		double arr[5]{11.1, 12.2, 13.3, 14.4, 15.5};

		std::cout << "func01_point:  " << (*func01_p)(arr, 3) << "\n";
		std::cout << "func02_point:  " << func02_p(arr, 3) << "\n";
		std::cout << "*(func03_point):  " << *((*func03_p)(arr, 3)) << "\n";

		const double* (*func_array_p1[3])(const double*, int){func01, func02, func03};
		Pf func_array_p2[]{func01, func02, func03};

		std::cout << "func_p1[0]:  " << func_array_p1[0](arr, 2) << "\n";
		std::cout << "*(func_p2[0]):  " << *(func_array_p2[0](arr, 2)) << "\n";

		std::cout << "func_p1[1]: " << func_array_p1[1](arr, 2) << "\n";
		std::cout << "*(func_p2[1]): " << *(func_array_p2[1](arr, 2)) << "\n";
		// C++ 函数这么写一堆，看着恶心....
	}

	const double* func01(const double arr[], int n)
	{
		return arr + 1;
	}

	const double* func02(const double arr[], int n)
	{
		return arr + 2;
	}

	const double* func03(const double* arr, int n)
	{
		return arr + 3;
	}

	// End Demo02



}

int main(int argc, char* argv[])
{
	test_function_pointer();
	csdn_demo01::test();
	csdn_demo01::demo02_test();
}
