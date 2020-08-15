// lvalue 左值=> 内存中占有确定位置的对象也即地址确定， 名称代表地址。
// rvalue 存储于内存或寄存器中，没有确切地址，故不能赋值

// lvalue 在赋值右侧时会<自动>进行从左值到右值转换，
// * 一元操作符，解引用操作，对指针解引用得到一个左值。
// 一元取地址符'&'， 拿一个左值作为参数可得右值

// ReSharper disable CppUseAuto

#include <iostream>
#include <utility>
#include <functional>
#include <vector>
#include <string>
#include <cstring>

void Stu01()
{
	int arr01[]{1, 2, 3};
	int* p01 = arr01; //int  *p =
	// 一元运算符(*) 解引用, 拿一个右值作为参数可得左值。
	*(p01 + 1) = 30; // * 解引用。
	// 一元取地址符'&'， 拿一个左值作为参数可得右值  ==> 左值引用。
	int num = 30;
	int* num_ptr = &num; // 左值引用. 引用一个左值可得右值。&取值运算；int* 声明指针
	printf("ptr: %p\n", num_ptr);
}

namespace lvalue_rvalue
{
	class IntVec
	{
	public:
		explicit IntVec(const size_t num = 0) : m_size_(num), m_data_(new int[m_size_])
		{
			log("constructor. \n");
		}

		~IntVec()
		{
			log("destructor. \n");
			if (m_data_) {
				delete[] m_data_;
				m_data_ = nullptr;
			}
		}

		/*
		 *    类的几种构造函数：
		 *    默认构造， 拷贝构造， 拷贝赋值
		 *    移动构造， 移动赋值， 析构
		 */

		IntVec(const IntVec& other) : m_size_(other.m_size_), m_data_(new int[m_size_])
		{
			log("copy constructor const&. \n");
			for (size_t i = 0; i < m_size_; ++i) {
				m_data_[i] = other.m_data_[i];
			}
		}

		IntVec(IntVec& other) : m_size_(other.m_size_), m_data_(new int[m_size_])
		{
			log("copy constructor &. \n");
		}

		IntVec(IntVec&& other) noexcept : m_size_(other.m_size_)
		{
			log("copy constructor &&\n");
		}

		IntVec& operator=(const IntVec& other)
		{
			log("copy assignment operator. \n");
			IntVec tmp(other);
			std::swap(this->m_size_, tmp.m_size_);
			std::swap(this->m_data_, tmp.m_data_);
			return *this;
		}

		IntVec& operator=(IntVec&& other) noexcept
		{
			log("Move assignment operator. \n");
			std::swap(this->m_size_, other.m_size_);
			std::swap(this->m_data_, other.m_data_);
			return *this;
		}

		size_t m_size() const
		{
			return this->m_size();
		}

	private:
		void log(const char* msg) const
		{
			std::cout << "[ " << this << " ]" << msg << "\n";
		}

		size_t m_size_;
		int* m_data_;
	};

	void TestIntVec()
	{
		IntVec v1(20);
		IntVec v2;
		std::cout << "Assigning lvalue... \n";
		v2 = v1;
		std::cout << "Ended assigning lvalue... \n";
		std::cout << "Assigning rvalue... \n";
		v2 = IntVec(33);
		std::cout << "Ended assigning rvalue... \n";

		std::cout << "Begin copy constructor... \n";
		IntVec v3 = v1;
		std::cout << "\n.... start std::move(v2)...\n";
		v3 = std::move(v2);
		std::cout << "output v2.\n";
		std::cout << v2.m_size() << "\n";
	}

	class CustomString
	{
	public:
		static size_t counter_;

		explicit CustomString(const char* cstr = nullptr)
		{
			if (cstr) {
				str_data_ = new char[strlen(cstr) + 1];
				strcpy_s(str_data_, strlen(cstr) + 1, cstr);
			}
			else {
				this->str_data_ = new char[1];
				*str_data_ = '\0';
			}
		}

		CustomString(const CustomString& str)
		{
			counter_++;
			const auto length = strlen(str.str_data_) + 1;
			str_data_ = new char[length];
			strcpy_s(str.str_data_, length, str_data_);
		}

		CustomString& operator=(const CustomString& str)
		{
			if (this == &str) return *this;
			delete[] this->str_data_;
			const auto length = strlen(str.str_data_) + 1;
			str_data_ = new char[length];
			strcpy_s(this->str_data_, length, str.str_data_);
			return *this;
		}

		char* get_c_str() const { return str_data_; }

		~CustomString() { delete[] this->str_data_; }

