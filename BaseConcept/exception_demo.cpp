#include <iostream>
#include <exception>

namespace exception_simple
{
	/*
	 *	成员函数，普通函数 用 noexcept 修饰，表明函数不能抛出异常，如果出现异常则直接终止程序。
	 *	noexcept是C++11， 同早期的throw(); throw修饰函数的功能已经弃用，noexcept相比throw()主要在性能上提升；
	 *	添加noexcept时编译器可能会进行代码优化。对于move constructor, assignment，如果不是noexcept， 编译器也可能会执行copy;
	 *	函数默认为 noexcept(false); noexcept = noexcept(true);  => throw();
	 *	大部分情况下, noexcept不要使用，最优的使用地方 [move constructor], [move assignment], 析构函数编译器会自动 noexcept;
	 *
	 */
	void FuncNoexcept() noexcept // 对于申明了noexcept 函数，如果出现异常，程序直接调用std::terminate(), 函数内部执行std::abort();
	{
		std::cout << "function with noexcept. " << "\n";
	}

	void CanThrow() { throw 1; }
	void NoBlockThrow() { throw(std::bad_alloc()); }
	void BlockThrow() noexcept { throw(3); }

	void TestThrow()
	{
		try {
			CanThrow();
		}
		catch (int err_no) {
			printf("throw num: %d\n", err_no);
		}

		try {
			NoBlockThrow();
		}
		catch (std::exception& e) {
			std::cout << "exception what():  " << e.what() << "\n";
		}
		catch (...) {
			std::cout << "some exception in NoBlockThrow(). " << "\n";
		}

		try {
			BlockThrow();
		}
		catch (int err_no) {
			printf("throw num: %d\n", err_no);
		}
		catch (...) {
			std::cout << "some error.\n";
		}
	}

	/*
	 * C++ Assert分为动态assert和静态static_assert， C方式的#error; 
	 *	static_assert 在C++0x引入，在编译期间断言，占用的是编译期，不产生目标代码，不会带来运行期性能损失。
	 *	assert 属动态断言，在运行器进行检查，在函数内部使用，若函数没调用，则不会进行检查及断言
	 *		static_assert(bool(condition), "Information"); 当条件不成立时,则停止编译并抛出错误信息。
	 *		在vs2017,有些static_assert直接由ide提示出来, static_assert主要使用于template时，编译时模板实例化时进行判断
	 */

	template <typename T, typename U, typename V>
	class MyTemplateClass
	{
		static_assert(!std::is_empty<T>::value, "T should be non-empty class.");
		static_assert(std::is_empty<U>::value, "U should be an empty class.");
		static_assert(std::is_base_of<std::allocator<T>, V>::value, "V should inherit from std::alloctor<T>");
	};

	void AssertDemo01()
	{
		// msdn demo: 这里如果不是 ==8，直接由IDE提示错误；
		static_assert(sizeof(void*) == 8, "Must 64-bit code generation supported.");

		struct MyStruct
		{
			char value;
		};
		struct MyEmptyStruct
		{
			//double value;
			void func();
		};

		// 确保MyEmptyStruct是空类，在IDE环境，直接就提示出"断言失败"
		static_assert(std::is_empty<MyEmptyStruct>::value, "Empty class needed.");
		// 确保MyStruct非空;
		static_assert(!std::is_empty<MyStruct>::value, "Non-empty struct needed.");
	}
}

int main(int argc, char* argv[])
{
	exception_simple::TestThrow();
}
