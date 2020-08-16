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
}
