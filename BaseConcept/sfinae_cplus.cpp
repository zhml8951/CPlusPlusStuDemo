/*
 * C++ sfinae 特性相关记录
 */

#include <iostream>
#include <type_traits>
#include <string>

 // ReSharper disable CppNonExplicitConvertingConstructor

 // Substitution Failure Is Not An Error ==> SFINAE, 匹配失败不是错误. 典型的使用std::enable_if<bool, T>
 // 函数重载可根据参数不同直接声明和定义即可,如果是模板函数,则需要使用std::enable_if来实现不同参数调用不同模板，匹配失败的则忽略

namespace sfinae_demo
{
	using std::cout;

	// -------------------------------------begin----------------------------------------------------------

	/*
	 *	一个简单的add函数， 调用时rst=Add(5U, 6U)这里参数不能匹配int Add(int, int)， 然后匹配模板，但unsigned没有value_type,
	 *	也就是这里会出现错误，C++在匹配模板错误时，也就是忽略了这个模板，实质调用则是进行类型转换,将unsigned转换为int,再进行调用add;
	 *	rst1 = Add(5.f, 6.f) 同样也进行类型转换,将float转换int， 再进行调用;
	 */

	int Add(const int a, const int b)
	{
		printf("Called Add. \n");
		return a + b;
	}

	template <typename T>
	typename T::value_type Add(const T& a, const T& b)
	{
		printf("Called template func. \n");
		return a + b;
	}

	void SimpleDemo01()
	{
		const auto rst = Add(5U, 6U); // 自动进行了类型转换,实质调用 int Add(const int a, const int b);
		cout << "rst: " << rst << "\n";

		auto const rst1 = Add(5.f, 6.f); // 自动类型转换,float -> int；调用int Add(...)/
		cout << "rst1: " << rst1 << "\n";
	}

	// ---------------------------------------end--------------------------------------------------------

	/*
	 * C++ 完备的类型体系, 总体包括 (trivial, standard_layout) 同时具有这两个特性的即POD；
	 *	trivial:  支持静态初始化, C-style array, struct, array<N>, 拷贝不变的class, 可以使用memcpy, memmove;
	 *	C++ 相关的类型判断都集中在<type_traits>:
	 *		is_array, is_class, is_function, is_integral, is_empty, is_object, is_pod
	 *		is_trivial, is_union, is_void, is_signed, is_pointer, is_null_pointer, is_reference....
	 *	内置类型(int, float, double..., array<int>)  这些是POD类型， POD类型是递归的，string不是POD类型
	 *	STL库: pair, tuple, vector, list, deque, set, map, unordered_set, unordered_map
	 *		shared_ptr, unique_ptr, 都是is_standard_layout, 但都不是is_trivial, 估不满足is_pod类型条件
	 *		unique_ptr最接近普通指针，故大多数时候都应该使用它，而不是shared_ptr; unique_ptr性能优于shared_ptr; 		
	 */

	void c_plus_type_demo()
	{
		using std::cout;
		std::cout << std::boolalpha;
		cout << "int: \n" << "is_trivial: " << std::is_trivial<int>::value << "\n";
		cout << "is_standard_layout: " << std::is_standard_layout<int>::value << "\n";
		cout << "is_pod: " << std::is_pod<int>::value << "\n";

		cout << "string: " << "\n";
		cout << "is_trivial: " << std::is_trivial<std::string>::value << "\n";
		cout << "is_standard_layout: " << std::is_standard_layout<std::string>::value << "\n";
		cout << "is_pod: " << std::is_pod<std::string>::value << "\n";
		cout << "sizeof(string): " << sizeof(std::string) << "\n";
	}

	/// SfinaeT1如果是普通函数(模板函数)都是不能通过编译的， 但使用 enable_if<T> 则可以使用，因为在推导过程中，只有一个是合法。这样就不会出现二义性
	/// enable_if<bool B, typename T=void>, enable_if 结合type_traits内的类型判断来定义模板，bool B为true时，则进行类型定义，否则不处理

	template <typename T>
	typename std::enable_if<std::is_trivial<T>::value>::type SfinaeT1(T value)
	{
		std::cout << typeid(T).name();
		std::cout << " is Trivial" << ", value: " << value << "\n";
		return value;
	}

	template <typename T>
	typename std::enable_if<!std::is_trivial<T>::value>::type SfinaeT1(T value)
	{
		std::cout << typeid(T).name();
		std::cout << "is not trivial. " << ", value: " << value << "\n";
		return value;
	}

	struct T1
	{
		enum { kIntT, kFloatT } m_type;

		template <typename Integer, std::enable_if_t<std::is_integral<Integer>::value, int> = 0>
		T1(Integer) : m_type(kIntT) {}

		template <typename Floating, typename std::enable_if<std::is_floating_point<Floating>::value, int>::type = 0>
		T1(Floating) : m_type(kFloatT) {}
	};

	void SfinaeTest01()
	{
		SfinaeT1(std::string("string is not trivial value type."));
		SfinaeT1(888); // int is a trivial value type.
	}

	void TypeTraitsTest01()
	{
		struct Foo
		{
			void m() { std::cout << "Non-CV" << "\n"; }
			void m() const { std::cout << "Const func." << "\n"; }
			void m() volatile { std::cout << "Volatile func. " << "\n"; }
			void m() const volatile { std::cout << "Const-Volatile func. " << "\n"; }
		};

		const auto foo_main = []()-> void {
			Foo{}.m();
			std::add_const<Foo>::type{}.m(); //type_traits调用方式,相当于const Foo tmp; tmp.m();
			std::add_volatile<Foo>::type{}.m();
			std::add_cv<Foo>::type{}.m();
		};
		foo_main();
	}
}

int main(int argc, char* argv[])
{
	sfinae_demo::SfinaeTest01();
	sfinae_demo::c_plus_type_demo();
}