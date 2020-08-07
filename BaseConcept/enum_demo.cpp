#include <iostream>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppUseAuto

namespace enum_sample
{
	/*
	 * 旧版enum 存在的几个小问题：
	 *	1. 向整型的隐式转换(整型提升)，只能是整型数；
	 *	2. 无法指定enum底层使用的数据类型
	 *	3. enum 的作用域
	 *	4. 不同编译器解决方法不统一 
	 *	
	 */

	void enum_convert_int()
	{
		enum ColorA { R, G, B, Y, M };
		enum ColorB { Red, Green, Blue, Yellow };

		const auto test1 = [&](const int data) -> void {
			std::cout << "test called." << "\n";
			std::cout << "color: " << data << "\n";
		};

		ColorA color_a(R);
		ColorB color_b(Blue);
		bool rst(color_a < color_b); // 这里由于int 隐式转换bool会截断，故警告。
		std::cout << std::boolalpha << rst << "\n";

		std::cout << "color_b:  " << color_b << "\n";
		color_b = Yellow; // enum内的成员直接成为全局量。
		std::cout << "color_b:  " << color_b << "\n";

		rst = B; // 旧式enum 自动成为全局作用域，随处可以看到。 如果需要全局量，当然更好。 
		std::cout << "bool_rst:  " << rst << "\n";

		//color_a = Green;
		color_a = Y;
		std::cout << "color_a: " << color_a << "\n";
		//color_b = 2;
		std::cout << "ColorB: " << ColorB(Red) << "\n";
		test1(color_a); // enum自动隐式转换为int;
	}

	enum SmallEnum : int16_t { a, b, c };

	enum Color { red, green, yellow = 20, blue };

	enum class Altitude : char { high = 'H', low = 'L' };

	enum { d, e, f=e + 2 };

	std::ostream& operator<<(std::ostream& os, const Color c)
	{
		switch (c) {
		case red: os << "red";
			break;
		case yellow: os << "yellow";
			break;
		case green: os << "green";
			break;
		case blue: os << "blue";
			break;
		default: os.setstate(std::ios_base::failbit);
		}
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const Altitude al)
	{
		return os << static_cast<char>(al);
	}

	void EnumTest01()
	{
		Color color = red;
		Altitude a = Altitude::low;
		std::cout << "color: " << color << "\n"
			<< "altitude: " << a << "\n"
			<< "f: " << f << "\n";
	}
}

int main(int argc, char* argv[])
{
	enum_sample::enum_convert_int();
	enum_sample::EnumTest01();
}
