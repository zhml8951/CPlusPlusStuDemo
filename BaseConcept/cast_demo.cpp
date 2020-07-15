// C++ static_cast<>(), const_cast<>, dynamic_cast<>.., reinterpret_cast<>()...//
// C++ 类型转换， 优化了C语言式的强制转换方式， （int)3.5 或double(3);
//
#include "cast_demo.h"
#include <iostream>
#include <string>
#include <cstdio>

void cast_stu::demo01()
{
	// static_cast<> 强转， 静态转换，常用于简单变量转换。
	class ClsA
	{
	public:
		explicit operator int() const { return 1; };
		explicit operator const char*() const { return "ok"; }
	};

	using namespace std;

	const ClsA cls01;
	//char* ch =(char*)"New Dragon Inn";

	auto n = static_cast<int>(3.1415);
	std::printf("\nn: %d\n", n); // 3
	n = static_cast<int>(cls01); // 实际调用operator (); 返回1(int)；
	std::printf("n: %d\n", n); // 1
	const auto ch = static_cast<const char*>(cls01); // 调用operator (), 返回 char*;
	std::printf("ch: %s\n", ch);
	//n = static_cast<int>(ch);				// 直接提示错误，不能这样进强制转换
	//ch = static_cast<char*>(n);
	printf_s("cast_demo01 end.\n\n");
}

void cast_stu::demo02()
{
	class ClsB
	{
	public:
		// class 元素申明顺序也可以影响后面程序的运行，如果将ch_或str_放第一位， reinterpret_cast<int&> 就不可以正常运行。
		int i_;
		int j_;
		char ch_[200]{};
		const char* str_ = nullptr;
		std::string string01_;

		explicit ClsB(const int n) : i_(n), j_(n)
		{
			sprintf_s(ch_, sizeof(ch_) / sizeof(ch_[0]), "%d sprintf_s()__ok", n);

			auto str_n = std::to_string(n);
			str_n = "n_" + str_n + "::to_string()__ok.";
			str_ = str_n.c_str();
			std::cout << str_ << "\n\n";

			string01_ = std::to_string(n);
			string01_ += "_num,to_string__ok";

			//// 这里_BufferCount: 应该与前面的字符数组应该一样， 数组越界， 常见的是程序崩溃.
			//sprintf_s(ch_, 200, "%d,....sprintf_s()__ok, if the string is too long as along as here. ", n);
			//std::cout << "ch:  " << ch_ << "\n";
			//std::cout << "ch.length:  " << sizeof(ch_) / sizeof(char) << "\n";
			//for (auto i = 0; i <= sizeof(ch_) / sizeof(char); i++) {
			//	printf_s("%c ", ch_[i]);
			//}
		}

		const char* get_str() const
		{
			return this->str_;
		}
	};

	ClsB cls02(48);
	std::printf("cls02.i: %d, cls02.j: %d\n", cls02.i_, cls02.j_);
	printf_s("string01: %s\n", cls02.string01_.c_str());
	printf_s("str: %s\n", cls02.str_);

	// reinterpret_cast<int &> 这样的将对象转引用，需要非常小心， 它依赖于对象内的元素顺序，
	// 这里cls02的第一个元素刚好是int,这里r 引用了i_。 如果将string类型或char[]放第一位则出现无效数。
	auto& r = reinterpret_cast<int &>(cls02);
	std::printf("r: %d\n", r);
	r = 200;
	std::printf("cls02.i: %d,  cls02.j: %d\n", cls02.i_, cls02.j_);
	std::cout << "cls02.i_: " << cls02.i_ << ", j_: " << cls02.j_ << ", ch_: " << cls02.ch_
		<< ", str_: " << cls02.str_ << "\n";
	// 这里r2 是char& 但cls02第一无素int这样进行逐bit转换是无效的。
	auto& r2 = reinterpret_cast<char &>(cls02);
	std::cout << "r2=reinterpret_cast<char&>: " << r2 << "\n";

	auto num = 300;
	auto* cls03 = reinterpret_cast<ClsB*>(&num);
	cls03->i_ = 400;
	cls03->j_ = 500;
	cls03->str_ = "ok";
	std::cout << "num:  " << num << "\n";
}

