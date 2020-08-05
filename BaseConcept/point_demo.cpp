#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <functional>

//指针相关操作记录

/* & 在两个元素之间时 位与操作： int a=12, b=11; int c=a&b; 则c=8; 位操作。
 * && 逻辑与操作。 同and
 * & 在类型后接元素声明一个引用变量： float f1 = 200; float& ref_f = f1; func(double& d) 这里声明一个引用。
 * & 在右值方向则是取地址。 int* p = &a;
 * & 引用主要代替部分指针作用，作为参数时可减少拷贝。
 *
 * * 指针。 int* p1 = &a;
 * * 解引用。  *p1 = 88; int num = *p;
 * ** 指针的指针。 int** p2 = &p1;
 * *& 指针引用。 int*& rf2 = p1;
*/

// ReSharper disable CppUseAuto

namespace pointer_simple_demo
{
	void demo01()
	{
		int m_value = 11;
		// 指针在C++属复合类型，它包含两部分内容: 指针本身(4字节32位长整类似)、指针指向的对象。 在函数传参时要注意实际意图.
		// 这类复合类型，可以修改它本身的值(指向别的对象)，也可以修改它指的对象的值。
		auto t1 = [&](int* ptr) // 指针传参对指针本身是值传递(拷贝) //
		{
			printf("before func rewritten.\n");
			printf("pn: %p, *pn: %d\n", ptr, *ptr);
			ptr = &m_value; //注意，这样修改指针ptr的指向，但调用实参的实际指向是不会改变的。
			// 这里修改了ptr指向(指针值)，下面显示可以看到也确实修改了。但仅限函数内部。
			printf("In lambda func rewritten. \n");
			printf("pn: %p, *pn: %d\n", ptr, *ptr);
		};

		int n = 2;
		int* pn = &n;
		printf("pn: %p, *pn: %d\n", pn, *pn);

		t1(pn); // 传t1给pn指针，目标实现改变pn的指向，指针在这里是值传递(拷贝)，所以函数内部修改的是值的拷贝，而实参本身并没有变。
		printf("pn: %p, *pn: %d\n", pn, *pn);
	}

	void demo02() //要修改指针的实际指向(指针的值)，采用指针的指针，即指向指针的指针(同二维数组样式，但有本质区别)
	{
		int m_value = 100;

		auto t2 = [&](int** p) {
			printf("p: %p, *p: %p, **p: %d\n", p, *p, **p);
			*p = &m_value;
			printf("p: %p, *p: %p, **p: %d\n", p, *p, **p);
		};

		int n = 2;
		int* pn = &n;
		printf("pn: %p, *pn: %d\n", pn, *pn);

		t2(&pn);
		printf("pn: %p, *pn: %d\n", pn, *pn);
	}

	void demo03()
	{
		// 判断变量类型，从右向左，离变量名最近的符号确定变量类型，int*& ref. 这里ref是引用类型，引用int指针。即int*& p;
		// int& *p, p指针变量，指向int类型的引用。这在C++是不允许的。
		int m_value = 88;
		auto t3 = [&](int*& p) {
			// 指针引用， 即指向指针的引用
			printf("p: %p, *p: %d\n", p, *p);
			p = &m_value; //同int** p 效果类似，但传引用效率会更高。但要确切理解这里的p
			printf("p: %p, *p: %d\n", p, *p);
		};
		int n = 3;
		int* pn = &n;

		printf("pn: %p, *pn: %d\n", pn, *pn);
		t3(pn);
		printf("pn: %p, *pn: %d\n", pn, *pn);
	}

	void demo04()
	{
		// 这里没有始终没有修改指针的指向，*p解引用，
		int m_value = 488;
		auto t4 = [&](int* p) {
			printf("p: %p, *p: %d", p, *p);
			*p = m_value; // *p 直接修改p指向对象的值。但指针地址
			printf("p: %p, *p: %d", p, *p);
		};

		int n = 4;
		int* pn = &n;
		printf("pn: %p, *pn: %d\n", pn, *pn);
		t4(pn);
		printf("pn: %p, *pn: %d\n", pn, *pn);
	}

