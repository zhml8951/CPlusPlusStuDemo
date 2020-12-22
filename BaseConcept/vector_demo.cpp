#include <vector>
#include <iostream>
#include <memory>
#include <cstring>
#include <algorithm>
#include <string>


// ReSharper disable CppUseAuto

namespace vec_demo
{
	class Stu
	{
	public:
		explicit Stu(char* name = nullptr, const int age = 0) : name_(name), age_(age)
		{
			std::vector<std::reference_wrapper<int>> vec;
			auto i = 888;
			vec.push_back(std::ref<int>(i));
			vec.emplace_back(std::ref(i));
			printf("%s UStu constructor called. \n", name);
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
		Stu stu01((char*)"Stu01", 8);
		printf("Stu01.get_name: %s", stu01.get_name());
		stu01 = Stu();
		printf("Stu01.get_name: %s", stu01.get_name());
	}

	class Ts
		// vector push_back(object) demo;
		// vector.push_back对象时要进行反复拷贝构造。
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

		Ts(const Ts& ts): num_(ts.num_), c_str_(ts.c_str_)
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

	void TsTest() // vector push_back 存储对象。
	// 从例子看到，向vector添加push_back对象时， 会调用类的拷贝构造函数，创建临时对象，并加入到vector,
	// vector的容量是动态扩展的。容量增长是2的倍数增长的，当容量不足时vector会将当前对象全部复制到新的内存区，又全部调用一遍copy构造函数。
	// 这样性能是非常低下的。 最佳实践，对于大容量对象，vector最好是存指针，还需要提前预计容量，减少内在空间移动。
	// 这里调用了vec.reserve(新容量）可减少vector多次移动(移动又会调用对象拷贝构造函数)
	{
		//std::vector<Ts> vec(5);
		std::vector<Ts> vec;
		vec.reserve(kDefaultCapacity);
		printf("define vector end.\n");

		vec.push_back(Ts(1, const_cast<char*>("a")));
		printf("vec.cap: %zu\n", vec.capacity());
		printf("vec.size: %zu\n", vec.size());

		vec.push_back(Ts(2, const_cast<char*>("ni")));
		printf("vec.cap: %zu\n", vec.capacity());
		printf("vec.size: %zu\n", vec.size());

		vec.push_back(Ts(3, const_cast<char*>("l")));
		printf("vec.cap: %zu\n", vec.capacity());
		printf("vec.size: %zu\n", vec.size());

		vec.push_back(Ts(4, const_cast<char*>("4")));
		printf("vec.cap: %zu\n", vec.capacity());
		printf("vec.size: %zu\n", vec.size());

		vec.push_back(Ts(5, const_cast<char*>("5")));
		printf("vec.cap: %zu\n", vec.capacity());
		printf("vec.size: %zu\n", vec.size());

		printf("end push_back. \n");

		auto i = 0;
		for (auto it = vec.begin(); it != vec.end(); ++it) {
			*it = Ts(i, const_cast<char*>("for_loop"));
			printf_s("iterator address: %p\n", &(*it));
			++i;
		}
	}

	void TsStoreVecEmplace()
	// vector使用emplace_back添加对象时，可减少每次的拷贝构造过程，方式是不要emplace_back(....)直接构造，见下：
	{
		std::vector<Ts> vec_ts;
		vec_ts.reserve(kDefaultCapacity); // 指定容量Capacity，减少不必要的整块内存移动
		// 使用此类方式同push_back是完全相同的。
		vec_ts.emplace_back(Ts(101, const_cast<char*>("emplace_back01"))); //这里调用构造和copy构造。
		vec_ts.emplace_back(Ts(102, const_cast<char*>("emplace_back02")));
		//

		vec_ts.emplace_back();
		vec_ts.emplace_back(103, const_cast<char*>("emplace_back03")); //这样直接初始化， 实际调用的默认构造函数
		vec_ts.emplace_back(105, const_cast<char*>("emplace_back05")); //使用emplace_back只需要调用构造，减少了copy构造一步。

		//vec_ts.push_back(104, const_cast<char*>("push_back04"));		// push_back 不支持emplace_back方式，直接对象初始化。
		printf("vec_ts: num: %d\n ", vec_ts[3].get_num()); //
		printf("vec04.c_str:  %s\n", vec_ts[4].get_str());
	}