	private:
		char* str_data_;
	};

	size_t CustomString::counter_ = 0;

	void TestCustomString()
	{
		std::vector<CustomString> vec_str(10);
		vec_str.reserve(64); //提前预估容量， 并提交分配，可减少因容量不足而导致的移动，
		const char* str = "CustomString_";
		const int length = strlen(str) + 6;
		char* buff = new char[length];

		//for (int i = 0; i <20; ++i) {
		//	sprintf_s(buff, length, "%s%d", str, i);
		//	CustomString *t = new CustomString(std::move(buff));
		//	printf("t: %s\n", t->get_c_str());
		//	vec_str.push_back(*t);
		//	delete t;
		//}

		for (int i = 0; i < 20; i++) {
			vec_str.push_back(CustomString("custom_string."));
		}

		std::cout << "counter: " << CustomString::counter_ << std::endl;

		for (auto it = vec_str.begin(); it != vec_str.end(); ++it) {
			//std::cout << "vec:  " << typeid(it->get_c_str()).get_name() << "\n";
			printf("it.get_c_str:  %s", it->get_c_str());
		}
	}

	// 理解右值引用的另一例子， 这里是常量左值引用
	class Copyable
	{
	public:
		Copyable() { printf("Copyable(). \n"); }

		Copyable(const Copyable& o) { printf("Copied. \n"); }
	};

	Copyable ReturnValue() { return Copyable(); }

	void AcceptVal(Copyable a) { printf("AcceptVal, val.address: %p\n", &a); }

	void AcceptRef(const Copyable& a) { printf("AcceptRef. ref.address: %p\n", &a); }

	void TestCopyable()
	// 这里为了说明右值引用， 无论传值或传引用，这里return Copyable()返回的都是右值，按理论来说，下面会调用两次拷贝构造，但实际运行则都没有调用，
	// 说明直接传右值， 这是编译器默认都使用了优化技术，自动进行了右值引用。
	// 左值引用, 使用T& 只能绑定左值。
	// 右值引用, 使用T&& 只能绑定右值。
	// 常量左值, 使用const T& 即可绑定左值也可以绑定右值。
	// 已命名的<右值引用>，编译器会认为其是左值。
	{
		printf("pass by value. \n");
		AcceptVal(ReturnValue()); //
		printf("pass by reference. \n");
		AcceptRef(ReturnValue());
	}

	class CustomStrMove
	{
	public:
		static size_t copy_counter_;
		static size_t move_counter_;
		static size_t assign_copy_counter_;
		static size_t assign_move_counter_;

		explicit CustomStrMove(const char* c_str = nullptr)
		{
			if (c_str) {
				const auto length = strlen(c_str) + 1;
				this->ch_data_ = new char[length];
				strcpy_s(ch_data_, length, c_str);
			}
			else {
				ch_data_ = new char[1];
				*ch_data_ = '\0';
			}
		}

		CustomStrMove(const CustomStrMove& str) : ch_data_(nullptr)
		{
			copy_counter_++;
			const auto length = strlen(str.ch_data_) + 1;
			strcpy_s(this->ch_data_, length, str.ch_data_);
		}

		CustomStrMove(CustomStrMove&& str) noexcept : ch_data_(str.ch_data_)
		{
			move_counter_++;
			str.ch_data_ = nullptr;
		}

		CustomStrMove& operator=(const CustomStrMove& str)
		{
			assign_copy_counter_++;
			if (this == &str) return *this;

			delete[] this->ch_data_;
			const auto length = strlen(str.ch_data_) + 1;
			ch_data_ = new char[length];
			strcpy_s(this->ch_data_, length, str.ch_data_);
			return *this;
		}

		CustomStrMove& operator=(CustomStrMove&& str) noexcept
		{
			assign_move_counter_++;
			if (this == &str) return *this;
			delete[] this->ch_data_;
			this->ch_data_ = str.ch_data_;
			str.ch_data_ = nullptr;
			return *this;
		}

		~CustomStrMove()
		{
			delete[] this->ch_data_;
		}

		char* get_ch_str() const { return this->ch_data_; }

	private:
		char* ch_data_;
	};

	size_t CustomStrMove::copy_counter_ = 0;
	size_t CustomStrMove::move_counter_ = 0;
	size_t CustomStrMove::assign_copy_counter_ = 0;
	size_t CustomStrMove::assign_move_counter_ = 0;

