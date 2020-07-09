#include <iostream>
#include <cmath>

// ��Ԫ����friend�� ������ķǹ��г�Ա����һ��������ʣ�
// ��Ԫ����Ĭ��Ϊextern,
// friend function, friend class
// friend function, ��Ԫ����ʵ�����ⶨ�壬��������������������ֱ�ӷ������˽�г�Ա�ģ� �����Ƿǳ�Ա������

namespace friend_demo
{
	class Point
	{
	public:
		Point(const double x, const double y) : x_(x), y_(y)
		{
		}

		void info() const;

		// ������������ʱ����friend�������������ǳ�Ա������ ʹ��ʱ����Ҫָ���࣬ �����Ե�������˽�г�Ա��

		friend double distance(Point& a, Point& b); // �˴�ʹ��ֻΪ��ʾ��û��ʵ�����á���ֱ�Ӳ��þ�̬���� ��
		static double distance_p(Point& a, Point& b); // ���÷�����Ҫ������: Point::distance_p(...);

		// ��Ԫ�����õĶ�ĵط����������������ض��� ���Խ�class�������ô��룬 �ı�os��������ݡ�

		friend std::ostream& operator<<(std::ostream& os, Point& p);

	private:
		double x_, y_;
	};

	// ��Ԫ
	double distance(Point& a, Point& b)
	{
		const auto length = sqrt((a.x_ - b.x_) * (a.x_ - b.x_) + (a.y_ - b.y_) * (a.y_ - b.y_));
		return length;
	}

	// ��Ԫ����ʵ��operator<<
	std::ostream& operator<<(std::ostream& os, Point& p)
	{
		os << "Point.x: " << p.x_ << ", Point.y: " << p.y_ << "\n";
		return os;
	}

	void Point::info() const
	{
		std::cout << "x:  " << this->x_ << ",  y: " << this->y_ << "\n";
	}

	double Point::distance_p(Point& a, Point& b)
	{
		return sqrt((a.x_ - b.x_) * (a.x_ - b.x_) + (a.y_ - b.y_) * (a.y_ - b.y_));
	}
}

// Stack Overflow ����std::cout ��� char, signed char, unsigned char�� uint8_t����ĸ�����⡣
// ����ostream& operator<<(std::ostream& os, char c) ����operator<< ��ʽ������������֡�
// �ؼ��㣺 static_cast<int>(c);ǿ��ת��Ϊint��
// ����uint8_t i = 123�� ������cout<<i ����ǿյģ� ֻ��Ҫ��+���ɣ�  std::cout << +i << "\n";
namespace numerical_char
{
	inline std::ostream& operator<<(std::ostream& os, char c)
	{
		return std::is_signed<char>::value ? os << static_cast<int>(c) : os << static_cast<unsigned int>(c);
	}

	inline std::ostream& operator<<(std::ostream& os, signed char const c)
	{
		return os << static_cast<int>(c);
	}

	inline std::ostream& operator<<(std::ostream& os, unsigned char const c)
	{
		return os << static_cast<unsigned int>(c);
	}
}

void print_uchar()
{
	{
		// �����Ŷ��������򣬱����ڣ����ǲ��ɷ��ʵġ�
		using namespace numerical_char;
		uint8_t ch2 = 65;
		std::cout << "ch2: " << ch2 << "\n";
	}
	{
		uint8_t ch1 = 200;
		using namespace std;
		std::cout << "ch1: " << ch1 << "\n";
		ch1 = 'Z';
		std::cout << "ch1: " << ch1 << "\n"; // ��������ǿհ�
		std::cout << "ch1:" << +ch1 << "\n"; // ���������200
	}
	{
		// ����������
		uint8_t ch3 = 67;
		using namespace numerical_char;
		std::cout << "ch3: " << ch3 << "\n";
		ch3 = 'A';
		std::cout << "ch3: " << ch3 << "\n";
	}
}

int main(int argc, char* argv[])
{
	::print_uchar();
}