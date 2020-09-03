#include <iterator>
#include <iostream>
#include <vector>

// ReSharper disable CppUseAuto

/*
 * STL iterator:
 *		LegacyInputIterator(输入迭代器), 用于序列中元素的读取, 单向顺序访问,迭代器增加一个元素,它之前的复制值都可能无效
 *		LegacyForwardIterator(前向迭代器), 用于读写元素, 单向顺序访问, 支持序列多遍扫描, forward_list容器都是向前迭代器
 *		LegacyBidirectionalIterator(双向迭代器), 用于读写元素,双向顺序访问, 支持序列多遍扫描.
 *		LegacyRandomAccessIterator(随机访问迭代器), 用于常量时间内对序列元素的任何访问能力,支持双向迭代器的所有功能.
 *		LegacyOutputIterator(输出迭代器), 用于序列中元素的写入能力,顺序单向输出.
 *		LegacyContiguousIterator(相连迭代器), 元素逻辑相邻及内存物理地址相邻.
 *	以上是迭代器的大体分类, 而每类容器都有自己的相应的迭代器. Iterator纯抽象概念:任何东西,只要行为类似iterator,它就是iterator;
 *
 */

namespace iterator_simple
{
	using namespace std;

	void RangeDemo01()
	{
		/*
		 * std::begin(vector), std::cbegin(vector)==>			cbegin(vector) ==>const iterator;
		 *	std::vector<T>::iterator it = vector.begin(),
		 *	std::vector<T>::iterator cit = vector.cbegin();
		 *	这里的std::begin == vector.begin, std::cbegin() == vector.cbegin() 这些是等效的, std::begin 更通用一些。
		 *	begin(vector)指向container第一个元素等同于数组的array[0], end(vector)指向container最后一个元素下一个元素，如果直接使用是越界；
		 *	在遍历{ for(auto& elem: vector)...}, for(auto it=begin(vector); it !=end(vector); ++it){... } 这些操作时都结束于最后一个元素的前一元素；
		 *	rbegin(vector) ==> reverse_iterator<T*>; 反向迭代器，rbegin指向末尾元素， rend指向第一个元素前面位置，
		 */
		vector<int> v1{ 1, 33, 44, 55, 11 };
		auto vi = std::begin(v1);
		cout << "*vi(begin): " << *vi << "\n";
		*vi = 101;
		vi = std::end(v1);
		cout << "*vi(end): " << *(--vi) << "\n";
		const std::vector<int>::iterator it = v1.begin();
		cout << "*it: " << *it << "\n";

		auto rit = std::rbegin(v1);
		cout << "*it(reverse_iterator_begin): " << *rit << "\n";
		rit = std::rend(v1);
		cout << "*it(reverse_iterator_end): " << *(--rit) << "\n";

		int arr[]{ 33, 44, 55, 66 };
		auto ai = std::begin(arr);
		cout << "*ai(begin): " << *ai << "\n";
		cout << "prev(ai): " << *(prev(ai)) << "\n"; // prev向后移动1位，位ai已经在第一个元素，实际上已经越界
		cout << "next(ai): " << *(next(ai)) << "\n";

		*(++ai) = 77;
		cout << "arr[1]: " << arr[1] << "\n";
	}


	// TODO:  stl的迭代器还有很多功能, 目前公此而已
}

int main(int argc, char* argv[])
{
	using namespace iterator_simple;
	RangeDemo01();
}