	void AllocSpaceDemo()
	{
		const auto alloc_one = [](const int n) -> char* {
			const auto p = static_cast<char*>(malloc(sizeof(char) * n));
			return p;
		};

		char*(*alloc_one_p)(const int n) = alloc_one;

		//使用typedef 定义函数指针类型。可直接代替长串的定义式。
		typedef char*(*GFuncApp)(const int);
		using GFuncPtr = char*(*)(const int);

		GFuncApp alloc_one_p2 = alloc_one;
		GFuncPtr alloc_one_p3 = alloc_one;

		const auto alloc_two = [](char** p, const int n) -> int {
			*p = static_cast<char*>(malloc(sizeof(char) * n));
			return *p == nullptr ? -1 : 1;
		};

		int(*alloc_two_p)(float**, const int) = [](float** ptr, const int n) -> int {
			*ptr = static_cast<float*>(calloc(n, sizeof(float)));
			return *ptr == nullptr ? -1 : 1;
		};
		/// 这些操作，重点是可以明了 typedef 和using比较高阶的用法。 //
		typedef int(*AllocTwoFuncPtr)(float**, const int); /// 使用typedef 定义类型，这里要理解类型处于中间位置时的含义。
		AllocTwoFuncPtr t02 = alloc_two_p;
		typedef std::function<int(double**, const int)> FuncPtrAllocTwo;
		using FuncPtrAllocTwoUsing = std::function<int(double**, const int)>;
		std::function<int(double**, const int)> alloc_two_f = [](double** ptr, const int n)-> int {
			*ptr = static_cast<double*>(calloc(1000, sizeof(**ptr)));
			*ptr = static_cast<double*>(realloc(*ptr, n * sizeof(**ptr)));
			return *ptr ? 1 : -1;
		};

		FuncPtrAllocTwo t2 = alloc_two_f;
		FuncPtrAllocTwoUsing t3 = alloc_two_f;

		auto str1 = "China";
		auto ptr1 = alloc_one(100);
		strcpy_s(ptr1, strlen(str1) + 1, str1);
		printf_s("ptr1: %s\n", ptr1);
		free(ptr1);
		ptr1 = nullptr;
		str1 = "Chinese.";
		if (alloc_two(&ptr1, 100) < 0) {
			exit(-1);
		}
		strcpy_s(ptr1, strlen(str1) + 1, str1);
		printf("after alloc_two(char** p, const int)\n");
		printf("ptr1: %s", ptr1);
		free(ptr1);
	}

	void Alloc2dSpaceDemo()
	{
		const std::function<void*(int, int, int)> alloc2d = [](const int base, const int row, const int col)-> void* {
			void* p = malloc(base * row * col);
			return p;
		};

		void*(*p_alloc2d)(uint8_t, uint8_t, uint8_t) = [](const uint8_t base, const uint8_t row,
			const uint8_t col) -> void* {
				void* ptr = calloc(row * col, base);
				return ptr;
		};

		printf("Test allocate space use *ptr. \n");

		constexpr auto row = 3;
		constexpr auto col = 5;

		const auto arr2d_ptr = static_cast<int*>(alloc2d(sizeof(int), row, col));
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col; j++) {
				*(arr2d_ptr + (col * i + j)) = i + j;
			}
		}

		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col; j++) {
				printf(" ptr:  %d  ", *(arr2d_ptr + (col * i + j)));
			}
			printf("\n");
		}

		int* arr02 = static_cast<int*>(p_alloc2d(sizeof(int), row, col));

		free(arr2d_ptr);
	}

	//内在释放问题测试；
	// 这里重点关注：当我们把指针的指针传进函数，函数内可对指针创建对象new T;也可以创建对象数组new T[10]; 这时我们如果在函数外释放
	// delete时，就需要分别对待了。new[]创建就必须使用delete[], new创建单个对象就用delete;
	// 当然简单类型(int, char, double, string),可以不用考虑直接delete就可以了。 但如果是自定义类型就必须确认创建的方式。
	// 所以最佳实践都是在调用函数外创建对象，函数外释放，保证内存正常释放。传对象引用是最优选择。
	// 最佳实践： 不要在函数内创建对象，返回指针，这是最容易造成内在泄漏的。

	void TestMemoryFree()
	{
		class T
		{
		public:
			T() { printf("T constructor.\n"); }
			~T() { printf("~T destructor.\n"); }
		};

		auto t1_func = [](T** t1) {
			*t1 = new T[10];
		};
		auto t2_func = [](T** t2) {
			*t2 = new T;
		};

		T* ptr1 = nullptr;
		t1_func(&ptr1);
		delete[] ptr1;
		T* ptr2 = nullptr;
		t2_func(&ptr2);
		delete ptr2;

		auto t0_func = [](T** t0, const int n) -> int {
			if (n > 1) {
				*t0 = new T[n];
			}
			else {
				*t0 = new T;
			}
			return *t0 == nullptr ? 0 : 1;
		};

		T* ptr0 = nullptr;
		int n1 = 3, n2 = 1;
		if (n1 > 1 && t0_func(&ptr0, n1)) {
			delete[] ptr0;
			ptr0 = nullptr;
		} else if(n2 == 1 && t0_func(&ptr0, n2)) {
			delete ptr0;
			ptr0 = nullptr;
		} else {
			printf("nothing to do.\n");
		}
	}
} // namespace pointer_simple_demo

