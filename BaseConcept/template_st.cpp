#include <iostream>

namespace template_struct
{
	// ��Դ�ڱ�׼��type_traits��integral_constant�� �ص�����struct���operator ValueType() const { �����÷���
	template <class Ty, Ty TVal>
	struct IntegralConstant
	{
		static constexpr Ty value = TVal;
		using ValueType = Ty;
		using Type = IntegralConstant;
		// ���� operator<ValueType> (����void). ���ֶ����͵������÷����⣬�������á�
		explicit constexpr operator ValueType(void) const { return (value); }

		constexpr auto operator()(void)->ValueType;
	};

	// ��������operator() ����(void) ����ValueTypeֵ��
	template <class Ty, Ty TVal>
	constexpr auto IntegralConstant<Ty, TVal>::operator()() -> ValueType { return (value); }

	//template ���ػ������ֲ����������� ����ֱ�Ӷ���һ��������
	template <bool TVal>
	using BoolConstant = IntegralConstant<bool, TVal>;

	template <int64_t TVal>
	using Int64Const = struct IntegralConstant<int64_t, TVal>;

	/*		//����ʹ�� typedef ��ֱ�Ӳ��ܱ��룬 ֻ��ʹ��using Char32Const=struct IntegralConstant<char32_t, TVal>
	template <char32_t TVal>
	typedef struct IntegralConstant<char32_t, TVal> Char32Const;
	*/

	template <>
	struct IntegralConstant<double, 2.0>
	{
	}; // ������ػ�û���ô���

	typedef IntegralConstant<double, 2.0> Double2;
	typedef IntegralConstant<float, 3.> Float3;
	//std::ostream operator<<(const std::ostream& lhs, const IntegralConstant<int, 1>& rhs);

	void test_integral(void)
	{
		using namespace std;
		typedef BoolConstant<true> TypeTrue;
		using TypeFalse = BoolConstant<false>;
		TypeTrue true01; // //����ֱ��ʹ��Type�������,��TypeTrue()�ǲ���ֱ��ʹ�õġ�
		TypeFalse false01;
		cout << boolalpha << "TypeTrue::value:  " << TypeTrue::value /* << ", TypeTrue():  " << TypeTrue() */
			<< ".  true01(): " << true01() << ", bool(true01): " << bool(true01)/*�������bool operator()*/ << "\n";
		//if (TypeTrue())
		if (true01) {
			/* if (true01) ����operator bool() const{}.  �൱��bool(true01) */
			// (true01 && false01) ==> false, (true01 || false01) ==> true, (false01) ==> false,
			cout << "true01(): " << true01() << ",  bool(true01): " << bool(true01) << "\n";
		}
		else {
			printf_s("false.\n");
		}

		IntegralConstant<int, 8> integral_8; //
		std::cout << "integral_8.value:  " << integral_8.value << "\nintegral_8(): " << integral_8() << "\n"
			<< "int(integral_8):  " << int(integral_8) << "\n";
		IntegralConstant<int, 1> integral_1; // ֱ�Ӷ�������� ʹ��using��typedef���Ƕ������͡�
		std::cout << "int(int_8)+10):  " << int(integral_8) + 10 << "\n";
		// typedef, using ��ͬд����������һ���ġ� �������ͣ� ��ͨ��������ٶ��������
		typedef IntegralConstant<int, 1> IntOne;
		using IntTwo = IntegralConstant<int, 2>;
		IntOne one;
	}

	template <class Ty1, class Ty2>
	struct DecayEquiv : std::is_same<typename std::decay<Ty1>::type, Ty2>::type
	{
	};

	// DecayEquiv �̳��� is_same<...>::type ����ļ̳����е㸴�ӣ�����̳���is_same<...>::type
	// C++ struct �̳п��������Ի� ���ڱ�׼���д�������ʹ�á�
	// struct���б��ʼ���ǹ��캯���У� struct{ Struct01(in_x, in_y): x_(in_x),y_(in_y)...; ͬ����ʹ����һ���ġ�

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