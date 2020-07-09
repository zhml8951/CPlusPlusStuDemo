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

		// ��Ա��������������ʽ�� ��ͬ�� constexpr int get_num() const {.... }
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
		ClsTest02() = default; // default����Ĭ�ϳ�ʼ�������ô�����
		ClsTest02(const ClsTest02&) = delete; // delete ɾ�������� Ҳ���ǲ�����ʹ�ÿ������캯��

	private:
		int num_{101}; // �����Ա�����������ó�ʼֵ��
	};

	void test_cls01()
	{
		ClsTest cls01; // �����޲ι��죬����������ջ�С� 
		ClsTest cls02{}; // ��ͬ������ʲô����д��
		ClsTest cls04{3}; //C++11 ��д�� ==> ClsTest cls04(3) .
		ClsTest cls03(); //	�����Ϊ����ֵΪClsTest���͵ĺ���cls03, cls03�޲�; ����ֱ�Ӿ��档
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