namespace csdn_demo01
	// 函数指针直接使用是看不出本来用途的，只有当定义函数指针作形参，传递函数作实参时可实现回调效果，感觉比js回调还要自然。这才是函数指针的直正用途。
{
	double Func01(const int num)
	{
		printf_s("in func01, num: %d\n", num);
		return static_cast<double>(num);
	}

	// 函数指针与指针函数的区别 ==>
	double* Func02(int) = delete;
	double(*p_func03)(int); // double* fun(int)  返回指针类型的函数即<指针函数>。 double (*func)(int) 指向函数的指针即<函数指针>

	void test_function_pointer()
	{
		// 定义函数指针，参数为int, 返回值double.
		// ReSharper disable CppJoinDeclarationAndAssignment
		double(*p_func01)(int);
		p_func01 = Func01;

		printf_s("p_func01 output: %.2lf\n", (*p_func01)(8));

		// 上面的double (*p_func01)(int) =  Demo02Func01; 可直接auto p_func01 = Demo02Func01, 即可。
		// 调用方式 （*p_func01)(8) 也可以直接 p_func011(8). 但为了与普通函数区别，采用(*p_func01)(8)比较明了

		const auto p_func02 = Func01;

		printf_s("p_func011 output: %.2lf\n", p_func02(8));
	}

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

	void estimate(const int line_num, double(*pf)(int))
	{
		const auto ret = (*pf)(line_num);
		std::cout << "num:  " << line_num << ",  need times is: " << ret << std::endl;
	}

	void test()
	{
		const auto lines = 10;
		// 函数指针做形参， 通过传不同函数，实现不同结果， 也就是实现函数回调功能。
		estimate(lines, cal_m1);
		estimate(lines, cal_m2);
	}

	// End demo01

	/* ---------------------------------------------------------------------------------------------*/

	// Begin Demo02

	const double* Demo02Func01(const double arr[], int n);
	const double* Demo02Func02(const double[], int);
	const double* Demo02Func03(const double*, int);

	void Demo02Main()
	{
		// 采用typedef 自定义类型， C 宏定义类似操作。
		// 这里将函数指针和指针函数结合在一起。

		typedef const double* (*Pf)(const double*, int);

		// 3种定义方式一样，auto可精简代码，实际编译时会自动转换为 const double* (*func02_p)(const double*, int);

		const double* (*p_func01)(const double*, int) = Demo02Func01;
		const auto p_func02 = Demo02Func02;
		const Pf p_func03 = Demo02Func03;

		double arr[5]{ 11.1, 12.2, 13.3, 14.4, 15.5 };

		std::cout << "func01_point:  " << (*p_func01)(arr, 3) << "\n";
		std::cout << "func02_point:  " << p_func02(arr, 3) << "\n";
		std::cout << "*(func03_point):  " << *((*p_func03)(arr, 3)) << "\n";

		const double* (*func_array_p1[3])(const double*, int) { Demo02Func01, Demo02Func02, p_func03 };
		Pf func_array_p2[]{ Demo02Func01, Demo02Func02, p_func03 };

		std::cout << "func_p1[0]:  " << func_array_p1[0](arr, 2) << "\n";
		std::cout << "*(func_p2[0]):  " << *(func_array_p2[0](arr, 2)) << "\n";

		std::cout << "func_p1[1]: " << func_array_p1[1](arr, 2) << "\n";
		std::cout << "*(func_p2[1]): " << *(func_array_p2[1](arr, 2)) << "\n";
		// C++ 函数这么写一堆，看着恶心....
	}

	const double* Demo02Func01(const double arr[], int n)
	{
		return arr + 1;
	}

	const double* Demo02Func02(const double arr[], int n)
	{
		return arr + 2;
	}

	const double* Demo02Func03(const double* arr, int n)
	{
		return arr + 3;
	}
}

