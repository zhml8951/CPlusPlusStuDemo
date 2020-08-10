#include <iostream>
#include <string>
#include <cstring>

// C++ 构造函数相关概念
// C++ 创建对象常见前3种方式: (构造函数, 拷贝构造, 赋值构造, 移动构造, 移动赋值)

// 创建一个空类ClsA, 编译器默认就有以下函数：
// 默认构造函数				ClsA()=default;
// 默认拷贝构造函数			ClsA(ClsA& b) = default;
// 析构函数					~ClsA()=default;
// 赋值函数					ClsA& operator= (const ClsA&);
// 取值函数
// 拷贝构造函数是一种特殊构造函数， 同一类对象，用一个对象构造和初始化另一对象。
class ClsA {};

// 浅拷贝(位拷贝):对于基本数据类型的成员变量按字节复制，堆内存变量只复制其指针， 类类型成员变量调用其相应类型的拷贝构造函数;
// 对于类成员变量含有指针变量，分配堆内存了。 使用浅拷贝，只是复制指针，也就两个对象指向同一块内存， 变量的变动会影响到另一对象，
// 而对象析构时，由于指向于同一内存块，导致一个对象被释放两次或多次，导致程序异常。

// 为解决浅拷贝问题(默认即浅拷贝)，必须显式提供拷贝构造函数和赋值函数，即手动实现  <深拷贝>

//拷贝构造函数与赋值函数非常容易混淆的， 拷贝构造是在对象创建时调用， 而赋值函数只能被已经存在的对象调用。
void str_copy_assign()
{
	using namespace std;
	string a_cs("String a_cs");
	string b_cs("String b_cs");

	string c_cs = a_cs; // 这里调用的是拷贝构造函数。 最好的写法 *** string c_cs(a_cs) ***
	auto d_cs{a_cs}; // 调用拷贝构造函数。 string d_cs(a_ca);
	c_cs = b_cs; // 调用赋值函数
}

// 手动实现String类，实现深拷贝
class CString
{
public:
	const char* c_str() const
	{
		return str_;
	}

	explicit CString(const char* str = "") : str_(new char[strlen(str) + 1])
	{
		strcpy_s(this->str_, strlen(str) + 1, str);
		std::cout << "CString(const char* str='') called.\n";
	}

	// 拷贝构造函数， 在变量定义及初始化时使用： CString cs02{cs01} ==> CString cs02 = cs01;
	CString(const CString& s) : str_(nullptr)
	{
		str_ = new char[strlen(s.c_str()) + 1];
		strcpy_s(str_, strlen(s.c_str()) + 1, s.c_str());
		std::cout << "CString(const CString& s) copy construction function called.\n";
	}

	CString& operator=(const CString& s)
	{
		if (this->str_) delete[] this->str_;
		this->str_ = new char[strlen(s.c_str()) + 1];
		strcpy_s(this->str_, strlen(s.c_str()) + 1, s.c_str());
		std::cout << "CString operator= called.\n";
		return *this;
	}

	CString& operator=(const char* s)
	{
		delete[] str_;
		this->str_ = new char(strlen(s) + 1);
		strcpy_s(this->str_, strlen(s) + 1, s);
		std::cout << "operator=(const char* s) Called.\n";
		return *this;
	}

	friend std::ostream& operator<<(std::ostream& os, CString& s)
	{
		os << "Cstring in operator:  " << s.str_ << "\n";
		return os;
	}

	~CString()
	{
		if (str_) delete[] str_;
		std::cout << "~CString called.\n";
	}

private:
	char* str_;
};

void cstring_test()
{
	using namespace std;
	CString cstr01;
	cout << "cstr01:  " << cstr01.c_str() << "\n";

	const CString cstr02{"C String 02"};
	cout << "cstr02: " << cstr02.c_str() << "\n";
	cstr01 = cstr02; // 这里调用 CString& operator=(const CString& s);
	cout << "cstr01=cstr02, cstr01: " << cstr01.c_str() << '\n';
	auto cstr03{cstr01};
	cout << "cstr03: " << cstr03.c_str() << "\n";
	cout << "cstr03: " << cstr03 << '\n';

	// CString point
	CString* cstr_ptr(new CString("new constructor.\n")); // 默认构造函数。
	CString* cstr_ptr02 = cstr_ptr; // 不产生构造，也就是直接是两个指针指向同一对象。
	CString* cstr_ptr03(cstr_ptr);
	cstr_ptr02 = new CString(*cstr_ptr); // 调用copy constructor (拷贝构造)
	*cstr_ptr03 = *cstr_ptr02; // 调用operator= (赋值)
	delete cstr_ptr02;
	delete cstr_ptr;
}

namespace copy_move_demo
{
	class Integer
	{
	public:
		explicit Integer(int value) : ptr_(new int(value))
		{
			std::cout << "Call Integer(int value) " << std::endl;
		}

		~Integer()
		{
			std::cout << "Call ~Integer()" << std::endl;
		}

	/*
	 *	Integer(const Integer& src) 与 Integer(Integer& src) const引用 非const引用是不同变量，故可以重载
	 */
		Integer(const Integer& src): ptr_(new int(*src.ptr_))
		{
			std::cout << "Call Integer(const Integer& src)" << std::endl;
		}

		Integer(Integer& src): ptr_(src.ptr_)
		{
			src.ptr_ = nullptr;
			std::cout << "Call Integer(Integer& src)" << std::endl;
		}

		int GetValue(void) const { return *ptr_; }

	private:
		std::string name_;
		int* ptr_;
	};

	void IntegerMain01()
	{
		Integer a(Integer(100));
		auto a_value = a.GetValue();
		std::cout << "a.value:  " << a_value << "\n";
		printf("---------------------------\n");
		Integer tmp(1000);
		Integer b(tmp);
		auto b_value = b.GetValue();
		std::cout << "b_value: " << b_value << "\n";
	}
}

int main(int argc, char* argv[])
{
	//cstring_test();
	copy_move_demo::IntegerMain01();
}
