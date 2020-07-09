#include <vector>
#include <iostream>

namespace vec_demo
{
	class Stu
	{
	public:
		explicit Stu(char* name = nullptr, const int age = 0): name_(name), age_(age)
		{
			printf("%s Stu constructor called. \n", name);
		}

		char* get_name() const
		{
			return this->name_;
		}

		int get_age() const
		{
			return this->age_;
		}

	private:
		char* name_;
		int age_;
	};


	void StuTest()
	{
		Stu stu01((char*)"stu01", 8);
		printf("stu01.get_name: %s", stu01.get_name());
		stu01 = Stu();
		printf("stu01.get_name: %s", stu01.get_name());
	}

	class Ts
		// vector push_back(object) demo;
		// vector.push_back����ʱҪ���з����������졣
	{
	public:
		Ts(): num_(0), c_str_(nullptr)
		{
			printf("Ts(){...} called.\n");
		}

		explicit Ts(const int n, char* s): num_(n), c_str_(s)
		{
			printf("Ts(){...} called.\n");
		}

		~Ts()
		{
			printf("~Ts()... called.\n");
		}

		Ts(const Ts& ts)
		{
			printf("Ts(const Ts& ts, copy constructor called.\n");
		}

	private:
		int num_;
		char* c_str_;
	};

	void TsTest()  // vector push_back �洢����
	// �����ӿ�������vector���push_back����ʱ�� �������Ŀ������캯����������ʱ���󣬲����뵽vector, 
	// vector�������Ƕ�̬��չ�ġ�����������2�ı��������ģ�����������ʱvector�Ὣ��ǰ����ȫ�����Ƶ��µ��ڴ�������ȫ������һ��copy���캯���� 
	// ���������Ƿǳ����µġ� ���ʵ�������ڴ���������vector����Ǵ�ָ�룬����Ҫ��ǰԤ���������������ڿռ��ƶ���
	// ���������vec.reserve(���������ɼ���vector����ƶ�(�ƶ��ֻ���ö��󿽱����캯��)
	{
		//std::vector<Ts> vec(5);
		std::vector<Ts> vec;
		vec.reserve(16);
		printf("define vector end.\n");

		vec.push_back(Ts(1, const_cast<char*>("a")));
		printf("vec.cap: %lld\n", vec.capacity());
		printf("vec.size: %lld\n", vec.size());

		vec.push_back(Ts(2, const_cast<char*>("ni")));
		printf("vec.cap: %lld\n", vec.capacity());
		printf("vec.size: %lld\n", vec.size());

		vec.push_back(Ts(3, const_cast<char*>("l")));
		printf("vec.cap: %lld\n", vec.capacity());
		printf("vec.size: %lld\n", vec.size());


		vec.push_back(Ts(4, const_cast<char*>("4")));
		printf("vec.cap: %lld\n", vec.capacity());
		printf("vec.size: %lld\n", vec.size());

		vec.push_back(Ts(5, const_cast<char*>("5")));
		printf("vec.cap: %lld\n", vec.capacity());
		printf("vec.size: %lld\n", vec.size());

		printf("end push_back. \n");

		auto i = 0;
		for (auto it = vec.begin(); it != vec.end(); ++it) {
			*it = Ts(i, const_cast<char*>("for_loop"));
			printf("iterator address: %p\n", it);
			++i;
		}
	}
}


int main(int argc, char* argv[])
{
	//vec_demo::StuTest();
	vec_demo::TsTest();
}