namespace intelligent_point
{
	// C++ 提供4种智能指针： auto_ptr, unique_ptr, shared_ptr, weak_ptr;(auto_ptr目前已被弃用)

	// unique_ptr
	template <class T, class Deleter = std::default_delete<T>>
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
		ClassA(const std::string& name_cs, const std::string& own_name_cs, const int n_val) : s_name_(name_cs),
			s_own_name_(own_name_cs)
		{
			if (0 == n_val) {
				std::runtime_error o_rt_ex("n_val can not 0\n");
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
		ClassB(const int val1, const int val2, const std::string& name)
		{
			printf("Name: %s ClassB object begin Construct. \n", name.c_str());
			a_ptr1_ = new ClassA("a_ptr1", name, val1);
			a_ptr2_ = new ClassA("a_ptr2", name, val2);
			this->s_name_ = name;
		}

		ClassB(const ClassB& be_copy)
		{
			printf_s("ClassB object name: %s Copy constructor begin construct. \n", s_name_.c_str());
			this->a_ptr1_ = new ClassA(*(be_copy.a_ptr1_));
			this->a_ptr1_->set_owner_name("ptr1 Copy Constructor.\n");
			std::ostringstream oss;
			oss << "Class name " << this->s_name_ << "ClassB object copy constructor occur error. \n";
			std::runtime_error runtime_error(oss.str());
			throw runtime_error;

			this->a_ptr2_ = new ClassA(*(be_copy.a_ptr2_));
			this->a_ptr2_->set_owner_name("ptr2 copy constructor. \n");

			printf_s("Name: %s  ClassB object finished Copy Constructor. \n", this->s_name_.c_str());
		}

		const ClassB& operator=(const ClassB& assign_b) const
		{
			*(this->a_ptr1_) = *assign_b.a_ptr1_;
			*(this->a_ptr2_) = *assign_b.a_ptr2_;
			return *this;
		}

		~ClassB()
		{
			printf_s("Name: %s ClassB object begin Destroy.\n", this->s_name_.c_str());
			delete this->a_ptr1_;
			delete this->a_ptr2_;
			printf_s("Name: %s, ClassB object end Destroy. \n", this->s_name_.c_str());
		}

	private:
		ClassA* a_ptr1_;
		ClassA* a_ptr2_;
		std::string s_name_;
	};

	class ClassUseUnique
	{
	public:
		ClassUseUnique(const int obj1, const int obj2, const std::string& name) : obj_name_(name)
		{
			printf_s("Name: %s ClassUseUnique object begin construct. \n", name.c_str());

			//// unique_ptr 不允许赋值，可使用reset, make_unique, 如果使用赋值，右值引用 unique_ptr<...>(move(new class...)
			obj_ptr1_.reset(new ClassA("obj1_ptr", obj_name_, obj1));
			obj_ptr2_ = std::make_unique<ClassA>(ClassA("obj2_ptr", obj_name_, obj2));

			/*
			std::unique_ptr<ClassA> obj3_ptr;
			obj3_ptr = std::unique_ptr<ClassA>(std::move(new ClassA("obj3_ptr", obj_name_, obj2))); // 使用move

			std::unique_ptr<ClassA> obj4_ptr;
			obj4_ptr = std::move(obj3_ptr); // 使用move可将obj3_ptr对象 直接转移到obj4
			*/
			printf_s("Name: %s ClassUseUnique object begin construct. \n", obj_name_.c_str());
		}

		ClassUseUnique(const ClassUseUnique& copy_obj)
		{
			printf_s("Name: %s ClassUseUnique object begin copy construct.\n", this->obj_name_.c_str());
			obj_ptr1_ = std::make_unique<ClassA>(*copy_obj.obj_ptr1_);
			obj_ptr1_->set_owner_name("obj1_ptr copy constructor from obj");
			std::ostringstream oss;
			oss << "Name:  " << this->obj_name_ << ", copy constructor occur error. \n";
			std::runtime_error runtime_error(oss.str());
			throw runtime_error;

			obj_ptr2_ = std::unique_ptr<ClassA>(std::move(new ClassA(*(copy_obj.obj_ptr2_))));
			this->obj_ptr2_->set_owner_name("obj2_ptr copy constructor from copy_obj\n");

			printf("Name: %s ClassUseUnique object finished copy construct. \n", this->obj_name_.c_str());
		}

		const ClassUseUnique& operator=(const ClassUseUnique& assign_obj2) const
		{
			*(this->obj_ptr1_) = *(assign_obj2.obj_ptr1_);
			*(this->obj_ptr2_) = *(assign_obj2.obj_ptr2_);
			return *this;
		}

	private:
		std::unique_ptr<ClassA> obj_ptr1_;
		std::unique_ptr<ClassA> obj_ptr2_;
		std::string obj_name_;
	};

	void test_cls_ab()
	{
		try {
			ClassB b1(1, 0, "ClassB_b1");
		}
		catch (const std::exception& ex) {
			printf("ClassB b1(1,0) execute occur error... %s\n", ex.what());
		}

		printf("\n----------------------\n");

		try {
			const ClassB b101(1, 2, "ClassB_b101");
			auto b102(b101);
		}
		catch (const std::exception& exception) {
			printf("ClassB b102 execute occur error... %s\n", exception.what());
		}
	}

	void test_cls_use_unique()
	{
		try {
			ClassUseUnique b1(1, 0, "obj011");
		}
		catch (const std::exception& ex) {
			printf("ClassUseUnique object b1 construct occur error. detail: %s \n", ex.what());
		}

		printf("---------------------------------------\n");

		try {
			const ClassUseUnique b2(2, 21, "obj021");
			auto b3(b2);
		}
		catch (std::exception ex) {
			printf("ClassUseUnique object construct occur error. detail:  %s\n", ex.what());
		}

		try {
			ClassUseUnique b3(31, 32, "obj30");
			/*	ClassUseUnique b4(41, 42, "obj4");
				b4 = b3;
			*/
		}
		catch (std::exception ex) {
			printf("ClassUseUnique object assign occur error. %s\n", ex.what());
		}
	}

	class Stark;
	class TargetYen;

	class Stark
	{
	public:
		static void print() { printf("stark love target_yen. \n "); }

		~Stark()
		{
			//delete target_yen_;
			printf("~Stark()\n");
		}

	private:
		TargetYen* target_yen_ = nullptr;
	};

	class TargetYen
	{
	public:
		static void print() { printf("target_yen love stark.\n"); }

		~TargetYen()
		{
			delete stark_;
			printf("~TargetYen()\n");
		}

	private:
		Stark* stark_ = nullptr;
	};

	void SharedPtrDemo01()
	{
		const std::shared_ptr<std::string>
			str_ptr(new std::string("first string shared_ptr, use default constructor."));
		auto str2_ptr = str_ptr;
		printf("str2_ptr use count: %d", str2_ptr.use_count());
		str2_ptr.reset();
		printf("str_ptr use count: %d", str_ptr.use_count());

		std::shared_ptr<double> db1_ptr(std::move(new double(8.8)));
		auto db2_ptr(db1_ptr);
		auto db3_ptr(std::move(db1_ptr));
		printf("db1_ptr use count: %d", db1_ptr.use_count());

		std::shared_ptr<int> int_ptr;
		std::shared_ptr<float> fl_ptr(nullptr);

		const auto pair_ptr = std::make_shared<std::pair<int, double>>(33, 33.0);
		printf("pair_ptr->data(first, second): %d, %lf", pair_ptr->first, pair_ptr->second);
	}
}

int main(int argc, char* argv[])
{
	//test_function_pointer();
	//csdn_demo01::test();
	//csdn_demo01::Demo02Main();
	//printf("\n----------------------\n");
	//intelligent_point::UniquePtrTest();
	//printf("\n----------------------\n");
	//const auto str = intelligent_point::UniquePtrString();
	//printf("str: %s, address: %p", str->c_str(), str);
	//delete str;

	printf("\n----------------------\n");
	intelligent_point::test_cls_ab();
	printf("\n----------------------\n");

	intelligent_point::test_cls_use_unique();
}