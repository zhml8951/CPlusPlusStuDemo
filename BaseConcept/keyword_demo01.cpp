#include <iostream>
#include <vector>
#include <string>

// C++ 关键词及关键技术点记录

/*
 *	explicit(显式) C++11只用于修饰构造函数；与explicit对应的是implicit; 构造函数默认都是implicit;
 *	构造函数默认的implicit， 当单参调用implicit的构造函数, 通常称之为 转换构造函数(converting constructor),这个带来一定方便性，但可能会带来意外的隐式类型转换
 *	explicit 不需要添加默认构造，多参构造(总是显示构造)。只需要添加单参构造，目的是防止不必要的隐式类型转换
 *	需要注意单参构造（多参构造但只有一个参数没有默认值）时，采用赋值时会产生隐式类型转换，这类转换有时并不是我们需要的。 explicit阻止隐式类型转换；
 */
/* ReSharper 可提供最佳代码实践， 如下： NonExplicitConvertingConstructor, NonExplicitConversionOperator, UseAuto... 这里先禁用这些*/

// ReSharper disable CppNonExplicitConvertingConstructor
// ReSharper disable CppNonExplicitConversionOperator
// ReSharper disable CppUseAuto

using namespace std;

namespace keywords_simple
{
	/*
	 *	本例主演示explicit 在class的作用.  总括一句话，explicit防止不必要的隐式类型转换
	 */
	class FractionNoExplicit
	{
	public:
		FractionNoExplicit(const int num, const int den = 1) : numerator_(num), denominator_(den)
		{
			printf("Called constructor.\nnumerator: %d, denominator: %d\n", num, den);
		}

		FractionNoExplicit(const FractionNoExplicit& o_explicit)
		{
			printf("Copy constructor.\n");
			this->numerator_ = o_explicit.numerator_;
			this->denominator_ = o_explicit.denominator_;
		}

		FractionNoExplicit& operator=(const FractionNoExplicit& o_explicit)
		{
			printf("Copy assignment constructor.\n");
			this->numerator_ = o_explicit.numerator_;
			//this->denominator_ = o_explicit.denominator_;
			//为了测试，将copy assignment; 此类成员变量都是基础类型数据时不需要自定义copy assign function. 编译器自动生成copy assign 可完成相同任务，
			//当类成员变量是复合类型(指针，引用)或其它结构，则需要考虑copy assignment function.

			return *this;
		}

		/*
		 * type conversion operator function. (类型转换操作符函数)， 没有explicit可自动隐式转换
		 */
		operator double() const
		{
			printf("Called operator conversion function.\n");
			return static_cast<double>(numerator_ / denominator_);
		} ///type conversion function

		void Display() const
		{
			printf("numerator/denominator = %d/%d\n", this->numerator_, this->denominator_);
		}

	private:
		int numerator_;
		int denominator_;
	};

	void no_explicit_test()
	{
		FractionNoExplicit f1(3, 5);
		double rst = 3.0 + f1; // f1产生隐式类型转换，调用operator double()， 这里f1有双重类型， 即是类类型，也是double类型

		f1 = 88; // 这里调用FractionN..(单参）构造, 同样也是隐式类型转换, 调用实质样式： f1 = FractionNoExplicit tmp(88);
		// 先调用Constructor, 再调用Copy assignment Function; 调用的实质是构造函数是单参，可进行隐式类型转换，也就将整型隐式转换为FractionNoEx...;
		f1.Display();
	}

	/*
	 * 类型转换分为: 提升转换(Promotion), 标准转换， 自定义转换；
	 *	提升转换: char,unsigned char,short,unsigned short 自动提升到int, unsigned int; float类型提升到double;
	 *	标准转换: int 转char, long 转double..
	 */

	/* C++ 一致初始化， 早期C++初始化有多种方式，很容易混淆。故引用列表初始化(一致初始化)｛..｝
	 * C++11 列表初始化(一致初始化) 内部实现的本质是用 initializer_list<T>来实现，而initializer_list<>实质也就是array 
	 */

	void Print(const std::initializer_list<int> values)
	{
		for (auto p = values.begin(); p != values.end(); ++p) {
			std::cout << "p_address: " << p << ",  value: " << *p << "\n";
		}
		for (auto val : values) {
			std::cout << "val: " << val << "\n";
		}
		/*
		 * 使用auto时，默认是copy构造的，尽量使用reference( auto& )，如果需需要常量引用则const auto&;
		 *
		 */
		vector<int> vec{16, 26, 36, 46, 56};
		for (auto& elem : vec) {
			elem += 3;
		}

		cout << "Reference use auto&. \n";
		for (const auto& elem : vec) {
			cout << "elem_address: " << &elem << ", elem_value:  " << elem << "\n";
		}

		for (auto elem_ptr = vec.begin(); elem_ptr != vec.end(); ++elem_ptr) {
			cout << "ptr_address: " << &(*elem_ptr) << ", value: " << *elem_ptr << "\n";
		}

		cout << "Copy use auto.\n";
		for (auto elem : vec) {
			// auto elem ==> vector<int>::value_type; 这里的value_type即Traits起到特性提取出来的。
			cout << "elem_address: " << &elem << ", value: " << elem << "\n";
		}
		// 使用auto, 不使用reference, 实质是copy constructor,创建了新的临时对象, 通过address可以看出。
		// 使用auto it = vec.begin(); 迭代器实质是指针包装出来的， 通过&(*it)可取地址。
	}

	// 这里再看一下隐式类型转换(类型转换)
	class C
	{
	public:
		C() = default;
		explicit C(const string& s) : str_(s) {};
		//C(const string& s): str_(s) {}
		string GetElem() const { return this->str_; }
	private:
		string str_;
	};

	void test_class()
	{
		vector<string> vs{"first", "second", "three"};

		for (const auto& el : vs) {
			cout << "el.size: " << el.size() << "el.type: " << typeid(el).name() << "\n";
			auto c1 = static_cast<C>(el);
			cout << c1.GetElem() << "\n";
		}
	}

	/*
	 * Type Trait
	 * C++ STL以template为根基，为减少template编程的重载数，引用TypeTrait， 它是template在编译期根据TypeTrait产出一个type和value;
	 */
	template <typename T>
	void TraitsFoo(const T& val)
	{
		// is_pointer<T>产出类型true_type或false_type; 而value只有两种结果： true, false; 
		if (std::is_pointer<T>::value) {
			std::cout << "TraitFoo() called for a pointer." << "\n";
		}
		else {
			std::cout << "TraitFoo() called for a pointer. " << "\n";
		}
	}
}

int main(int argc, char* argv[])
{
	keywords_simple::no_explicit_test();
}
