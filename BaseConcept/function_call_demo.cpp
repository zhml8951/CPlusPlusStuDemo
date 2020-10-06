#include <iostream>
#include <Windows.h>
#include <memory>
#include <functional>

#ifdef APIENTRY
#undef APIENTRY
#endif
#ifdef WINAPI
#undef WINAPI
#endif

// ReSharper disable CppMemberFunctionMayBeStatic
// ReSharper disable IdentifierTypo

void func_01(const int x, const int y) { std::cout << "x:" << x << ",y:" << y; };
int func_02() { return 0; }

// 在函数定义时，默认样式如上， 其实真实样子如下：
void __cdecl func_03(const int x, const int y)
{
	std::cout << "x:" << x << ",y:" << y;
};

int __cdecl func_04()
{
	return 0;
}

// 真实情况是我们省略了 __cdecl 而编译器会自动按C++ 函数默认调用__cdecl来处理函数。
// 查看windows api 时经常看到， 如下：

#define WINAPI __stdcall
#define APIENTRY WINAPI

// windows下常见这类形式，这是为库适配不同情况而定义的宏。
int APIENTRY winMain(_In_ HINSTANCE hInstance) { return 0; };
// 等同于：
int __stdcall win_main() { return 0; };
void __stdcall func_05() { printf("func_05"); }

#ifdef DECLARE_HANDLE
#undef DECLARE_HANDLE
#endif

#ifdef HINSTANCE__
#undef HINSTANCE__
#endif

#define DECLARE_HANDLE(name) struct get_name##__{int unused;}; typedef struct get_name##__ *get_name

namespace func_simple
{
	/* -------------------------------------------------------------------------------------*/
	// 成员函数指针的一般用法:  T(Cls::*func)(T1...);
	// 在std::thread(&namespace::Func, args,... )， 这里传函数指针，

	class ClsFuncPoint
	{
	public:
		int Func01(const int n)
		{
			std::cout << "Foo_n: " << n << "\n";
			return n;
		}

		void Func02()
		{
			std::cout << "Called Func02. \n";
		}

		static int Func03(const int a, const int b)
		{
			std::cout << "Called Func03. " << a + b << '\n';
			return a + b;
		}

		void Func04()
		{
			std::cout << "Called Func04. " << "\n";
		}

		// 类成员函数， 传普通函数指针，只能调用静态成员函数，实质是不存在this指针。
		int PFunc01(int(*p)(int, int), const int a, const int b)
		{
			return (*p)(a, b);
		}

		// ReSharper disable once CppMemberFunctionMayBeConst
		void PFunc02(void (ClsFuncPoint::*member_func)()) // ReSharper 提示const, 但添加const直接不能编译
		{
			// 类中定义函数指针,添加了类空间::, 内部使用this,相当于object指针，可调用成员函数
			(this->*member_func)();
		}
	};

	void Test01()
	{
		int (ClsFuncPoint::*p_func)(int) = &ClsFuncPoint::Func01; // T1 (CLASS_::FUNC)(T2,...) 可调用非静态成员函数
		ClsFuncPoint obj_a;
		(obj_a.*p_func)(5); // 这里一种很奇怪的调用方式,

		const std::shared_ptr<ClsFuncPoint> obj_b(new ClsFuncPoint);
		(obj_b.get()->*p_func)(6); // 当使用智能指针时, 需要使用get()将智能指针解出来才可指向到外部定义的函数指针

		const auto obj_c = new ClsFuncPoint;
		(obj_c->*p_func)(7);
		delete obj_c;
	}

	void Test02()
	{
		auto obj01 = std::make_unique<ClsFuncPoint>();
		auto obj02 = new ClsFuncPoint;

		//使用静态成员函数同使用标准函数一直，直接定义定义： T1(*ptr_func)(T1, ...) = &Class::Func;
		// 使用auto可自动推导出p_func = int(*p_func)(int, int);这是C++11后简化代码的方法
		const auto p_func = &ClsFuncPoint::Func03;
		p_func(88, 99);

		void (ClsFuncPoint::*func02_ptr)() = &ClsFuncPoint::Func02;
		const auto func02_ptr_auto = &ClsFuncPoint::Func02; // 使用auto可自动推导出void(ClsFuncPoint::*func2)()=...
		(obj01.get()->*func02_ptr)();

		(obj02->*func02_ptr_auto)();

		obj01->PFunc01(&ClsFuncPoint::Func03, 8, 9);

		obj02->PFunc02(&ClsFuncPoint::Func04);
		obj01->PFunc02(&ClsFuncPoint::Func02);

		delete obj02;
	}

