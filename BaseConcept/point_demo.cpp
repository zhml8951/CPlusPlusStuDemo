#include <iostream>
#include <memory>
#include <string>
#include <sstream>

//指针相关操作记录

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

	// End Demo02
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
		ClassA(const std::string& name_cs, const std::string& own_name_cs, const int n_val) :
			s_name_(name_cs), s_own_name_(own_name_cs)
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