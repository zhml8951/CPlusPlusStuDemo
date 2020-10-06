#include <random>
#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <deque>
#include <iterator>
#include <list>
#include <array>

/*
 *	https://www.cnblogs.com/jiading/p/11653911.html	(C++11随机数正确使用)
 *	random_device 非确定性随机数生成设备, Linux读取/dev/urandom; Windows由rand_s实现; 由操作系统来生成加密安全的伪随机数
 *	random_device 一般用来做其它随机算法的种子。
 *
 *	随机数基本引擎:
 *		default_random_engine,			// 通常实现为linear_congruential_engine;
 *		linear_congruential_engine,		// 线性同余法,速度快. 具体实现: minstd_rand, minstd_rand0-> knuth_b
 *		mersenne_twister_engine,		// 梅森旋转法,质量高. 具体实现: mt19937, mt19936_64
 *		subtract_with_carry_engine		// 滞后Fibonacci;   具体实现: ranlux24, ranlux48
 *
 *	随机类引擎创建后要结合随机分布模板distribution产生随机数, 常用bind(distribution, engine)方式, 相当于distribution(engine);
 *
 *	STL库集成大量随机分布类型，如：
 *		均匀分布相关(uniform_int_distribution, uniform_real_distribution);
 *		伯努利分布相关(bernoulli_distribution, binomial_distribution), 仅有yes/no两种结果;
 *		正态分布相关(normal_distribution, chi_squared_distribution, ...), normal_distribution使用比较多;
 *		分段分布相关(discrete_distribution, piecewise_constant_distribution, ...)
 *
 */
namespace random_simple
{
	void RandomMt19937Demo()
	{
		using namespace std;
		std::random_device rd; /// 使用random_device 作seed,  random_device 常用方法则是operator();
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dist(-10, 10);
		vector<int> v;
		generate_n(back_inserter(v), 20, bind(dist, gen));

		// copy复制vector到ostream_iterator, ostream_iterator输出迭代器，同类istream_iterator; 这主要代替for循环cout方式
		printf("Before sort:  ");
		std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));

		std::sort(v.begin(), v.end());

		printf("\n");
		printf("After sort:  ");
		std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, "  "));
		// 本句同下：
		printf("\nfor(item: vector){...}:  \n");
		for (auto item : v) {
			cout << item << "  ";
		}
		printf("\n");
	}

	void FormData()
	{
		std::random_device rd; // 1. 使用random_device 产生种子对象	==> 种子
		std::minstd_rand linear_eng(rd()); // 2. 使用内置minstd_rand得到随机引擎		==> 引擎

		// ReSharper disable once CppLocalVariableMayBeConst
		std::uniform_int_distribution<int> dist1(0, 1); // 3. 随机分布模板			==> 分布

		for (auto i = 0; i < 100; i++) {
			std::cout << dist1(linear_eng) << "   "; // 4. 随机分布模板调用引擎，产生随机数	==> 附加
		}
		printf("\n");
	}
}

int main(int argc, char* argv[])
{
	std::cout << "Begin random_demo.cpp. " << "\n";
	random_simple::RandomMt19937Demo();
	random_simple::FormData();
}
