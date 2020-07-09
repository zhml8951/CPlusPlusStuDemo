#include <iostream>
#include <string>

// ReSharper disable CppUseAuto

/*
 * 常量引用和指向常量指针是一样， 虽然字面上都是指向常量， 但没有规定其所指对象必须一定是常量。
 * 即使指向非常量，也不能通过常量引用和指向常量指针来修改所指对象的值，是因为它们认为自己指向的是常量。
 * 常量引用和指向常量指针所指向的是非常量对象，可通过非常量本身来进行赋值操作。
 */

/*
 * 顶层const, 底层const. ////
 * 对于一般变量，对象。 只有顶层const. 例如： const string str="lll"; const int i = 0; 不存在const位置问题，
 * 如： const string str="str", string const str="str" 是一样的。  都是顶层const, 没有底层const
 * 声明引用的const都是底层const, 因为引用本身不是对象， 所以不存在顶层const;
 * 指针const分为顶层const和底层const, 顶层const表示指针本身是常量即常量指针。 底层const表示指针指向的是常量即指向常量的指针。
 */

/*
 * 常量引用，指向常量指针多用于函数形参，这样修饰参数主要目的是防止函数修改实参值而设计，也就是底层const指针和常量引用：
 * void func(const string* str1, const string& str2);
 * 这里str1， str2可以传常量也可以传非常量，目标就是防止函数修改实参的值。
 * const...* 底层const: 指向常量指针, ...* const 顶层const：  常量指针
 * 
 */


namespace const_demo
{
	const int kA = 100;

	void func(const int* i, const std::string& str)
	{
	}

	void func2(int* const i1, const int* const i2)
	{
	}

	void Test01()
	{
		const int c = 3;
		int* p = const_cast<int*>(&c);
		// 可能通过强制转换const_cast<int*>(&c), (int*)&c 将常量转换成指向非常量指针，但通过指向非常量指针来修改目标对象的值是不能成功的。
		auto p2 = &c;
		*p = 4; // 这里通过指向非常量指针修改值，实质是将p指向了一相匿名变量tmp，而c值是不变的。
		printf("c: %d", c);
		printf("*p: %d", *p);

		p = const_cast<int*>(&kA); //普通指针指向全局非常，效果同局部常量是一样的。

		printf("a=*p: %d", *p);
		*p = 200;
		printf("a=*p: %d", kA);

		func2(p, &c);	//
		//func2(&c, &c);		//这里是错误的。
	}
}
