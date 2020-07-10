#pragma once
#include <string>

namespace vector_demo
{
	void test_iterator();
	void accumulate_demo();

	namespace accumulate_stu
	{
		inline int acc_add(const int x, const int y) { return x + 2 * y; }

		struct MyStruct
		{
			int operator()(const int x, const int y) const { return x + 3 * y; }
		};

		void test_main();
	} // namespace accumulate_stu
} // namespace vector_demo

namespace stl_china_demo
{
	// 实例来源于http://stlchina.huhoo.net/twiki/bin/view.pl/Main/STLEasyStudy
	// 通过实例对C++ stl 标准模板库进行大致了解，stl是C++的核心也是难点，
	// 需要不断学习和深入

	inline int compare(const void* l_arg, const void* r_arg)
	{
		// ReSharper disable CppCStyleCast
		return (*(int *)l_arg < *(int *)r_arg) ? -1 : (*(int *)l_arg > *(int *)r_arg ? 1 : 0);
	}

	// 采用原始的C语言方式实现compare和sort， 函数有很多问题没有解决
	void compare_no_stl(void);

	//使用STL库实现compare和sort
	void compare_stl(void);

	// 使用STL优化版，使用typedef使程序可读性更强
	void compare_stl_v2(void);

	// 使用istream_iterator, ostream_iterator, sort copy 进行输入，排序， 输出。主要理解STL的iterator操作。
	void io_iterator(void);

	// 
	int iter_copy_file(const std::string&, const std::string&) = delete;
}


namespace template_demo
{
	template <class C>
	bool last_greater_first(const C& container)
	{
		if (container.empty()) return false;
		typename C::const_iterator begin(container.begin());
		typename C::const_iterator end(container.end());
		return *(--end) > *begin;
	}

	template <class T>
	T max_value(const T* list, const int length)
	{
		T max = list[0];

		for (auto i = 1; i < length; i++)
			if (max < list[i]) max = list[i];

		return max;
	}

	class Fraction
	{
		int numerator_; // 分子
		int denominator_; // 分母
	public:

		/**
		 * \brief 分数式
		 * \param n 分子
		 * \param d 分母
		 */
		Fraction(int n, int d); // : numerator_(n), denominator_(d){};

		~Fraction() { std::printf("Fraction object %p delete. perform ~Fraction.\n\n", this); }

		bool operator<(const Fraction& f) const;
		bool operator==(const Fraction& f) const;

		friend std::ostream& operator<<(std::ostream& o, const Fraction& f);
	};

	std::ostream& operator<<(std::ostream& o, const Fraction& f);

	void define_typedef_using(void);

	void test_main(void);
	void test_main02(void);
}

class Complex
{
	int real_;
	const char* imag_;

public:
	explicit Complex(const int r) : real_(r), imag_("ok")
	{
		std::printf("Complex init in 'Complex(const int r)\n\n'");
	}

	explicit Complex(const char* i) : real_(101), imag_(i)
	{
		std::printf("Complex init in 'Complex(const char* i)\n\n'");
	}

	explicit Complex(const int r = 0, const char* i = "close") : real_(r), imag_(i)
	{
		std::printf("Complex init in 'Complex(  )\n\n'");
	};

	int get_real() const;

	const char* get_imag() const;

	Complex operator+(Complex const& obj) const;

	std::ostream& aff(std::ostream& out) const
	{
		out << real_ << " + i " << imag_;
		return out;
	}

	void print() const;
};

Complex operator+(const Complex& a, const Complex& b);

std::ostream& operator<<(std::ostream& out, Complex& c);
