#include <iostream>
#include <cmath>
#include <functional>

namespace template_simple
{
	// 模板基础应用(C++ 泛化)
	template <typename Tx, typename Ty> // 模板函数
	void Func(Tx x, Ty y)
	{
		std::cout << "num1: " << x << ", num2: " << y << "\n";
	}

	template <typename Tx, typename Ty> // 模板类
	class CompCls
	{
		static bool Comp(Tx x, Ty y) { return (x < y) ? true : false; }
	};

	// 全特化: 主模板类， 模板类型全部明确化
	template <>
	void Func(int x, double y)
	{
		std::cout << "num1: " << x << ", num2: " << y << "\n";
		y = round(y);
		auto y1 = static_cast<int>(y);

		std::swap<int>(x, y1);
		std::cout << "num1: " << x << ", num2: " << y1 << "\n";
	}

	template <>
	class CompCls<int, double>
	{
	public:
		static bool Comp(const int x, const double y) { return (x < y) ? true : false; }
	};

	//模板函数不支持偏特化, 这里是模板函数重载，只有1个模板参数
	template <typename Ty>
	void Func(int x, Ty y)
	{
		std::cout << "x size: " << sizeof(x) << ", y type: " << typeid(y).name() << "\n";
	}

	template <typename Tx>
	class CompCls<Tx, double>
	{
	public:
		static bool Comp(Tx x, double y) { return (x < y) ? true : false; }
	};

	/*---------------------------------------------------------------------------------------------------------*/
	/// 模板类型区分{
	template <typename T>
	class CompoundT /// 基本模板类型
	{
	public:
		enum { kIsPtrT = 0, kIsRefT = 0, kIsArrayT = 0, kIsFuncT = 0, kIsPtrMemT = 0 };

		typedef T BaseT;
		typedef T BottomT;
		typedef CompoundT<void> ClassT;
	};

	template <typename T>
	class CompoundT<T&> ///引用特化
	{
	public:
		enum { kIsPtrT = 0, kIsRefT = 1, kIsArrayT = 0, kIsFuncT = 0, kIsPtrMemT = 0 };

		typedef T BaseT;
		typedef typename CompoundT<T>::BottomT BottomT;
		typedef CompoundT<void> ClassT;
	};

	template <typename T>
	class CompoundT<T*> /// 指针特化(局部特化，也是特化的一种)
	{
	public:
		enum { kIsPtrT = 1, kIsRefT = 0, kIsArrayT = 0, kIsFuncT = 0, kIsPtrMemT = 0 };

		typedef T BaseT;
		typedef typename CompoundT<T>::BottomT BottomT;
		typedef CompoundT<void> ClassT;
	};

	template <typename T, size_t N>
	class CompoundT<T[N]>			// => class CompoundT<T[]>{....}
	{
	public:
		enum { kIsPtrT = 0, kIsRefT = 0, kIsArrayT = 1, kIsFuncT = 0, kIsPtrMemT = 0 };

		typedef T BaseT;
		typedef typename CompoundT<T>::BottomT BottomT;
		typedef CompoundT<void> ClassT;
	};

	//template<typename T>
	//class CompoundT<T C::*>		/// C::* ==> C的成员变量和成员函数

	/*---------------------------------------------------------------------------------------------------------*/

	// typename... 参数包，
	template <typename T, typename... Args>
	void Func02(const T& t, const Args&... res)
	{
		//
	}
}

namespace template_struct
{
	// 来源于标准库type_traits的integral_constant， 重点在于struct里的operator ValueType() const { 理解和用法。
	template <class Ty, Ty TVal>
	struct IntegralConstant
	{
		static constexpr Ty value = TVal;
		using ValueType = Ty;
		using Type = IntegralConstant;

		// 重载 operator<ValueType> (参数void). 这种对类型的重载用法特殊，但很有用。
		explicit constexpr operator ValueType(void) const { return (value); }

		constexpr auto operator()(void) -> ValueType;
	};

	// 这里重载operator() 参数(void) 返回ValueType值。
	template <class Ty, Ty TVal>
	constexpr auto IntegralConstant<Ty, TVal>::operator()() -> ValueType { return (value); }

	//template 偏特化，部分参数特例化。 这里直接定义一个参数。
	template <bool TVal>
	using BoolConstant = IntegralConstant<bool, TVal>;

	template <int64_t TVal>
	using Int64Const = struct IntegralConstant<int64_t, TVal>;

	/* //这里使用 typedef 就直接不能编译， 只能使用using Char32Const=struct IntegralConstant<char32_t, TVal>
	template <char32_t TVal>
	typedef struct IntegralConstant<char32_t, TVal> Char32Const;
	*/
	template <char32_t TVal>
	using Char32Const = struct IntegralConstant<char32_t, TVal>;

	/*  无效 全特化(template<typename T, T TVal>)
	template <>
	struct IntegralConstant<double, 88.0> { }; //这种第2参数类型依赖第1参数的，不可做全特化；
	*/

