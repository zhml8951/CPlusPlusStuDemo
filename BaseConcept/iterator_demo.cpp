#include <iterator>
#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <deque>
#include <array>
#include <string>

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

	/* <iterator>:  back_inserter(C) 为C 创建一个back_insert_iterator; 同理还有front_inserter => front_insert_iterator
	 * back_inserter为容器快捷创建末部插入器，容器需支持push_back, emplace_back; 如vector
	 * back_inserter, front_inserter 返回insert迭代器, 那么insert_iterator同 begin返的迭代器有什么不同呢？
	 */
	void FrontBackInserterDemo()
	{
		// generate_n use back_insert_iterator 使用generate_n对vector进行填充
		[]() -> void {
			std::vector<int> v;
			// generate ==> Generate values for range with function;
			// generate_n ==> Generate values for sequence with function;
			std::generate_n(std::back_insert_iterator<std::vector<int>>(v), 10,
				[n = 0]()mutable { return ++n; });
			for (auto n : v) {
				printf("%d  ", n);
			}
			printf("\n");
		}();

		// back_inserter(Container) 快捷创建 std::back_insert_iterator<Ty>(Object)对象。 C++同类方式很多， 大多make_...; 如：make_shared, make_pair,make_tuple
		// std::make_unique<Ty>(...) 快捷创建 std::unique_ptr<Ty>对象. ref创建reference_wrapper<
		[]()-> void {
			std::vector<int> numbers{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
			std::fill_n(std::back_inserter(numbers), 3, -1);

			for (auto n : numbers) {
				printf("%d  ", n);
			}
			printf("\n");
		}();

		// 从vector使用back_inserter, copy数据到list, deque. 迭代器可以使用back_insert_iterator, front_insert_iterator;
		[]()-> void {
			std::vector<int> v{ 1, 2, 3, 4, 5 };
			std::deque<int> db1;
			std::list<int> list1;
			std::array<int, 10> array1; //// array 不支持push_back; 故后面的copy(... back_insert_iterator<...>不能编译
			array1.fill(0);
			//std::copy(v.begin(), v.end(), std::front_inserter(d));
			std::copy(v.begin(), v.end(), std::back_inserter(db1));
			std::copy(v.begin(), v.end(), std::front_inserter(list1));
			//std::copy(v.begin(), v.end(), std::back_insert_iterator<std::array<int,10>>(array1));
			//std::copy(v.begin(), v.end(),std::begin(d));
			auto ait = array1.begin();
			for (auto vit = v.begin(); vit != v.end(); ++vit) {
				*ait = *vit;
				++ait;
			}

			for (auto it = db1.begin(); it < db1.end(); ++it) {
				printf("%d  ", *it);
			}
			printf("\n");

			for (auto i = 0; i < array1.size(); i++) {
				printf("%d  ", array1[i]);
			}
			printf("\n");

			for (auto it = list1.begin(); it != list1.end(); ++it) {
				printf("%d   ", *it);
			}
			printf("\n");
		}();
	}

	// TODO:  stl的迭代器还有很多功能, 目前公此而已
}

int main(int argc, char* argv[])
{
	using namespace iterator_simple;
	RangeDemo01();
}