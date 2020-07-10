#include <vector>
#include <iostream>

namespace vec_demo
{
	class Stu
	{
	public:
		explicit Stu(char* name = nullptr, const int age = 0) : name_(name), age_(age)
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
		Ts() : num_(0), c_str_(nullptr)
		{
			printf("Ts(){...} called.\n");
		}

		explicit Ts(const int n, char* s) : num_(n), c_str_(s)
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

		int get_num() const
		{
			return this->num_;
		}

		char* get_str() const
		{
			return this->c_str_;
		}

	private:
		int num_;
		char* c_str_;
	};

	constexpr static size_t kDefaultCapacity = 16;

	void TsTest() // vector push_back �洢����
	// �����ӿ�������vector���push_back����ʱ�� �������Ŀ������캯����������ʱ���󣬲����뵽vector,
	// vector�������Ƕ�̬��չ�ġ�����������2�ı��������ģ�����������ʱvector�Ὣ��ǰ����ȫ�����Ƶ��µ��ڴ�������ȫ������һ��copy���캯����
	// ���������Ƿǳ����µġ� ���ʵ�������ڴ���������vector����Ǵ�ָ�룬����Ҫ��ǰԤ���������������ڿռ��ƶ���
	// ���������vec.reserve(���������ɼ���vector����ƶ�(�ƶ��ֻ���ö��󿽱����캯��)
	{
		//std::vector<Ts> vec(5);
		std::vector<Ts> vec;
		vec.reserve(kDefaultCapacity);
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

	void TsStoreVecEmplace()
	// vectorʹ��emplace_back��Ӷ���ʱ���ɼ���ÿ�εĿ���������̣���ʽ�ǲ�Ҫemplace_back(....)ֱ�ӹ��죬���£�
	{
		std::vector<Ts> vec_ts;
		vec_ts.reserve(kDefaultCapacity); // ָ������Capacity�����ٲ���Ҫ�������ڴ��ƶ�
		// ʹ�ô��෽ʽͬpush_back����ȫ��ͬ�ġ�
		vec_ts.emplace_back(Ts(101, const_cast<char*>("emplace_back01"))); //������ù����copy���졣
		vec_ts.emplace_back(Ts(102, const_cast<char*>("emplace_back02")));
		//

		vec_ts.emplace_back();
		vec_ts.emplace_back(103, const_cast<char*>("emplace_back03")); //����ֱ�ӳ�ʼ���� ʵ�ʵ��õ�Ĭ�Ϲ��캯��
		vec_ts.emplace_back(105, const_cast<char*>("emplace_back05")); //ʹ��emplace_backֻ��Ҫ���ù��죬������copy����һ����

		//vec_ts.push_back(104, const_cast<char*>("push_back04"));		// push_back ��֧��emplace_back��ʽ��ֱ�Ӷ����ʼ����
		printf("vec_ts: num: %d\n ", vec_ts[3].get_num()); //
		printf("vec04.c_str:  %s\n", vec_ts[4].get_str());
	}

	void TsStorePointerVec()
	{
		std::vector<Ts*> vec_ts_point;
		//vec_ts_point.reserve(kDefaultCapacity);
		vec_ts_point.push_back(new Ts(11, const_cast<char*>("point push_back01")));
		printf("vec.size: %lld\n", vec_ts_point.size());
		printf("vec.capacity: %lld\n", vec_ts_point.capacity());

		vec_ts_point.push_back(new Ts(12, const_cast<char*>("vector push_back point02")));
		printf("vec.size: %lld\n", vec_ts_point.size());
		printf("vec.capacity: %lld\n", vec_ts_point.capacity());
		vec_ts_point.push_back(new Ts(13, const_cast<char*>("vector push_back point03")));
		printf("vec.size: %lld\n", vec_ts_point.size());
		printf("vec.capacity: %lld\n", vec_ts_point.capacity());

		printf("vec_point.num:  %d\n", vec_ts_point[0]->get_num());
		printf("vec_point.str:  %s\n", vec_ts_point[1]->get_str());

		// ��������Կ����� ��������ʱTs�Ķ��󲢵���Ts���������� Ҳ���Ǵ����ڴ�й©�� ������������ڴ棬��Ҫ�ֶ������ڴ�����
		//for (auto i = 0; i < vec_ts_point.size(); ++i) {
		//	delete vec_ts_point[i];
		//	vec_ts_point[i] = nullptr;
		//}
		// vector �洢ָ�룬��Ҫ�ֶ��������ж������١�
		for(auto it = vec_ts_point.begin(); it != vec_ts_point.end(); ++it) {
			if (nullptr != *it) {
				delete *it;
				*it = nullptr;
			}
		}

		vec_ts_point.clear(); // clear ������󣬵�vector����capacity�ǲ�����ٵģ� ��Ҫ����swap������һ�յ�vector
		printf("vec.size: %lld\n", vec_ts_point.size());
		printf("vec.capacity: %lld\n", vec_ts_point.capacity());

		std::vector<Ts*> tmp;
		vec_ts_point.swap(tmp);		// ����swap��������vector������һ����vectorʵ���ڴ������ͷš�
		printf("vec.size: %lld\n", vec_ts_point.size());
		printf("vec.capacity: %lld\n", vec_ts_point.capacity());
	}

	void TsStoreIntelligentPoint()
	{
		//ʹ��ָ�����vector��Ҫ�ֶ��ͷ��ڴ棬ʹ������ָ�����������vector.
		std::unique_ptr<Ts> unique_ts1;
		std::vector<std::unique_ptr<Ts>> vec_unique_ptf_ts;
		vec_unique_ptf_ts.push_back(unique_ts1);

	}
}

int main(int argc, char* argv[])
{
	//vec_demo::StuTest();
	//vec_demo::TsTest();
	vec_demo::TsStoreVecEmplace();
	printf("Ts emplace end....\n\n");
	vec_demo::TsStorePointerVec();
}
