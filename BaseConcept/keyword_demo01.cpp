#include <iostream>

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
	class FractionNoExplicit
	{
	public:
		FractionNoExplicit(const int num, const int den = 1) : numerator_(num), denominator_(den)
		{
			printf("Called constructor.\nnumerator: %d, denominator: %d\n", num, den);
		}

		FractionNoExplicit(const FractionNoExplicit & o_explicit )
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
		operator double() const {
			printf("Called operator conversion function.\n");
			return static_cast<double>(numerator_ / denominator_); } ///type conversion function

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
}

int main(int argc, char* argv[])
{
	keywords_simple::no_explicit_test();	
}