	/* -------------------------------------------------------------------------------------*/

	/*
	 * 获取成员函数指针
	 *	例子使用了两次类型转换: &src转void*, void*再转DsyDy*, *取值
	 *		*static_cast<DstTy*>(static_cast<void*>(&src)) ==> DstTy;
	 *
	 */
	class CApple
	{
	public:
		CApple() = default;
		~CApple() = default;

		void Func01() { std::cout << "Called Func01. " << "\n"; }

		void Func02(const int a, const int b) { std::cout << "Called Func02. num: " << a << ", " << b << "\n"; }

		int Func03(const int a)
		{
			std::cout << "Called Func03. \n" << "\n";
			return a;
		}
	};

	// 模板函数的妙用, 在调用时SrcTy到底是什么类型呢？ //TODO 需要更进一步理解模板和函数指针方面的内容
	template <typename DstTy, typename SrcTy>
	DstTy PointerCast(SrcTy src)
	{
		return *static_cast<DstTy*>(static_cast<void*>(&src));
	}

	// 这里充分利用union的特点， // TODO 目前还理解不了
	template <class DstTy, class SrcTy>
	DstTy UnionCast(SrcTy src)
	{
		union
		{
			SrcTy s;
			DstTy d;
		} u;
		u.s = src;
		return u.d;
	}

	typedef void(__fastcall *FuncArg1)(void* p_this, int edx);
	typedef void(__fastcall *FuncArg2)(void* p_this, int edx, int a);
	typedef void(__fastcall *FuncArg3)(void* p_this, int edx, int a, int b);

	// ReSharper disable CppUseAuto

	void func_pointer_test01()
	{
		CApple app01;
		const auto app02 = std::make_unique<CApple>();
		const auto app03 = std::make_shared<CApple>();

		const FuncArg1 func01 = PointerCast<FuncArg1>(&CApple::Func01);
		const auto func02 = PointerCast<FuncArg2>(&CApple::Func02);
		const FuncArg3 func03 = UnionCast<FuncArg3>(&CApple::Func03);

		// 输出string, func的指针，使用printf("%p",&str),即可，若使用std::cout <<则必须进行强制转换为const void*
		//样式:  static_cast<const void*>(&func), static_cast<const void*>(&str);
		std::cout << "func01_id: " << static_cast<const void*>(&func01) << "\n";
		func01(app02.get(), 88);

		std::cout << "func02_id: " << static_cast<const void*>(&func02) << "\n";
		func02(&app01, 8, 8);

		printf("func03_address: %p\n", &func03);
		std::cout << "func03_address: " << static_cast<const void*>(&func03) << "\n";
		func03(app03.get(), 88, 8, 9);

		const char* str = "Test string use const char*, address. ";
		printf("str.address: %p\n", &str);
		std::cout << "str_address: " << static_cast<void*>(&str) << "\n";
		std::string str2 = "Test string use std::string,  address. ";
		std::cout << "str2_address: " << &str2 << "\n";
		std::cout << "str2_address: " << static_cast<void*>(&str2) << "\n";
	}

	void test_func_ref()
	{
		// 概念: C++ 函数名实质指函数指针, 调用函数中, 编译器在内部转换成函数指针, 同样,我们在调用函数时,也可以直接使用函数指针样式,
		Test02(); // 直接调用.
		(&Test02)(); /// &取地址， 函数取地址，直接调用，同直接调用函数名是一样的。 这里可以明白函数指针的了，
		(*Test01)(); /// *解引用, 同样说明函数名即指针
	}
}

int main(int argc, char* argv[])
{
	//func_simple::Test01();
	func_simple::func_pointer_test01();
	printf("\n\n");
	func_simple::test_func_ref();
}