	void TsStorePointerVec()
	{
		std::vector<Ts*> vec_ts_point;
		//vec_ts_point.reserve(kDefaultCapacity);
		vec_ts_point.push_back(new Ts(11, const_cast<char*>("point push_back01")));
		printf("vec.size: %zu\n", vec_ts_point.size());
		printf("vec.capacity: %zu\n", vec_ts_point.capacity());

		vec_ts_point.push_back(new Ts(12, const_cast<char*>("vector push_back point02")));
		printf("vec.size: %zu\n", vec_ts_point.size());
		printf("vec.capacity: %zu\n", vec_ts_point.capacity());

		vec_ts_point.push_back(new Ts(13, const_cast<char*>("vector push_back point03")));
		printf("vec.size: %zu\n", vec_ts_point.size());
		printf("vec.capacity: %zu\n", vec_ts_point.capacity());

		printf("vec_point.num:  %d\n", vec_ts_point[0]->get_num());
		printf("vec_point.str:  %s\n", vec_ts_point[1]->get_str());

		// 到这里可以看到， 函数结束时Ts的对象并调用Ts析构函数。 也就是存在内存泄漏。 如果不用智能内存，需要手动进行内存清理。
		//for (auto i = 0; i < vec_ts_point.size(); ++i) {
		//	delete vec_ts_point[i];
		//	vec_ts_point[i] = nullptr;
		//}
		// vector 存储指针，需要手动遍历进行对象销毁。
		for (auto it = vec_ts_point.begin(); it != vec_ts_point.end(); ++it) {
			if (nullptr != *it) {
				delete *it;
				*it = nullptr;
			}
		}

		vec_ts_point.clear(); // clear 清理对象，但vector容量capacity是不会减少的， 需要采用swap交换成一空的vector
		printf("vec.size: %zu\n", vec_ts_point.size());
		printf("vec.capacity: %zu\n", vec_ts_point.capacity());

		std::vector<Ts*> tmp;
		vec_ts_point.swap(tmp); // 采用swap交换，将vector交换到一个空vector实现内存真正释放。
		printf("vec.size: %zu\n", vec_ts_point.size());
		printf("vec.capacity: %zu\n", vec_ts_point.capacity());
	}

	void TsStoreIntelligentPoint()
	{
		//使用指针存入vector需要手动释放内存，使用智能指针管理对象存入vector.
		std::vector<std::unique_ptr<Ts>> unique_ptr_vector;

		auto ptr_ts01 = std::make_unique<Ts>(21, const_cast<char*>("unique_ptr01"));
		unique_ptr_vector.push_back(std::move(ptr_ts01));

		std::unique_ptr<Ts> ptr_ts02(new Ts(22, const_cast<char*>("unique_ptr02")));
		unique_ptr_vector.push_back(std::move(ptr_ts02));

		printf("unique_ptr_vector: %d: %s\n", unique_ptr_vector[0]->get_num(), unique_ptr_vector[0]->get_str());
		printf("unique_ptr_vector: %d: %s\n", unique_ptr_vector[1]->get_num(), unique_ptr_vector[1]->get_str());

		auto elem0 = std::move(unique_ptr_vector[0]);
		printf("elem01_num: %d \nelem01_cstr: %s\n", elem0->get_num(), elem0->get_str());

		//printf("unique_vector[0], num: %d", unique_ptr_vector[0]->get_num());

		// 使用智能指针unique_ptr还是需要重置unique_ptr，缺少reset而直接跳出函数，内存还是汇漏的。
		for (auto it = unique_ptr_vector.begin(); it != unique_ptr_vector.end(); ++it) {
			/*		auto ptr = it->release();
					delete ptr;*/
			it->reset();
		}

		printf("elem01_num: %d \nelem01_cstr: %s\n", elem0->get_num(), elem0->get_str());
	}

	void VecPushUseLoop()
	{
		std::vector<int*> data;
		for (auto i = 0; i < 10; i++) {
			auto temp = new int(i);
			data.push_back(temp);
			delete temp;
			temp = nullptr;
		}

		for (auto i = 0; i < 10; i++) {
			//通过显示可以看出来， push_back 一个指针到vector后直接进行delete会导致vector内容失效
			// 这里涉及到对象浅拷贝，删除对象会导致vector内部对象同时被删除。
			printf("vec[%d]: %d \n", i, *data[i]);
		}
	}

	void VecObjectClear()
	{
		std::vector<const char*> data;
		for (auto i = 0; i < 10; i++) {
			char char_num[4];
			char c_str[] = "vector_c-str_";
			_itoa_s(i, char_num, sizeof(char_num), 10);
			const auto length = sizeof(c_str) + sizeof(char_num);
			//strcpy_s(c_str, sizeof(c_str)+sizeof(char_num), char_num);
			strcat_s(c_str, sizeof(c_str) + sizeof(char_num), char_num);
			printf("sizeof: %zu", sizeof(c_str) + sizeof(char_num));
			const auto elem = new char[length];
			memcpy_s(elem, length + 1, c_str, length);
			printf("c_str: %s\n", elem);
			data.push_back(elem);
		}

		for (auto it = data.begin(); it != data.end(); ++it) {
			printf("*it: %s \n", *it);
		}

		const auto elem0 = data[0];
		const auto elem1 = data[1];

		data.clear();
		printf("After clear the Vector. \n");
		printf("data.size: %zu, data.capacity: %zu\n", data.size(), data.capacity());
		printf("elem0: %s\n", elem0);
		printf("elem1: %s\n", elem1);

		std::vector<const char*> tmp;
		data.swap(tmp);

		printf("After swap the Vector. \n");
		printf("data.size: %zu, data.capacity: %zu\n", data.size(), data.capacity());
		printf("elem0: %s\n", elem0);
		printf("elem1: %s\n", elem1);
	}

