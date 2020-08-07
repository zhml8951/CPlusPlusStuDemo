#include <iostream>
#include <string>
#include <cstring>
#include <vector>

/*
 *  内存对齐是CPU结构的一部分，当CPU访问正确对齐的数据时效率最高的。
 *  class, struct, 每个成员相对于这个结构体变量地址的偏移量正好是该成员类型所占字节的整数倍，
 *  且最终占用字节数为成员类型中最大占用字节数的整数倍。
 */

 /*
  * union 是特殊的类类型， 它在某一时刻只能保持有其一个非静态数据成员。 union的大小是最大成员的大小。 其它成员分配于该最大成员的一部分相同字节。
  */
namespace union_sample
{
	/*
	 * 通过反复调整struct里的char[]，可以看到，struct 内存对齐是对最宽成员的成员大小做为基数，这里是double，8字节。也就是对象方式以8的倍数对齐。
	 * 当char[10]时，sizeof(Student)=24.而实际使用22；当char[21]时，实际分配40，而struct使用只有33，也就是中间有7字节空隙。
	 * 本质上class的内存分配和补齐方式同struct是相同的。
	 */

	struct Student
	{
		char name[10]; // size: 10, 21, 17
		long num; // size:
		double score;
	};

	/*
	 * union使用方式同struct基本相同，但内存分配方式不同，struct各个成员内存是排列相加的，也就是各成员内存有偏移值，但union的各成员都是相同起点，
	 * 这就导致各成员值相互覆盖，union 内存对齐方式同struct,以最大宽度成员的倍数增长，不足部分会自动补齐。
	 *
	 */

	union UStu
	{
		char name[10];
		long num;
		double score;
	};

	void Test1()
	{
		Student stu01;
		printf("char[10].size: %llu,  long.size: %llu,  double.size %llu\n",
			sizeof(stu01.name), sizeof stu01.num, sizeof(stu01.score));
		std::cout << "struct.size: " << sizeof(stu01) << "\n";

		const char* name = "stu01";
		strcpy_s(stu01.name, 10, name);
		std::cout << "stu01.name: " << stu01.name << "\n";

		UStu union01;
		std::cout << "union01: " << sizeof(union01) << "\n";
		union01.num = 8;
		std::cout << "union01.num:  " << union01.num << "\n";

		const char* u_name = "union01";
		strcpy_s(union01.name, 10, u_name);
		std::cout << "union01.name: " << union01.name << "\n";
		union01.score = 8.8;
		std::cout << "union01.name: " << union01.score << "\n";
	}

	union EndianTest
	{
		int8_t u[4];
		int32_t i;
	};

	// 采用union来判断系统大小端。
	bool LittleEndSystem()
	{
		EndianTest t1;
		t1.i = 1;
		std::cout << "&t1.i: " << &t1.i << "\n";
		std::cout << "&t1.u: " << &t1.u << "\n";
		const auto is_little_endian = t1.u[0] == 1;
		std::cout << std::boolalpha << is_little_endian << "\n";
		return is_little_endian;
	}

	union S
	{
		int32_t n; // 4Bytes;
		uint16_t s[2]; // 2*2Bytes;
		uint8_t c; // 1Byte;
	};

	void TestS()
	{
		S s = { 0x12345678 };
		std::cout << std::hex << "s.n: " << s.n << "\n";
		std::cout << std::hex << "s.s: " << s.s[1] << "\n" << "s.c: " << s.c << "\n";
		s.s[0] = 0x0011;
		std::cout << "s.s:  " << s.s << "\n"
			<< "s.n: " << s.n << "\n"
			<< "s.c: " << s.c << "\n";
	}

	union S2
	{
		std::string str;
		std::vector<int> vec;
		~S2(){}
	};

	void TestS02()
	{
		S2 s2 = {"TestS02 String"};
		std::cout << "S.str:  " << s2.str << "\n";
		s2.str.~basic_string();
		// vector union new 特殊用法。
		new (&s2.vec) std::vector<int>;
		//s2.vec = {10};
		s2.vec.push_back(10);
		std::cout << "S2.vec: " << *(s2.vec.begin()) << "\n";
		s2.vec.~vector();
	}
}

int main(int argc, char* argv[])
{
	union_sample::Test1();
	union_sample::LittleEndSystem();
	union_sample::TestS();
	union_sample::TestS02();
}