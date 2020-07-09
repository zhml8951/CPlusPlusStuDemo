#include <iostream>
#include <string>
#include <cstring>

// C++ ���캯����ظ���
// C++ �������󳣼�3�ַ�ʽ: (���캯��, �������캯��,  ��ֵ����)


// ����һ������ClsA, ������Ĭ�Ͼ������º�����
// Ĭ�Ϲ��캯��				ClsA()=default;
// Ĭ�Ͽ������캯��			ClsA(ClsA& b) = default;
// ��������					~ClsA()=default;
// ��ֵ����					ClsA& operator= (const ClsA&);
// ȡֵ����
// �������캯����һ�����⹹�캯���� ͬһ�������һ��������ͳ�ʼ����һ���� 
class ClsA
{
};

// ǳ����(λ����):���ڻ����������͵ĳ�Ա�������ֽڸ��ƣ����ڴ����ֻ������ָ�룬 �����ͳ�Ա������������Ӧ���͵Ŀ������캯��;
// �������Ա��������ָ�������������ڴ��ˡ� ʹ��ǳ������ֻ�Ǹ���ָ�룬Ҳ����������ָ��ͬһ���ڴ棬 �����ı䶯��Ӱ�쵽��һ���� 
// ����������ʱ������ָ����ͬһ�ڴ�飬����һ�������ͷ����λ��Σ����³����쳣��

// Ϊ���ǳ��������(Ĭ�ϼ�ǳ����)��������ʽ�ṩ�������캯���͸�ֵ���������ֶ�ʵ��  <���>

//�������캯���븳ֵ�����ǳ����׻����ģ� �����������ڶ��󴴽�ʱ���ã� ����ֵ����ֻ�ܱ��Ѿ����ڵĶ�����á� 
void str_copy_assign()
{
	using namespace std;
	string a_cs("String a_cs");
	string b_cs("String b_cs");

	string c_cs = a_cs; // ������õ��ǿ������캯���� ��õ�д�� *** string c_cs(a_cs) *** 
	auto d_cs{a_cs}; // ���ÿ������캯���� string d_cs(a_ca);
	c_cs = b_cs; // ���ø�ֵ����    
}

// �ֶ�ʵ��String�࣬ʵ�����
class CString
{
public:
	const char* c_str() const
	{
		return str_;
	}

	explicit CString(const char* str = ""): str_(new char[strlen(str) + 1])
	{
		strcpy_s(this->str_, strlen(str) + 1, str);
		std::cout << "CString(const char* str='') called.\n";
	}

	// �������캯���� �ڱ������弰��ʼ��ʱʹ�ã� CString cs02{cs01} ==> CString cs02 = cs01;
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

	friend std::ostream& operator<< (std::ostream &os, CString &s)
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
	cstr01 = cstr02; // ������� CString& operator=(const CString& s);
	cout << "cstr01=cstr02, cstr01: " << cstr01.c_str() << '\n';
	auto cstr03{cstr01};
	cout << "cstr03: " << cstr03.c_str() << "\n";
	cout << "cstr03: " << cstr03 <<'\n';
}

int main(int argc, char* argv[])
{
	cstring_test();
}
