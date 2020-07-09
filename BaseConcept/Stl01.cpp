#include "Stl01.h"
#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
#include <exception>
#include <iterator>
#include <functional>
#include <fstream>


void vector_demo::test_iterator()
{
	std::vector<int> vec{1, 2, 3, 4, 5};
	auto iter = vec.begin();
	for (; iter != vec.end(); ++iter) {
		std::cout << *iter << "  ";
	}
	std::cout << std::endl;

	std::cout << typeid(iter).name() << std::endl;
	std::cout << std::accumulate(vec.begin(), vec.end(), 0);
}

void vector_demo::accumulate_demo()
{
	using namespace std;
	vector<int> vec{1, 2, 3, 4, 5};
	auto vec_sum = accumulate(vec.begin(), vec.end(), 0);
	cout << "Sum: " << vec_sum << endl;
	vec_sum = accumulate(vec.begin(), vec.end(), 100);
	cout << "Sum+100:  " << vec_sum << endl;
}

void vector_demo::accumulate_stu::test_main()
{
	const MyStruct struct_01;
	const auto init = 100;
	int numbers[]{10, 20, 30};
	const auto array_length = std::end(numbers) - std::begin(numbers);

	std::cout << "array length: " << array_length << '\n';
	std::cout << "Using Default Accumulate(init=100): ";
	std::cout << std::accumulate(numbers, numbers + array_length, init);
	std::cout << '\n' << std::endl;

	std::cout << "Using functional minus: ";
	std::cout << std::accumulate(numbers, numbers + array_length, init,
	                             std::minus<int>());
	std::cout << '\n' << std::endl;

	std::cout << "Using user custom function: ";
	std::cout << std::accumulate(numbers, numbers + array_length, init, acc_add);
	printf("\n\n");

	std::cout << "Using custom object: ";
	std::cout << std::accumulate(numbers, numbers + array_length, init, struct_01);
	printf("\n\n");
}

void stl_china_demo::compare_no_stl()
{
	const auto max_size = 10;
	int num[max_size];
	auto n = 0;
	while (true) {
		try {
			std::cin >> num[n];
			n++;
		}
		catch (std::exception& e) {
			std::cout << e.what() << '\n';
			break;
		}
	}
	// for (n = 0; std::cin >> num[n]; n++);

	qsort(num, n, sizeof(int), compare);

	for (auto i = 0; i < n; i++) {
		std::cout << num[i] << '\n';
	}
}


void stl_china_demo::io_iterator()
{
	using namespace std;
	string text;
	const istream_iterator<int> input_int(cin);
	const istream_iterator<int> eof;

	copy(input_int, eof, back_inserter(text));
	sort(text.begin(), text.end());
	const ostream_iterator<int> output_int(cout, ",  ");
	copy(text.begin(), text.end(), output_int);
}

//int stl_china_demo::iter_copy_file(const std::string& in_file_name, const std::string& out_file_name)
//{
//	using namespace std;
//	ifstream in_file(in_file_name, ios::in | ios::binary);
//	ofstream out_file(out_file_name);
//
//	if(!in_file) {
//		cout << "Read file " << in_file_name << " Failed!! \n";
//		return -1;
//	}
//
//	istream_iterator<string> in_iter(in_file);
//	istream_iterator<string> eof;
//	vector<string> text;
//
//	copy(in_iter, eof, back_inserter(text));
//	sort(text.begin(), text.end());
//	copy(text.begin(), text.end(), out_file_name);
//	return 0;
//}

void stl_china_demo::compare_stl()
{
	std::vector<int> num;
	int element;
	while (std::cin >> element)
		num.push_back(element);

	std::sort(num.begin(), num.end());

	for (auto n : num) {
		std::cout << n << "   ";
	}
}

void stl_china_demo::compare_stl_v2()
{
	// 采用STL库可创建出非常优美的函数，前提是必须能够熟练使用stl。 使用stl可使程序抽象程度更高。
	typedef std::vector<int> IntVector;
	typedef std::istream_iterator<int> IstreamIterator;
	typedef std::ostream_iterator<int> OstreamIterator;
	typedef std::back_insert_iterator<IntVector> BackInsertIterator;

	IntVector num;
	std::copy(IstreamIterator(std::cin), IstreamIterator(), BackInsertIterator(num));

	std::sort(num.begin(), num.end());

	std::copy(num.begin(), num.end(), OstreamIterator(std::cout, "  "));
}


template_demo::Fraction::Fraction(const int n, const int d) : numerator_(n), denominator_(d)
{
	std::printf("Fraction init ok.\n");
}

bool template_demo::Fraction::operator<(const Fraction& f) const
{
	if (this->denominator_ * f.denominator_ > 0)
		return this->numerator_ * f.denominator_ < this->denominator_ * f.numerator_;

	return this->numerator_ * f.denominator_ > this->denominator_ * f.numerator_;
}

bool template_demo::Fraction::operator==(const Fraction& f) const
{
	return numerator_ * f.denominator_ == denominator_ * f.numerator_;
}


std::ostream& template_demo::operator<<(std::ostream& o, const Fraction& f)
{
	o << f.numerator_ << "/" << f.denominator_;
	return o;
}

void template_demo::define_typedef_using()
{
	// C++ 定义别名的方法常用有3种， #define, typedef, using;
	// #define C语言宏， 在预编译阶段就进行字符串替换，不参入编译过程。目前不建议使用。
	// typedef C++98 采用的方式， 使用比较多，
	// using C++11 最新方式， 方式比较直观。 
#ifndef VEC_INT
#define VEC_INT std::vector<int>
#endif
	typedef std::vector<double> VecDouble;
	using VecStr = std::vector<std::string>;

	VEC_INT vec01{10, 10};
	auto iter = vec01.begin();
	auto i = 0;
	std::for_each(vec01.begin(), vec01.end(), [&](int it)-> void
	{
		std::printf("vec[%d] num in lambda func:%d", i, *iter);
		++iter;
		++i;
	});
	std::printf("#define demo end.\n\n");
}

void template_demo::test_main()
{
	int list[]{89, 99, 21, 11, 999, 65};
	const int length = sizeof(list) / sizeof(list[0]);
	const auto m = max_value<int>(list, length);
	std::printf("max:  %d", m);

	using VecInt = std::vector<int>;
	const VecInt v1{33, 24, 99, 51, 78};
	const auto big_now = last_greater_first<VecInt>(v1);
	if (big_now) std::printf("first smaller than last:  True\n");
	else std::printf("last smaller than first! \n");
	const Fraction fr1(33, 55);
	const auto fr2 = new Fraction(34, 88);
	Fraction fr3{34, 44};
	auto big = fr1 < fr3;
	std::printf("fr1 < fr3: %d", big);
	delete fr2;
}

void template_demo::test_main02()
{
	define_typedef_using();
}


int Complex::get_real() const
{
	return this->real_;
}

const char* Complex::get_imag() const
{
	return this->imag_;
}

Complex Complex::operator+(Complex const& obj) const
{
	Complex res;
	res.real_ = this->real_ + obj.real_;
	res.imag_ = (std::string(this->imag_) + std::string(obj.imag_)).data();
	return res;
}

void Complex::print() const
{
	std::cout << this->real_ << " + i " << this->imag_ << std::endl;
}

Complex operator+(const Complex& a, const Complex& b)
{
	using std::string;
	const auto r = a.get_real() + b.get_real();
	const auto i = (string(a.get_imag()) + string(b.get_imag())).data();
	const Complex res(r, i);
	return res;
}

std::ostream& operator<<(std::ostream& out, Complex& c)
{
	return c.aff(out);
}
