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

		char* name() const
		{
			return this->name_;
		}

	private:
		char* name_;
		int age_;
	};


	void stu_test()
	{
		Stu stu01((char*)"stu01", 8);
		printf("stu01.name: %s", stu01.name());
		stu01 = Stu();
		printf("stu01.name: %s", stu01.name());
	}

	// vector push_back(object) demo;
	class Ts
	// vector.push_back对象时要进行反复拷贝构造。
	{
	public:
		Ts():num_(0), c_str_(nullptr) { printf("Ts(){...} called.\n"); }
		explicit Ts(int n, char* s): num_(n), c_str_(s) { printf("Ts(){...} called.\n"); }

		~Ts() { printf("~Ts()... called.\n"); }

		Ts(const Ts& ts) { printf("Ts(const Ts& ts, copy constructor called.\n"); }

	private:
		int num_;
		char* c_str_;
	};

	void ts_test()
	{
		//std::vector<Ts> vec(5);
		std::vector<Ts> vec;
		vec.resize(6);
		printf("define vector end.\n");
		vec.push_back(Ts(1, const_cast<char*>("a")));
		vec.push_back(Ts(2, const_cast<char*>("ni")));
		vec.push_back(Ts(3, const_cast<char*>("l")));
		printf("vec.size: %lld\n", vec.size());
		printf("end push_back. \n");

		int i = 0;
		for(auto it = vec.begin(); it != vec.end(); ++it) {
			*it = Ts(i, const_cast<char*>("for_loop"));
			printf("iterator address: %p\n", it);
			++i;
		}
	}
}


int main(int argc, char* argv[])
{
	//vec_demo::stu_test();
	//vec_demo::ts_test();

}

///
