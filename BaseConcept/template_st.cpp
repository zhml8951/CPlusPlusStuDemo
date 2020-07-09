#include <iostream>

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

		constexpr auto operator()(void)->ValueType;
	};

	// 这里重载operator() 参数(void) 返回ValueType值。
	template <class Ty, Ty TVal>
	constexpr auto IntegralConstant<Ty, TVal>::operator()() -> ValueType { return (value); }

	//template 编特化，部分参数特例化。 这里直接定义一个参数。
	template <bool TVal>
	using BoolConstant = IntegralConstant<bool, TVal>;

	template <int64_t TVal>
	using Int64Const = struct IntegralConstant<int64_t, TVal>;

	/*		//这里使用 typedef 就直接不能编译， 只能使用using Char32Const=struct IntegralConstant<char32_t, TVal>
	template <char32_t TVal>
	typedef struct IntegralConstant<char32_t, TVal> Char32Const;
	*/

	template <>
	struct IntegralConstant<double, 2.0>
	{
	}; // 这里的特化没有用处。

	typedef IntegralConstant<double, 2.0> Double2;
	typedef IntegralConstant<float, 3.> Float3;
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
			printf_s("false.\n");
		}

		IntegralConstant<int, 8> integral_8; //
		std::cout << "integral_8.value:  " << integral_8.value << "\nintegral_8(): " << integral_8() << "\n"
			<< "int(integral_8):  " << int(integral_8) << "\n";
		IntegralConstant<int, 1> integral_1; // 直接定义变量， 使用using，typedef则是定义类型。
		std::cout << "int(int_8)+10):  " << int(integral_8) + 10 << "\n";
		// typedef, using 不同写法，作用是一样的。 定义类型， 可通这个类型再定义变量，
		typedef IntegralConstant<int, 1> IntOne;
		using IntTwo = IntegralConstant<int, 2>;
		IntOne one;
	}

	template <class Ty1, class Ty2>
	struct DecayEquiv : std::is_same<typename std::decay<Ty1>::type, Ty2>::type
	{
	};

	// DecayEquiv 继承自 is_same<...>::type 这里的继承体有点复杂，总体继承自is_same<...>::type
	// C++ struct 继承看着让人迷惑， 但在标准库中大量这样使用。
	// struct的列表初始化是构造函数中， struct{ Struct01(in_x, in_y): x_(in_x),y_(in_y)...; 同类中使用是一样的。

	template <class Ty1, class Ty2>
	struct DecayEq
	{
		typedef typename std::is_same<typename std::decay<Ty1>::type, Ty2>::type Type1;
		using Type2 = typename std::is_same<typename std::decay<Ty1>::type, Ty2>::type;
	};
}

int main(int argc, char* argv[])
{
	template_struct::test_integral();
}