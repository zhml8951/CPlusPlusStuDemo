#include <iostream>
#

namespace test_class1
{
	class ClsTest
	{
	public:
		//ClsTest() = default;
		explicit ClsTest(): num_(1)
		{
			std::cout << " Class called ClsTest(). num: " << this->num_ << "\n";
		}

		explicit ClsTest(const int num): num_(num)
		{
			std::cout << "Class called ClsTest(int). num:" << this->num_ << "\n";
		}

		void test_func() const
		{
			std::cout << "Output member variable(num):  " << this->num_ << '\n';
		}

		// 成员函数特殊命名方式， 等同于 constexpr int get_num() const {.... }
		constexpr int (get_num)() const
		{
			return this->num_;
		}

	private:
		int num_;
	};

	class ClsTest02
	{
	public:
		ClsTest02() = default; // default定义默认初始函数，用处不大
		ClsTest02(const ClsTest02&) = delete; // delete 删除函数， 也就是不可以使用拷贝构造函数

	private:
		int num_{101}; // 定义成员变量，并设置初始值。
	};

	void test_cls01()
	{
		ClsTest cls01; // 调用无参构造，创建对象在栈中。 
		ClsTest cls02{}; // 等同于上面什么都不写。
		ClsTest cls04{3}; //C++11 新写法 ==> ClsTest cls04(3) .
		ClsTest cls03(); //	可理解为返回值为ClsTest类型的函数cls03, cls03无参; 这里直接警告。
		auto cls05 = new ClsTest();
		ClsTest* cls06 = new ClsTest;
		ClsTest* cls07 = new ClsTest{88};
		std::cout << "get_num: " << cls07->get_num() << "\n";

		delete cls05;
		delete cls06;
		delete cls07;
	}
}

int main(int argc, char* argv[])
{
	test_class1::test_cls01();
}
