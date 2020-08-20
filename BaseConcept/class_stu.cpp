#include <iostream>

namespace simple_demo
{
	class ClsTest
	{
	public:
		//ClsTest() = default;
		explicit ClsTest() : num_(1)
		{
			std::cout << " Class called ClsTest(). num: " << this->num_ << "\n";
		}

		explicit ClsTest(const int num) : num_(num)
		{
			std::cout << "Class called ClsTest(int). num:" << this->num_ << "\n";
		}

		void test_func() const
		{
			std::cout << "Output member variable(num):  " << this->num_ << '\n';
		}

		// 成员函数特殊命名方式， 等同于 constexpr int get_num() const {.... }
		constexpr int (get_num)() const
		{
			return this->num_;
		}

	private:
		int num_;
	};

	class ClsTest02
	{
	public:
		ClsTest02() = default; // default定义默认初始函数，用处不大
		ClsTest02(const ClsTest02&) = delete; // delete 删除函数， 也就是不可以使用拷贝构造函数

	private:
		int num_{101}; // 定义成员变量，并设置初始值。
	};

	void DemoMain01()
	{
		ClsTest cls01; // 调用无参构造，创建对象在栈中。
		ClsTest cls02{}; // 等同于上面什么都不写。
		ClsTest cls04{3}; //C++11 新写法 ==> ClsTest cls04(3) .
		ClsTest cls03(); //	可理解为返回值为ClsTest类型的函数cls03, cls03无参; 这里直接警告。
		auto cls05 = new ClsTest();
		ClsTest* cls06 = new ClsTest;
		ClsTest* cls07 = new ClsTest{88};
		std::cout << "get_num: " << cls07->get_num() << "\n";

		delete cls05;
		delete cls06;
		delete cls07;
	}

	/*
	 *	函数重载：在一个访问区内被声明的几个具有不同参数列(参数类型，个数，顺序)的同名函数， 根据参数列表确定调用哪个函数。重载不管返回类型
	 *  函数可以重载， 运算符也可以重载（个人感觉运算符重载应该称运算符重写）。
	 *  重载发生在相同访问区，全局函数，同一namespace，或同class中。 相同名称，但参数不同称之为重载
	 *  重载时 顶层const是被忽略的。如func(int i), func(const int i) 这里不能算重载。
	 *  重载时 指针的const要区别对待， 顶层const和底层const处理方式不同。
	 *  重载时 const引用与普通引用是不同参数，可以进行重载。
	 *  重载时 const函数与非const函数是重载。
	 */

	class ClsConst
	{
	public:
		explicit ClsConst(const int i) : x_(i) {}
		// 这里的func() const 与 func() 是不同的。 // 故有时ReSharper提示添加const的时候还要谨慎对待
		void func() const { std::cout << "func() const called." << this->x_ << "\n"; }
		void func() { std::cout << "func() called." << this->x_ << "\n"; }

		void func_int(int i)
		{
			this->x_ = i;
			std::cout << "func_int(int): " << this->x_ << "\n";
		}

		void func_int(const int* p_i)
		{
			this->x_ = *p_i;
			std::cout << "Called func_int(const int*). " << this->x_ << "\n";
		}

		void func_int(int* p_i) const //void func_int(int* const p_i);
		{
			*p_i = this->x_;
			std::cout << "Called func_int(int*). " << this->x_ << "\n";
		}

		//void func_int(const int i){}   // 顶层const,不能作为重载条件 即const int i ==> int i;

		//void func_int(int i) const { this->x_ = i; }  // const成员函数不能修改成员变量。

		// 底层const; char* 与const char* ; 可以重载，这里判断为不同参数。
		void func_char(char* c_str) { std::cout << "Called func_char(char*) ; " << this->x_ << "\n"; }
		void func_char(const char* c_str) { std::cout << "Called func_char(const char*);" << this->x_ << "\n"; }
		//void func_char(char* c_str) const { std::cout << "Called func_char(char*) const; " << this->x_ << "\n"; }
		//void func_char(char* const c_str);	// char* const 顶层const 认作为char* 相同函数。 故不能算作重载
		/*
		 * 函数重载时const的引入，使用重载变得复杂。 形参是顶层const 传入实参时并不一定需要const变量。区别在于实参替换到形参时到函数内部变成const变量
		 * 当形参为指针类型， 底层const 对实参要求是const. 而顶层const表明变量在函数内部的const属性，而不要求实参一定const;
		 */