	/*
	 * remove_if 同 remove是一样，并不会真正删除对象，这里remove可解释为移动，即将不匹配条件对象移动到左侧，返回最后匹配对象下一迭代器；
	 * remove只简单定量匹配，remove_if可使用lambda或函数定义匹配条件。 
	 * 常用删除对象采用erase(std::remove(beginIt, endIt, conf), endIt); 拆分为
	 *	iter = remove(vec.begin(), vec.end(), 88);
	 *	vec.erase(iter, vec.end());
	 *	
	 *	iter = std::remove_if(vec.begin(), vec.end(), [](int n)->bool{ return bool};);
	 *	vec.erase(iter, vec.end()); 
	 *	合并后:
	 *		vec.erase(std::remove_if(vec.begin(), vec.end(), [](int n)->bool{return ...bool};), vec.end());
	 */

	void RemoveT3()
	{
		using namespace std;
		vector<char> vec_char{'a', 'a', 'a', 'b', 'c', 'b', 'c', 'c', 'b', 'a', 'a', 'b', 'a', 'a'};
		for (auto ch : vec_char) {
			cout << ch << "  ";
		}
		cout << endl;

		auto p = vec_char.erase(remove(vec_char.begin(), vec_char.end(), 'a'), vec_char.end());
		cout << "remove count: " << (vec_char.end() - p) << "\n";
		for (auto ch : vec_char) {
			cout << ch << "  ";
		}
		cout << endl;
	}

	void RemoveSome()
	{
		using namespace std;
		vector<int> vec_int;
		for (int i = 100; i < 200; i++) {
			vec_int.emplace_back(i);
		}
		for (int i = 0; i < vec_int.size(); i++) {
			cout << vec_int[i] << "  ";
		}
		cout << endl;


		const vector<int>::iterator iter = remove_if(vec_int.begin(), vec_int.end(),
		                                             [](const int n) -> bool { return n % 2 == 0; });
		for (auto n : vec_int) {
			cout << n << "  ";
		}

		cout << endl;
		for (auto it = vec_int.begin(); it != iter; ++it) {
			cout << *it << "  ";
		}

		cout << "\n";
		vec_int.erase(iter, vec_int.end());
		for (auto n : vec_int) {
			cout << n << "  ";
		}
	}

	void find_demo1()
	{
		std::vector<std::string> vector_01{"str01", "string02", "string03"};
		auto ret = std::find(vector_01.cbegin(), vector_01.cend(), "str01");
		if (ret == vector_01.end()) {
			std::cout << "Not Found!";
		}
		else {
			std::cout << "Fond: " << *ret << "\n";
		}
	}


	void find_if_demo()
	{
		struct ObjA
		{
			std::string str;
			int id;
		};
		auto compare_func = [&](ObjA& a) -> bool {
			if (a.id > 88) {
				return true;
			}
			return false;
		};

		std::vector<ObjA> some_obj{{"obj01", 1}, {"obj02", 200}, {"obj03", 300}};
		auto rst_iter = std::find_if(some_obj.cbegin(), some_obj.cend(), compare_func);
		if (rst_iter == some_obj.cend()) {
			printf("Not Found obj");
		}
		else {
			std::cout << "obj.id: " << rst_iter->id << ", obj.str: " << rst_iter->str << "\n";
		}
	}

	/*
	 *	[](cost std::unique_ptr<Student> student) -> bool {};
	 *	std::find_if(vec.cbegin(), vec.cend(), vec里存储为std::unique_ptr<stu>, 这样不能通过编译. 
	 *	原因在于unique_ptr不能做为值传递的,相当再个两个变量共享一个unique_ptr. 这不符合unique_ptr属性。
	 *	可使用传引用。 
	 */
	//auto ret_iter = std::find_if(
	//	vecStudents.cbegin(), vecStudents.cend(), [&](const std::unique_ptr<Student> stu) mutable -> bool {
	//		if (stu->strID == strID) {
	//			return true;
	//		}
	//		return false;
	//	});
}

int main(int argc, char* argv[])
{
	//vec_demo::StuTest();
	vec_demo::TsTest();
	printf("Ts demo end....\n\n");

	vec_demo::TsStoreVecEmplace();
	printf("Ts emplace end....\n\n");

	vec_demo::TsStorePointerVec();
	printf("end Store Pointer to Vector. \n\n");

	vec_demo::VecPushUseLoop();
	printf("End vector push use for loop and delete object. \n\n");

	vec_demo::VecObjectClear();

	printf("End vector push use unique_ptr. \n\n");
	vec_demo::TsStoreIntelligentPoint();
}