	//typedef IntegralConstant<double, 2.0> Double2;
	//typedef IntegralConstant<float, 3.> Float3;
	//std::ostream operator<<(const std::ostream& lhs, const IntegralConstant<int, 1>& rhs);

	void test_integral(void)
	{
		using namespace std;
		typedef BoolConstant<true> TypeTrue;
		using TypeFalse = BoolConstant<false>;
		TypeTrue true01; // //不能直接使用Type进行输出,即TypeTrue()是不能直接使用的。
		TypeFalse false01;
		cout << boolalpha << "TypeTrue::value:  " << TypeTrue::value /* << ", TypeTrue():  " << TypeTrue() */
			<< ".  true01(): " << true01() << ", bool(true01): " << bool(true01)/*这里调用bool operator()*/ << "\n";
		//if (TypeTrue())
		if (true01) {
			/* if (true01) 调用operator bool() const{}.  相当于bool(true01) */
			// (true01 && false01) ==> false, (true01 || false01) ==> true, (false01) ==> false,
			cout << "true01(): " << true01() << ",  bool(true01): " << bool(true01) << "\n";
		}
		else {
			cout << "false01:  " << false01() << ", bool(false): " << bool(false01) << "\n";
		}

		IntegralConstant<int, 8> integral_8; //
		std::cout << "integral_8.value:  " << integral_8.value << "\nintegral_8(): " << integral_8() << "\n"
			<< "int(integral_8):  " << int(integral_8) << "\n";

		IntegralConstant<int, 1> integral_1; // 直接定义变量， 使用using，typedef则是定义类型。
		std::cout << "integral_1:  " << int(integral_1) << ",  integral(): " << integral_1() << "\n";
		std::cout << "int(int_8)+10):  " << int(integral_8) + 10 << "\n";

		// typedef, using 不同写法，作用是一样的。 定义类型， 可通这个类型再定义变量，
		typedef IntegralConstant<int, 1> IntOne;
		using IntTwo = IntegralConstant<int, 2>;
		IntOne one;
		std::cout << "IntegralConstant<int,1>:  " << one() << "\n";
	}

	template <class Ty1, class Ty2>
	struct DecayEquiv : std::is_same<typename std::decay<Ty1>::type, Ty2>::type { };

	// DecayEquiv 继承自 is_same<...>::type 这里的继承体有点复杂，总体继承自is_same<...>::type
	// C++ struct 继承看着让人迷惑， 但在标准库中大量这样使用。
	// struct的列表初始化是构造函数中， struct{ Struct01(in_x, in_y): x_(in_x),y_(in_y)...; 同类中使用是一样的。

	template <class Ty1, class Ty2>
	struct DecayEq
	{
		typedef typename std::is_same<typename std::decay<Ty1>::type, Ty2>::type Type1;
		using Type2 = typename std::is_same<typename std::decay<Ty1>::type, Ty2>::type;
		Type1 element1;
		Type2 element2;
	};

	/*
	 * 定义 func 多重载版本， int& x 左值引用， int&& x 右值引用；对于此类调用，需要根据参数类型手动调用相应函数；
	 */
	void Func(int& x) { std::cout << "lvalue ref: " << x << "\n"; }
	void Func(int&& x) { std::cout << "rvalue ref: " << x << "\n"; }
	void Func(const int& x) { std::cout << "const lvalue ref: " << x << "\n"; }
	void Func(const int&& x) { std::cout << "const rvalue ref: " << x << "\n"; }

	// std::forward<T> 完美转发；std::forward<T>根据T相应实参类型调用相应的Func函数；
	template <typename T>
	void PerfectForward(T&& t) { Func(std::forward<T>(t)); }

	void test_perfect_forward()
	{
		PerfectForward(10); // rvalue ref; 10为右值
		auto a = 88;
		PerfectForward(a); // lvalue;  a为左值
		PerfectForward(std::move(a)); // std::move(a) 左值move为右值引用

		const auto b = 8;
		PerfectForward(b); // const lvalue,
		PerfectForward(std::move(b)); // const rvalue;
	}

	/*
	 *	typename...T:  参数包(模板参数列表, 函数参数列表)
	 *	模板参数包在模板推导过程中被认定为一个特殊的类型参数(函数参数包被认为是一个特殊类型参数)
	 *	参数包不可以直接使用，需要展开， 展开方式两种： 1. 递归操作， 2. 逗号操作
	 */
	template <typename T, typename ... Ts>
	void PrintSize(T arg1, Ts ... args)
	{
		std::cout << "arg1 size: " << sizeof(arg1) << "\n"; // 显示元素字节
		std::cout << "args size: " << sizeof...(Ts) << "\n"; // sizeof...() 显示元素个数。
	}

	void test_print_size()
	{
		PrintSize(88.8, 36, 24, 55, 99);
	}