		void func_double(double& d)
		{
			this->x_ = d;
			std::cout << "Called func_double(double& ).  " << this->x_ << "\n";
		}

		/*
		 *  const reference(const 引用） 与 非const 引用, 是不同参数，可进行重载.
		 *  形参const ref:  func(const double& d) 实参可以是 const变量，也可以是 非const变量；
		 *  形参 非const ref:  func(double& d) 那么实参必须不能是const; 相当于const变量需要转换为非const变量；
		 */

		void func_double(const double& d)
		{
			this->x_ = d;
			std::cout << "Called func_double(const double&). " << this->x_ << "\n";
		}

	protected:
		int x_;
	};

	void ClsConstMain()
	{
		ClsConst t1(10);
		const ClsConst t2(20);
		// t2为const, t2非 const; 从下面代码跳转可以看出，func跳转到不同函数。也是调用函数不同。
		t1.func();
		t2.func();

		auto i1 = 88;
		//t1.func_int(&i1);			// 底层const => func_int(const int* i) 实参为必需为const; 理解 底层const代表 指针指向const变量。顶层const代表指针本身是常量
		t2.func_int(&i1);

		const auto i2 = 99;
		t1.func_int(i2);
		t1.func_int(&i2);

		auto d1 = 1.01;
		t1.func_double(d1);
	}

	/*
	 * 重写(覆盖)， 隐藏。  重载 3者的关键区别
	 * 重载在同一访问区，同class内，namespace内及全局函数，同名函数，参数不同。属静态。
	 * 重写，和隐藏 发生在类继承关系上。 属动态。
	 * 重写要求是函数名，参数等必须相同，基类必须是virtual成员函数。  重写当然也有隐藏属性。
	 * 重写(覆盖)可实现基类指针指向子类，直接调用子类方法，基类需要virtual函数才能完全满足要求。
	 * 如果没有virtual， 即使用名称，参数等全部相同，也不能实现重写， 只能算隐藏。
	 * 子类与基类 函数名相同，参数相同或不同都是隐藏基类方法， 使用基类指针指向子类，但调用依然是基类方法。
	 */
	class Base01
	{
	public:
		virtual ~Base01() = default;
		void func(double, int) const { std::cout << "Base::func(double,int)" << "\n"; }
		virtual void f(const float f) { std::cout << "Base::f(float f)" << f << "\n"; }
		void g(const float f) { std::cout << "Base::g(float)" << f << "\n"; }
		void h(const float f) { std::cout << "Base::h(float)" << f << "\n"; }
	};

	class Derive final : public Base01
	{
	public:
		void func(int) { std::cout << "Derive::func(int)" << std::endl; }
		void f(float f) { std::cout << "Derive f(float)" << "\n"; }
		void g(int x) { std::cout << "Derive g(int)" << "\n"; }
		void h(float x) { std::cout << "Derive h(float)" << "\n"; }
	};

	void Main02()
	{
		Derive pd;
		pd.func(1); // func 隐藏了Base的func;
		//pd.func(0.1, 1);

		Base01* bs01 = &pd;
		bs01->func(0.1, 1);

		Derive* df = &pd;
		df->func(10);

		bs01->f(3.14f);
		df->f(3.14f);
	}

	/*
	 * C++ 有4个特殊成员函数(默认): 构造函数、析构函数、拷贝构造、拷贝赋值；如果程序没显示创建，则编译器自动隐式创建这些函数；
	 * 但是如果程序创建了其它参数的构建函数，则隐式的默认构建函数则不会自动产生，创建对象就必须显式调用有参构建函数，
	 * 如果需要编译器生成默认构建函数， 就在函数后 =default, 实现内容由编译器自动产生；如下: Student()=default;
	 * 
	 *  有时候程序需要禁用某成员函数，过去的方式则是将成员成成private, C++11直接提供delete; 
	 *  Student& operator=(const Student&)=delete; 显式提示禁用些operator=
	 */

	class Student
	{
	public:
		Student() = default;
		explicit Student(const std::string name): name_(name), sex_('M'), age_(0) {};

		Student(Student const& stu) { this->name_ = stu.name_, sex_ = stu.sex_, age_ = stu.age_; }
		Student& operator=(const Student&) = delete;
	private:
		std::string name_;
		char sex_;
		uint8_t age_;
	};
}

int main(int argc, char* argv[])
{
	//simple_demo::DemoMain01();
	simple_demo::Student stu01;
}