void cast_stu::demo03()
{
	// const_cast<> demo
	// const_cast<>()  仅用于去除const 属性的转换
	using namespace std;
	const std::string s1 = "Inception ";
	auto& ref_s1 = const_cast<string&>(s1);
	auto p_s1 = const_cast<string*>(&s1);
	cout << "s1: " << s1;
	cout << "ref_s1: " << ref_s1 << "\n";
	cout << "p_s1: " << p_s1 << ";  *p_s1: " << *p_s1 << '\n';
	cout << "Modify *p_s1.\n";
	*p_s1 = "String modify by *p_s1.";
	cout << "p_s1: " << p_s1 << ";  *p_s1: " << *p_s1 << '\n';
	cout << "ref_s1: " << ref_s1 << "\n";
}

void cast_stu::demo04()
{
	// dynamic_cast<>() demo
	// dynamic_cast 主要用于将多态基类的指针和引用强制转换为派生类的指针和引用， reinterpret_cast也可进行此类转换，但不进行安全检查。
	// dynamic_cast 可实现reinterpret_cast相同功能，并且进行安全性检查。

	class Base01
	{
	private:
		int base_a_;
	protected:
		const char* base_str_ = nullptr;
	public:
		explicit Base01(const int n) : base_a_(n)
		{
			const auto s = std::to_string(n);
			this->base_str_ = s.c_str(); // std::to_string(n).c_str(); 直接使用这类调用尽然不可以。
			printf_s("Init object in base\n");
			std::cout << "str: " << base_str_ << ", base_int_a: " << base_a_ << "\n";
		}

		virtual ~Base01() { base_a_ = 0; } // ~ 对象销毁这样操作只是测试，没有实际用处。

		virtual int get_a() const { return this->base_a_; }

		const char* get_str() const { return this->base_str_; }
	};

	class Derived : public Base01
	{
	private:
		int derived_a_;
	public:
		explicit Derived(const int n) : Base01(n), derived_a_(n)
		{
			printf_s("Initial object in derived \n");
			auto s = std::to_string(n);
			s += s;
			base_str_ = s.c_str(); // (std::string(base_str_) + s).c_str(); ## python 链式调用不能使用，需要中转变量。
			std::cout << "Derived._base_str: " << this->base_str_ << ", this->a: " << this->derived_a_ << "\n";
		}

		~Derived() { derived_a_ = 0; }

		int get_a() const override { return this->derived_a_; }

		const char* get_info() const { return this->base_str_; }
	};

	Base01 b01(88);
	std::cout << "b01.get_str(virtual):  " << b01.get_str() << "\n";
	Derived d01(99);
	Derived* d02 = nullptr;
	d02 = reinterpret_cast<Derived*>(&b01); // 子类指针强制转向父类，是不安全的。 这同java等语言继承相同的。
	if (d02 == nullptr)
		std::cout << "Unsafe reinterpret_cast. " << "\n";
	else {
		std::cout << "d02.get_a:  " << d02->get_a() << "\n";
		std::cout << "d02.get_info: " << d02->get_info() << "\n"; // 指向父对象，父对象没get_info函数，返回为空
		std::cout << "d02.get_str: " << d02->get_str() << "\n"; // 父对象get_str是virtual函数，没有实际执行。
	}

	d02 = dynamic_cast<Derived*>(&b01);
	if (d02 == nullptr)
		std::cout << "unsafe convert in dynamic_cast is do nothing... \n";

	Base01* b02 = dynamic_cast<Derived*>(&d01);
	if (b02 == nullptr) std::cout << "here is not except. ";
	std::cout << "Base01 (*b02 = &d01), str:  " << b02->get_str() << ", a: " << d01.get_info() << "\n";
	printf("demo04 end.");
}


//int main(int argc, char* argv[])
//{
//	cast_stu::demo04();
//}