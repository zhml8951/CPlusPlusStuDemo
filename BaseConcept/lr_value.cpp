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

void stu01()
{
	int arr01[]{1, 2, 3};
	int* p01 = arr01; //int  *p =
	// 一元运算符(*) 解引用, 拿一个右值作为参数可得左值。
	*(p01 + 1) = 30; // * 解引用。
	// 一元取地址符'&'， 拿一个左值作为参数可得右值  ==> 左值引用。
	int num = 30;
	int* num_p = &num; // 左值引用. 引用一个左值可得右值。
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

		IntVec(const IntVec& other) : m_size_(other.m_size_), m_data_(new int[m_size_])
		{
			log("copy constructor. \n");
			for (size_t i = 0; i < m_size_; ++i) {
				m_data_[i] = other.m_data_[i];
			}
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

	void test_int_vec()
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

	void test_custom_string()
	{
		std::vector<CustomString> vec_str(10);
		vec_str.resize(20);
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

		for(int i =0 ; i< 20; i++) {
			vec_str.push_back(CustomString("custom_string."));
		}

		std::cout << "counter: " << CustomString::counter_ << std::endl;

		for(auto it = vec_str.begin(); it!=vec_str.end(); ++it) {
			//std::cout << "vec:  " << typeid(it->get_c_str()).name() << "\n";
			printf("it.get_c_str:  %s", it->get_c_str());
		}
	}
}

// CV-unqualified, const, volatile()

int main(int argc, char* argv[])
{
	//lvalue_rvalue::test_int_vec();
	lvalue_rvalue::test_custom_string();
}
