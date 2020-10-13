/*
 * 实例来源: https://www.cnblogs.com/qicosmos/p/5277011.html;
 *	项目来源: https://github.com/qicosmos/Kapok; C++ 序列化库，这里主要理解宏高级用法及使用新C++14重写后的效果
 *	部分代码与 macro.cpp 重合， 这里重点在于使用C++14重写思路
 */

#include <iostream>
#include <tuple>
#include <string>
#include <array>
#include <functional>

namespace kapok_macro
{
	/*
	 *	实例主要目标，将参数(可变参数)转换为key-value的pair, 将字段名与值组成KV序列，各个字段组合成tuple; 如：
	 *		struct Person{
	 *		  std::string name;
	 *		  int age;
	 *		  EMBED(name, age)
	 *		};
	 *		Person p1{"name01", 20};
	 *		组合成功后，可以得到p1.Tuple({"name", std::ref("name01")}, {"age", 20});
	 *	采用宏显得非常复杂， 这里最多只能10个参数，也就是10个元素，bug 在于如果空参数时也显示1个参数;
	 *	当超过10个参数时，编译会出错。
	 */

#define MACRO_ARGS(...) __VA_ARGS__
#define MACRO_EXP(arg_list) arg_list
#define APPLY_VARIADIC_MACRO(macro, args) macro args

#define MAKE_ARG_LIST_1(op, arg, ...) op(arg)
#define MAKE_ARG_LIST_2(op, arg, ...) op(arg), MACRO_EXP(MAKE_ARG_LIST_1(op, __VA_ARGS__))
#define MAKE_ARG_LIST_3(op, arg, ...) op(arg), MACRO_EXP(MAKE_ARG_LIST_2(op, __VA_ARGS__))
#define MAKE_ARG_LIST_4(op, arg, ...) op(arg), MACRO_EXP(MAKE_ARG_LIST_3(op, __VA_ARGS__))
#define MAKE_ARG_LIST_5(op, arg, ...) op(arg), MACRO_EXP(MAKE_ARG_LIST_4(op, __VA_ARGS__))
#define MAKE_ARG_LIST_6(op, arg, ...) op(arg), MACRO_EXP(MAKE_ARG_LIST_5(op, __VA_ARGS__))
#define MAKE_ARG_LIST_7(op, arg, ...) op(arg), MACRO_EXP(MAKE_ARG_LIST_6(op, __VA_ARGS__))
#define MAKE_ARG_LIST_8(op, arg, ...) op(arg), MACRO_EXP(MAKE_ARG_LIST_7(op, __VA_ARGS__))
#define MAKE_ARG_LIST_9(op, arg, ...) op(arg), MACRO_EXP(MAKE_ARG_LIST_8(op, __VA_ARGS__))
#define MAKE_ARG_LIST_10(op, arg, ...) op(arg), MACRO_EXP(MAKE_ARG_LIST_9(op, __VA_ARGS__))

#define ADD_REFERENCE(t) std::reference_wrapper<decltype(t)>(t)
#define ADD_REFERENCE_CONST(t) std::reference_wrapper<std::add_const_t<decltype(t)>>(t)

#define PAIR_OBJECT(t) std::make_pair(#t, ADD_REFERENCE(t))
#define PAIR_OBJECT_CONST(t) std::make_pair(#t, ADD_REFERENCE_CONST(t))

#define MAKE_TUPLE(...) auto Tuple(){return std::make_tuple(__VA_ARGS__);}
#define MAKE_TUPLE_CONST(...) const auto TupleConst(){return std::make_tuple(__VA_ARGS__);}

#define MACRO_CONCAT_INNER(A, B) A##_##B
#define MACRO_CONCAT(A, B) MACRO_CONCAT_INNER(A, B)

#define MAKE_ARG_LIST(N, op, arg, ...) MACRO_CONCAT(MAKE_ARG_LIST, N)(op, arg, __VA_ARGS__)

#define EMBED_TUPLE(N, ...) \
	MAKE_TUPLE(MAKE_ARG_LIST(N, PAIR_OBJECT, __VA_ARGS__)) \
	MAKE_TUPLE_CONST(MAKE_ARG_LIST(N, PAIR_OBJECT_CONST, __VA_ARGS__))

#define NUM_SEQ() 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
#define ARG_N(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N
#define GET_ARG_COUNT_INNER(...) APPLY_VARIADIC_MACRO(ARG_N, (__VA_ARGS__))
#define GET_ARG_COUNT(...) GET_ARG_COUNT_INNER(0, ##__VA_ARGS__, NUM_SEQ())

#define EMBED(...) EMBED_TUPLE(GET_ARG_COUNT(__VA_ARGS__), __VA_ARGS__)

	struct Person
	{
		std::string name;
		int age;
		EMBED(name, age)
	};

	void test_person()
	{
		Person p1 = {"tom", 33};
		auto tp = p1.Tuple();
		std::cout << std::get<0>(tp).first << ": " << std::get<0>(tp).second << "\n";
	}
}

namespace kapok_template
{
	/*
	 * 采用宏变参实现多元素组合成kv对，实现比较冗杂。 采用C++14的template 实现只需要更少代码即可；
	 */

	template <size_t N>
	std::array<std::string, N> split(const std::string& s, const char delimiter)
	{
		size_t start = 0;
		auto end = s.find_first_of(delimiter);
		std::array<std::string, N> output;
		size_t i = 0;
		while (end <= std::string::npos) {
			output[i++] = std::move(s.substr(start, end - start));
			if (end == std::string::npos) break;

			start = end + 2;
			end = s.find_first_of(delimiter, start);
		}
		return output;
	}

	/*
	 *	参数包typename... Args 递归调用的终止函数;
	 */
	template <size_t N, typename T>
	static auto Make(const std::array<std::string, N>& arr, size_t index, const T& args)
	{
		return args;
	}

	template <size_t N, typename T1, typename T2, typename... Args>
	static auto Make(const std::array<std::string, N>& arr, size_t index, T1 const& t, T2& first, Args&... args)
	{
		// 这里会不断递归调用自身， 参数包Args... 会不断向析出first, 直到args...全部析完，会调用重载的Make终止函数
		return Make(arr, index + 1, std::tuple_cat(t, std::make_tuple(std::pair<std::string, T2&>(arr[index], first))),
		            args...);
	}
	//TODO 后续这里问题需要更深入理解

#define VA_ARGS_NUM(...) \
  std::tuple_size<decltype(std::make_tuple(__VA_ARGS__))>::value

#define META(...)                                                  \
  auto meta() {                                                    \
    auto arr = split<VA_ARGS_NUM(__VA_ARGS__)>(#__VA_ARGS__, ','); \
    return Make(arr, 0, std::tuple<>(), __VA_ARGS__);              \
  }

	struct Per
	{
		size_t id;
		std::string name;
		META(id, name)
	};
}

int main(int argc, char* argv[])
{
	std::string name;
	int age;
	printf("macro_adv.cpp test....\n");
	kapok_macro::test_person();
}
