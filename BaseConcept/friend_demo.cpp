#include <iostream>
#include <cmath>

// 友元机制friend， 允许类的非公有成员被另一类或函数访问，
// 友元声明默认为extern,
// friend function, friend class
// friend function, 友元函数实现类外定义，但必须在类中声明。可直接访问类的私有成员的， 但它是非成员函数。

namespace friend_demo
{
	class Point
	{
	public:
		Point(const double x, const double y) : x_(x), y_(y)
		{
		}

		void info() const;

		// 类中声明函数时加入friend，表明函数不是成员函数， 使用时不需要指明类， 但可以调用类中私有成员。

		friend double distance(Point& a, Point& b); // 此处使用只为演示，没起实质作用。可直接采用静态方法 。
		static double distance_p(Point& a, Point& b); // 调用方法需要加类名: Point::distance_p(...);

		// 友元函数用的多的地方就在类的输入输出重定向， 可以将class对象引用传入， 改变os的输出内容。

		friend std::ostream& operator<<(std::ostream& os, Point& p);

	private:
		double x_, y_;
	};

	// 友元
	double distance(Point& a, Point& b)
	{
		const auto length = sqrt((a.x_ - b.x_) * (a.x_ - b.x_) + (a.y_ - b.y_) * (a.y_ - b.y_));
		return length;
	}

	// 友元函数实现operator<<
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

// Stack Overflow 关于std::cout 输出 char, signed char, unsigned char， uint8_t是字母的问题。
// 采用ostream& operator<<(std::ostream& os, char c) 重载operator<< 方式来正常输出数字。
// 关键点： static_cast<int>(c);强制转换为int型
// 定义uint8_t i = 123。 正常用cout<<i 输出是空的， 只需要用+即可：  std::cout << +i << "\n";
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
		// 大括号定义作用域，变量在｝外是不可访问的。
		using namespace numerical_char;
		uint8_t ch2 = 65;
		std::cout << "ch2: " << ch2 << "\n";
	}
	{
		uint8_t ch1 = 200;
		using namespace std;
		std::cout << "ch1: " << ch1 << "\n";
		ch1 = 'Z';
		std::cout << "ch1: " << ch1 << "\n"; // 这里输出是空白
		std::cout << "ch1:" << +ch1 << "\n"; // 可正常输出200
	}
	{
		// 独立作用域
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