﻿#include <iostream>
#include <memory>
#include <string>

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
		const auto func02_p = func02;
		const Pf func03_p = func03;
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

namespace intelligent_point
{
	// C++ 提供4种智能指针： auto_ptr, unique_ptr, shared_ptr, weak_ptr;(auto_ptr目前已被弃用)

	// unique_ptr 
	template <class T, class Deleter=std::default_delete<T>>
	class UniquePtr;

	template <typename T, typename Deleter>
	class UniquePtr<T[], Deleter>;

	/*
	 * unique_ptr 独立拥有它所指的对象， 在某一时刻只能有一个unique_ptr指向特定对象， 当unique_ptr被销毁(reset)时，
	 * 它所指的对象会自动被销毁。 unique_ptr不允许拷贝构造和赋值操作。只能进行std::move. 也就是右值引用
	 * unique_ptr release()时放弃它所指对象的控制权，将其置空，并返回对象指针，但并不会释放内存。
	 * unique_ptr reset(...) 释放ptr对象，并重置ptr的值。 
	 */

	void UniquePtrTest()
	{
		// unique_ptr 不允许使用赋值，也就是不存在赋值构造

		std::unique_ptr<std::string> str_ptr01(new std::string("unique_ptr string."));

		auto db = new double(88.8);
		std::unique_ptr<double> db_ptr01(db);
		printf("db_ptr01: %.2lf, address: %p\n", *db_ptr01, &(*db_ptr01));
		std::unique_ptr<double> db_ptr02(db);
		printf("db_ptr02: %.2lf, address: %p\n", *db_ptr02, &(*db_ptr02));

		db_ptr01.reset();
		printf("db: %.2lf\n", *db_ptr02);

		std::unique_ptr<float> f_ptr01 = nullptr;
		printf("str: %s,  address: %p\n", str_ptr01->c_str(), &(*str_ptr01));

		auto str01 = str_ptr01.get(); // unique_ptr.get返回对象指针。 str01与str_ptr01指向同一对象。
		printf("str: %s,  address: %p\n", str01->c_str(), str01);

		str_ptr01.release(); // unique_ptr.release 只是释放控制权， 并不销毁对象
		printf("str: %s, address: %p\n", str01->c_str(), str01);

		//printf("str: %s,  address: %p\n", str_ptr01->c_str(), &(*str_ptr01)); //unique_ptr失去对象控制， 
		str_ptr01.reset(nullptr);

		delete db;
		printf("db_ptr02: %.2lf, address: %p\n", *db_ptr02, &(*db_ptr02));
	}

	std::string* UniquePtrString()
	{
		std::unique_ptr<std::string> ptr_str01(new std::string("Test String."));
		std::string* str01 = ptr_str01.get();
		std::unique_ptr<std::string> str02 = std::move(ptr_str01);
		//ptr_str01.release();
		ptr_str01.reset();
		str02.reset();
		return str01;
	}

	class ClassA
	{
	public:
		ClassA(const std::string& name_cs, const std::string& own_name_cs, const int n_val):
			s_name_(name_cs), s_own_name_(own_name_cs)
		{
			if (0 == n_val) {
				std::runtime_error o_rt_ex("nValue can not 0\n");
				throw o_rt_ex;
			}
			else {
				d_value_ = 1.0 / n_val;
			}
			printf("own_name [%s] ClassA Object, Finished construct.\n", this->s_name_.c_str());
		}

		ClassA(const ClassA& be_copy)
		{
			this->d_value_ = be_copy.d_value_;
			this->s_name_ = be_copy.s_name_;
			this->s_own_name_ = be_copy.s_own_name_;
			printf("ClassA copy constructor. \n");
		}

		~ClassA()
		{
			printf_s("ClassA object[%s] ~ClassA Called. object Destroy. \n", this->s_name_.c_str());
		}

		void set_owner_name(const std::string& own_name) { this->s_own_name_ = own_name; }

	private:
		double d_value_;
		std::string s_name_;
		std::string s_own_name_;
	};

	class ClassB
	{
	public:
		ClassB(int val1, int val2, const std::string& name)
		{
			printf("Name: %s ClassB object begin Construct. \n", name.c_str());
			a_ptr1_ = new ClassA("a_ptr1", name, val1);
			a_ptr2_ = new ClassA("a_ptr2", name, val2);
			this->s_name_ = name;
		}
		ClassB(const ClassB& be_copy)
		{
			//TODO some constructor....
			// https://blog.csdn.net/xuyouqiang1987/article/details/104127669?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-3.nonecase&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-3.nonecase#default_delete%3C%3E%E7%B1%BB
		}

	private:
		ClassA* a_ptr1_;
		ClassA* a_ptr2_;
		std::string s_name_;
	};
}

int main(int argc, char* argv[])
{
	test_function_pointer();
	csdn_demo01::test();
	csdn_demo01::demo02_test();
	printf("\n----------------------\n");
	intelligent_point::UniquePtrTest();
	printf("\n----------------------\n");
	const auto str = intelligent_point::UniquePtrString();
	printf("str: %s, address: %p", str->c_str(), str);
	delete str;
}