	void TestCustomStrMove()
	{
		constexpr size_t total_elem = 2000;
		std::vector<CustomStrMove> vec_str;
		vec_str.reserve(total_elem);

		printf("use auto right reference. \n");
		for (int i = 0; i < total_elem; i++) {
			vec_str.push_back(CustomStrMove("TestCustomStrMove.")); // 直接产生临时右值，调用move也即右值引用。
		}
		printf("copy constructor counter: %lld\n", CustomStrMove::copy_counter_);
		printf("move constructor counter: %lld\n", CustomStrMove::move_counter_);
		printf("copy assign counter: %lld\n", CustomStrMove::assign_copy_counter_);
		printf("move assign counter: %lld	\n", CustomStrMove::assign_move_counter_);

		printf("use copy tmp constructor. \n");
		CustomStrMove::copy_counter_ = 0;
		CustomStrMove::move_counter_ = 0;
		CustomStrMove::assign_copy_counter_ = 0;
		CustomStrMove::assign_move_counter_ = 0;
		vec_str.clear();
		for (auto i = 0; i < total_elem; i++) {
			CustomStrMove tmp{"custom_str_copy"};
			vec_str.push_back(tmp);
		}
		printf("copy constructor counter: %lld\n", CustomStrMove::copy_counter_);
		printf("move constructor counter: %lld\n", CustomStrMove::move_counter_);
		printf("copy assign counter: %lld\n", CustomStrMove::assign_copy_counter_);
		printf("move assign counter: %lld	\n", CustomStrMove::assign_move_counter_);

		printf("use vector emplace_back(). \n");
		CustomStrMove::copy_counter_ = 0;
		CustomStrMove::move_counter_ = 0;
		CustomStrMove::assign_copy_counter_ = 0;
		CustomStrMove::assign_move_counter_ = 0;
		vec_str.clear();
		for (auto i = 0; i < total_elem; ++i) {
			CustomStrMove tmp{"custom_string emplace_back."};
			vec_str.emplace_back(tmp);
		}
		printf("copy constructor counter: %lld\n", CustomStrMove::copy_counter_);
		printf("move constructor counter: %lld\n", CustomStrMove::move_counter_);
		printf("copy assign counter: %lld\n", CustomStrMove::assign_copy_counter_);
		printf("move assign counter: %lld	\n", CustomStrMove::assign_move_counter_);

		printf("use vector std::move(), call right reference. \n");
		CustomStrMove::copy_counter_ = 0;
		CustomStrMove::move_counter_ = 0;
		CustomStrMove::assign_copy_counter_ = 0;
		CustomStrMove::assign_move_counter_ = 0;
		vec_str.clear();
		for (auto i = 0; i < total_elem; i++) {
			CustomStrMove tmp{"custom_string std::move."};
			vec_str.push_back(std::move(tmp));
		}
		printf("copy constructor counter: %lld\n", CustomStrMove::copy_counter_);
		printf("move constructor counter: %lld\n", CustomStrMove::move_counter_);
		printf("copy assign counter: %lld\n", CustomStrMove::assign_copy_counter_);
		printf("move assign counter: %lld	\n", CustomStrMove::assign_move_counter_);
	}

	/*
	 *  修饰类的4个关键字:   const, &, &&, volatile; 这些关键字写函数后面，其本质修饰的就是this;
	 *  如下(PText)： string GetText() const& ==> GetText(const PText&) ==> GetText(const& this);
	 */
	struct PText
	{
		std::string text;

		std::string& GetText() & //  相当于std::string& GetText()  {....}
		{
			return text;
		}

		std::string GetText() &&
		{
			return std::move(text);
		}

		std::string GetText() const & // 这里必须写 const&, 其它样式都无法编译
		{
			return text;
		}
	};

	void TestText()
	{
		auto t1 = PText{"Test t1"};
		auto str1 = std::move(t1).GetText(); // move右值引用会调用 string GetText()&&; 如果没有定义, 则实际调用GetText()&
		std::cout << "str1:  " << str1 << "\n";
		str1 = PText{"Test rvalue t1"}.GetText(); // 这里直接创建右值，调用GetText()&&, 如果没有定义，则调用GetText()&, 右值&&引用减少拷贝；
		std::cout << "str1:  " << str1 << "\n";
		std::cout << "t1.GetValue: " << t1.GetText() << "\n";
	}
}

// CV-unqualified, const, volatile()

int main(int argc, char* argv[])
{
	//lvalue_rvalue::TestIntVec();
	lvalue_rvalue::TestCustomString();
	printf("\n--------------------\n");

	lvalue_rvalue::TestCopyable();
	printf("\n--------------------\n");

	lvalue_rvalue::TestCustomStrMove();
	printf("\n--------------------\n");

	lvalue_rvalue::TestText();
}
