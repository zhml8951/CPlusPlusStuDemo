#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

namespace group_demo
{
	using namespace std;

	struct Person
	{
		string name;
		int age;
		string city;
	};

	template <typename R>
	class Range
	{
	public:
		typedef typename R::value_type VType;
		explicit Range(const R& range) : range_(range) {}
		~Range() {};

		/*
		 * 实现类似SQL的group by功能
		 *	key_selector回调函数, 实例keyword_demo01.cpp中有详细说明
		 */
		template <typename Fn>
		auto GroupBy(const Fn& key_selector) -> multimap<typename result_of<Fn(VType)>::type, VType>
		{
			typedef typename result_of<Fn(VType)>::type KType;
			multimap<result_of_t<Fn(VType)>, VType> rst;
			for_each(begin(this->range_), end(range_), [&rst, &key_selector](const VType& val) {
				rst.insert(make_pair(key_selector(val), val));
			});
			return rst;
		}

		template <typename FnKey, typename FnVal>
		auto GroupUseCast(const FnKey& key_selector, const FnVal& val_selector)
						/*返回值可直接使用auto -> multimap<decltype(key_selector(*static_cast<VType*>(nullptr))),
										decltype(val_selector(*static_cast<VType*>(nullptr)))> */
		{
			typedef decltype(key_selector(*static_cast<VType*>(nullptr))) KTy;
			typedef decltype(val_selector(*static_cast<VType*>(nullptr))) VTy;
			multimap<KTy, VTy> rst;
			for_each(range_.begin(), range_.end(), [&](const VType& val) {
				rst.insert(make_pair(key_selector(val), val_selector(val)));
			});
			return rst;
		}

		template <typename FnKey, typename FnVal>
		auto GroupUseDeclval(const FnKey& key_selector, const FnVal& val_selector)
		{
			typedef decltype(declval<FnKey>()(declval<VType>())) KTy;
			typedef decltype(declval<FnVal>()(declval<VType>())) VTy;

			map<KTy, VTy> rst;
			for (const auto item : range_) {
				rst.insert(make_pair(key_selector(item), val_selector(item)));
			}
			return rst;
		}

		template <typename KeyFn, typename ValFn>
		auto GroupUseResultOf(const KeyFn& key_fn,
		                      const ValFn& val_fn) -> map<result_of_t<KeyFn(VType)>, result_of_t<ValFn(VType)>>
		{
			map<result_of_t<KeyFn(VType)>, result_of_t<ValFn(VType)>> rst;
			for_each(begin(range_), end(range_), [&rst, &key_fn, &val_fn](const VType& item) {
				result_of_t<KeyFn(VType)> key = key_fn(item);
				typename result_of<ValFn(VType)>::type val = val_fn(item);
				rst.insert(make_pair(key, val));
			});
			return rst;
		}

		template <typename Rt>
		void PrintMap(const Rt& range)
		{
			//Person p{"name1", 88, "city"};
			//auto tp = std::make_pair(p.name, p);
			//std::vector<std::pair<std::string, Person>> v{tp};
			for (auto it : range) {
				std::cout << it.first << " " << it.second.name << " " << it.second.age << " " << it.second.city << "\n";
			}
		}

		template <typename Ty, typename... Rt>
		void PrintMap(Ty t, const Rt&... range)
		{
			PrintMap(t);
			PrintMap(range...);
		}

		template <class Tuple, size_t N>
		struct TuplePrinter
		{
			static void print(const Tuple& t)
			{
				TuplePrinter<Tuple, N - 1>::print(t);
				cout << ", " << std::get<N - 1>(t);
			}
		};

		template <class Tuple>
		struct TuplePrinter<Tuple, 1>
		{
			static void print(const Tuple& t)
			{
				std::cout << "finished. " << std::get<0>(t);
			}
		};

		template <class... Args>
		void PrintTuple(const std::tuple<Args...>& t)
		{
			std::cout << "(";
			TuplePrinter<decltype(t), sizeof...(Args)>::print(t);
			std::cout << ")\n";
		}

	private:
		R range_;
	};

	inline void TestGroupBy()
	{
		const vector<Person> users{
			{"aa", 20, "SH"},
			{"bb", 25, "BJ"},
			{"cc", 25, "KS"},
			{"dd", 20, "KS"},
			{"bb", 25, "BJ"},
			{"aa", 25, "SH"},
		};

		Range<vector<Person>> range1{users};
		auto result1 = range1.GroupBy([](const Person& p) -> int { return p.age; });
		for (auto item : result1) {
			cout << "key: " << item.first << ", value: " << item.second.name << ", " << item.second.city << "\n";
		}

		const auto result2 = range1.GroupUseCast(
			[](const Person& p) { return p.name; },
			[](const Person& p) { return p.age; });
		for (auto it = result2.cbegin(); it != result2.cend(); ++it) {
			cout << "key: " << it->first << ", value: " << it->second << "\n";
		}

		const auto result3 = range1.GroupUseDeclval(
			[](const Person& p) { return p.name; },
			[](const Person& p) { return p.age; });

		for_each(begin(result3), end(result3),
		         [](const pair<string, int> p) {
			         cout << "key:" << p.first << ", value: " << p.second << "\n";
		         });

		const auto result4 = range1.GroupUseResultOf(
			[](const Person& person) { return person.age; },
			[](const Person& person) { return tie(person.name, person.city); });

		for (auto item : result4) {
			std::cout << "key: " << item.first << ",value: "
				<< std::get<0>(item.second) << "," << get<1>(item.second) << "\n";
		}

		//const auto result5 =
	}
}
