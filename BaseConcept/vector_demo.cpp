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
		// vector.push_back对象时要进行反复拷贝构造。
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

	void TsTest()  // vector push_back 存储对象。
	// 从例子看到，向vector添加push_back对象时， 会调用类的拷贝构造函数，创建临时对象，并加入到vector, 
	// vector的容量是动态扩展的。容量增长是2的倍数增长的，当容量不足时vector会将当前对象全部复制到新的内存区，又全部调用一遍copy构造函数。 
	// 这样性能是非常低下的。 最佳实践，对于大容量对象，vector最好是存指针，还需要提前预计容量，减少内在空间移动。
	// 这里调用了vec.reserve(新容量）可减少vector多次移动(移动又会调用对象拷贝构造函数)
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