	// 模板函数参数包递归展开， Sum(Ty num) 递归终止函数
	template <typename Ty>
	Ty Sum(Ty num)
	{
		printf("Called Sum<Ty>(), num: %d\n", num);
		return num;
	}

	// 递归展开，采用递归调用重载单参函数; 递归调用必需有一个重载的终止函数
	// Sum<Ty>(rest...) 递归调用，自动展开rest...， 这里的调用实质上是不断调用自身，直到最后一个元素调用递归终止函数

	template <typename Ty, typename... Tys>
	Ty Sum(Ty first, Tys ... rest)
	{
		printf("Called Sum<Ty, Tys>, num: %d\n", first);
		return first + Sum<Ty>(rest...);
	}

	// 可变参数模板函数, 采用逗号展开
	template <class Ty>
	void PrintArg(Ty t) { std::cout << t << "\n"; }

	template <class... Tys>
	void Expand(Tys ... rest)
	{
		int arr[] = {(PrintArg(rest), 1)...};
		for (auto i = 0; i < sizeof(arr) / sizeof(arr[0]); i++) {
			printf("i: %d  ", arr[i]);
		}
	}

	template <class Func, class... Tys>
	void ExpandUseList(const Func& func, Tys&&... args)
	{
		// C++ "一致性初始化", 旧样式int a=3; 列表样式: int a{3}; vector<int> vec; 引入列表初始化后可直接vector<int> vec{1,2,3};
		// {列表初始化} 引入实现由 initializer_list 体现.
		//列表初始化时使用auto 可见db_list即std::initializer_list<double> 类型， 这里 = 号必可少的
		auto db_list_auto = {1.1, 2., 3.0, 4., 5.1};
		std::initializer_list<double> db_list_init{3.2, 4, 5, 5};

		// initializer_list 奇特用法,需要结合前面{(PrintArg(... 来更解。这里传回调函数会很有用处。
		std::initializer_list<int>{(func(std::forward<Tys>(args)), 0)...};
	}

	void test_expand()
	{
		auto rst = Sum(11, 12, 13, 14); // 递归调用的逻辑需要了解
		printf("sum result:  %d\n", rst);

		rst = Sum(88);
		printf("sum result:  %d\n", rst);
		Expand(1, 2, 3, 4);
		printf("\n");

		/// 传入lambda 函数；
		ExpandUseList([](const int i) { std::cout << i << "  " << "\n"; }, 1, 2, 3, 4, 5);
	}

	/*
	 * 可变参数模板类的展开方式:  模板类偏特化方式展开, 模板类继承方式展开
	 * 详细使用见 https://www.cnblogs.com/qicosmos/p/4325949.html
	 *
	 * 可变参数模板类实例:  C++ 委托函数
	 */

	template <typename Ty, typename R, typename... Args>
	class DelegateFun
	{
		/*
		 *   程序包含几个过去没注意的点:
		 *		R(Ty::*pf)(Args...): Ty::类空间, *pf函数指针，即:成员函数指针
		 *	(cls_ptr_->*func_ptr_)(std::forward<Args>(args)...)
		 *		函数指针调用时方式(*func_p)(args...), 这里添加class限定，格式只能是(class_ptr->*func_ptr)(args)
		 *		或者  (*class_ptr.*func_ptr)(args);   原理相通
		 *
		 *	(std::forward<Args>(args)...)即参数包 解开时的标准样式, 同类样式如下：
		 *		initializer_list<int>{func(forward<Tys>(args), 0)...}
		 *		arr[] = {(print(rest),1)...}
		 */
	public:
		DelegateFun(Ty* cls, R (Ty::*pf)(Args ...)) : cls_ptr_(cls), func_ptr_(pf) {}

		R operator()(Args&&... args)
		{
			//(*cls_ptr_.*func_ptr_)(std::forward<Args>(args)...);
			return (cls_ptr_->*func_ptr_)(std::forward<Args>(args)...);
		}

	private:
		Ty* cls_ptr_{nullptr};
		R (Ty::*func_ptr_)(Args ...){nullptr};
	};

	template <typename T, typename R, typename... Args>
	DelegateFun<T, R, Args...> CreateDelegate(T* t, R (T::*f)(Args ...))
	{
		return DelegateFun<T, R, Args...>(t, f);
	}

	void test_delegate()
	{
		struct T1
		{
			void Func0(const int i) { printf("num:  %d\n", i); }
			void Func1(const int i, const double j) { printf("num1+num2: %d\n", int(i + j)); }
		};
		T1 test0;
		auto test_func0 = CreateDelegate(&test0, &T1::Func0);
		auto test_func1 = CreateDelegate(&test0, &T1::Func1);
		test_func0(888);
		test_func1(88, 99);
	}
}

int main(int argc, char* argv[])
{
	namespace ts = template_struct;
	//template_struct::test_integral();
	ts::test_print_size();
	ts::test_expand();
	ts::test_delegate();
}
