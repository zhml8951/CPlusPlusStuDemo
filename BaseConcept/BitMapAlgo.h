#pragma once
#include <vector>

/*
 BitMap 概念：Bit对应二进制，Map图， 二进制图，大量数据处理高性能算法, 更优算法Bloom Filter(布隆过滤器)
	基本概念： 用一个bit位来标记某个元素对应的value,而key即是该元素， 用数组和每一个数据的每一个二进制位来表示一个数据
	0表示不存在， 1表示存在。例：int 322; 由于int类型4Byte,也就是32bits, 322>>5(322/32)=10, 322%32=2.
	BitMap位在10个数第2位， 即vec[10] = 2 。
 */

namespace bit_map
{
	class BitMap
	{
	public:
		explicit BitMap(const size_t num)
		{
			data_.reserve((num >> 5) + 1);
		}

		void set(size_t num);
		void reset(size_t num);
		bool has_existed(size_t num);
	private:
		std::vector<size_t> data_;
	};
}
