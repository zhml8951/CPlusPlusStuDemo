#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <map>
#include <list>
#include <numeric>
#include <random>

// C++ 关键词及关键技术点记录

/*
 *	explicit(显式) C++11只用于修饰构造函数；与explicit对应的是implicit; 构造函数默认都是implicit;
 *	构造函数默认的implicit， 当单参调用implicit的构造函数, 通常称之为 转换构造函数(converting constructor),这个带来一定方便性，但可能会带来意外的隐式类型转换
 *	explicit 不需要添加默认构造，多参构造(总是显示构造)。只需要添加单参构造，目的是防止不必要的隐式类型转换
 *	需要注意单参构造（多参构造但只有一个参数没有默认值）时，采用赋值时会产生隐式类型转换，这类转换有时并不是我们需要的。 explicit阻止隐式类型转换；
 */
 /* ReSharper 可提供最佳代码实践， 如下： NonExplicitConvertingConstructor, NonExplicitConversionOperator, UseAuto... 这里先禁用这些*/

 // ReSharper disable CppNonExplicitConvertingConstructor
 // ReSharper disable CppNonExplicitConversionOperator
 // ReSharper disable CppUseAuto

using namespace std;

namespace keywords_simple
{
	/*
	 *	本例主演示explicit 在class的作用.  总括一句话，explicit防止不必要的隐式类型转换
	 */
	class FractionNoExplicit
	{
	public:
		FractionNoExplicit(const int num, const int den = 1) : numerator_(num), denominator_(den)
		{
			printf("Called constructor.\nnumerator: %d, denominator: %d\n", num, den);
		}

		FractionNoExplicit(const FractionNoExplicit& o_explicit)
		{
			printf("Copy constructor.\n");
			this->numerator_ = o_explicit.numerator_;
			this->denominator_ = o_explicit.denominator_;
		}

		FractionNoExplicit& operator=(const FractionNoExplicit& o_explicit)
		{
			printf("Copy assignment constructor.\n");
			this->numerator_ = o_explicit.numerator_;
			//this->denominator_ = o_explicit.denominator_;
			//为了测试，将copy assignment; 此类成员变量都是基础类型数据时不需要自定义copy assign function. 编译器自动生成copy assign 可完成相同任务，
			//当类成员变量是复合类型(指针，引用)或其它结构，则需要考虑copy assignment function.

			return *this;
		}

		/*
		 * type conversion operator function. (类型转换操作符函数)， 没有explicit可自动隐式转换
		 */
		operator double() const
		{
			printf("Called operator conversion function.\n");
			return static_cast<double>(numerator_ / denominator_);
		} ///type conversion function

		void Display() const
		{
			printf("numerator/denominator = %d/%d\n", this->numerator_, this->denominator_);
		}

	private:
		int numerator_;
		int denominator_;
	};

	void no_explicit_test()
	{
		FractionNoExplicit f1(3, 5);
		double rst = 3.0 + f1; // f1产生隐式类型转换，调用operator double()， 这里f1有双重类型， 即是类类型，也是double类型
		std::cout << "Result: " << rst << "\n";

		f1 = 88; // 这里调用FractionN..(单参）构造, 同样也是隐式类型转换, 调用实质样式： f1 = FractionNoExplicit tmp(88);
		// 先调用Constructor, 再调用Copy assignment Function; 调用的实质是构造函数是单参，可进行隐式类型转换，也就将整型隐式转换为FractionNoEx...;
		f1.Display();
	}

	/*
	 * 类型转换分为: 提升转换(Promotion), 标准转换， 自定义转换；
	 *	提升转换: char,unsigned char,short,unsigned short 自动提升到int, unsigned int; float类型提升到double;
	 *	标准转换: int 转char, long 转double..
	 */

	 /* C++ 一致初始化， 早期C++初始化有多种方式，很容易混淆。故引用列表初始化(一致初始化)｛..｝
	  * C++11 列表初始化(一致初始化) 内部实现的本质是用 initializer_list<T>来实现，而initializer_list<>实质也就是array
	  */

	void Print(const std::initializer_list<int> values)
	{
		for (auto p = values.begin(); p != values.end(); ++p) {
			std::cout << "p_address: " << p << ",  value: " << *p << "\n";
		}
		for (auto val : values) {
			std::cout << "val: " << val << "\n";
		}
		/*
		 * 使用auto时，默认是copy构造的，尽量使用reference( auto& )，如果需需要常量引用则const auto&;
		 *
		 */
		vector<int> vec{ 16, 26, 36, 46, 56 };
		for (auto& elem : vec) {
			elem += 3;
		}

		cout << "Reference use auto&. \n";
		for (const auto& elem : vec) {
			cout << "elem_address: " << &elem << ", elem_value:  " << elem << "\n";
		}

		for (auto elem_ptr = vec.begin(); elem_ptr != vec.end(); ++elem_ptr) {
			cout << "ptr_address: " << &(*elem_ptr) << ", value: " << *elem_ptr << "\n";
		}

		cout << "Copy use auto.\n";
		for (auto elem : vec) {
			// auto elem ==> vector<int>::value_type; 这里的value_type即Traits起到特性提取出来的。
			cout << "elem_address: " << &elem << ", value: " << elem << "\n";
		}
		// 使用auto, 不使用reference, 实质是copy constructor,创建了新的临时对象, 通过address可以看出。
		// 使用auto it = vec.begin(); 迭代器实质是指针包装出来的， 通过&(*it)可取地址。
	}

	// 隐式类型转换(类型转换)
	class C
	{
	public:
		C() = default;
		explicit C(const string& s) : str_(s) {};
		//C(const string& s): str_(s) {}
		string GetElem() const { return this->str_; }
	private:
		string str_;
	};

	void test_class()
	{
		vector<string> vs{ "first", "second", "three" };

		for (const auto& el : vs) {
			cout << "el.size: " << el.size() << "el.type: " << typeid(el).name() << "\n";
			auto c1 = static_cast<C>(el);
			cout << c1.GetElem() << "\n";
		}
	}

	/*
	 * Type Trait
	 * C++ STL以template为根基，为减少template编程的重载数，引用TypeTrait， 它是template在编译期根据TypeTrait产出一个type和value;
	 */
	template <typename T>
	void TraitsFoo(const T& val)
	{
		// is_pointer<T>产出类型true_type或false_type; 而value只有两种结果： true, false;
		if (std::is_pointer<T>::value) {
			std::cout << "TraitFoo() called for a pointer." << "\n";
		}
		else {
			std::cout << "TraitFoo() called for a pointer. " << "\n";
		}
	}

	/*
	 *	内部实现FooImpl有多个重载, 而外部统一调用Foo,降低调用复杂性, 初步体现TypeTraits作用，TypeTraits的关键作用就在根据template<class T>里的T类型来调用相应重载，
	 *	这里的关键在于对template<class T>进行判断，
	 *	TypeTrait是泛型代码(Generic Code)的基石
	 */
	 //--------------------------------------------------------------------------------------------//

	template <typename T>
	void FooImpl(const T& val, std::true_type)
	{
		std::cout << "FooImpl() Called for pointer to: " << *val << ", Address: " << val << "\n";
	}

	template <typename T>
	void FooImpl(const T& val, std::false_type)
	{
		std::cout << "FooImpl() Called for value to: " << val << ", Address: " << &val << "\n";
	}

	template <typename T>
	void Foo(const T& val)
	{
		FooImpl(val, std::is_pointer<T>());
	}

	/*
	 * decltype 是关键词， declval<T> 是模板函数，它的作用是将T替换成T&&， 并将T&&作为返回对象， 目的是绕过一个对象的构造函数，直接使用这个对象的成员函数.
	 * declval<T>不能作为常规模板函数使用，必须配合decltype使用， 可以在编译期获得这个对象的成员函数的返回类型。
	 */
	template <typename T1, typename T2>
	struct CommonType
	{
		typedef decltype(true ? declval<T1>() : declval<T2>()) Type;
	};

	/*
	 *	常用类型推导: auto,  decltype,  result_of, declval;
	 *	auto与auto*在使用时没有本质区别, 即指针类型可以自动推导出来. 但auto& 与auto就不同了，即如果使用auto默认是值， auto&才是引用。
	 *	auto与cv-qualifier要结合使用, auto并不能从表达式中带走cv-qualifier; 故当需要const或volatile时，需添加进去, const auto, volatile auto;
	 *	decltype 用库开发， 用于复杂类型推导
	 *	declval 常用使用样式  decltype(declval<T>().func()),
	 */

	struct Person
	{
		std::string name;
		int age;
		std::string city;
	};

	/*
	 *	实现SQL的group by功能， 如：
	 *		vector<Person> vec = {{"name1", 20, "ks"}, {"name2", 22, "sz"}, {"name3",22,"ks"}, {"name4", 20, "sz"}};
	 *		如果我们按age分组，则效果如：{20, {"name1",..}}, {20, {"name4"...}}, {22, {"name2...}}, {22, {"name3",..}};
	 *	如果不使用template则需要分别定义根据name,age,city的分组函数，如果字段很多,会产生大量重复代码；
	 */

	 // 最初版本，T为selector_key的类型， 参数需要传入key;
	template <typename T>
	auto GroupBy0(const vector<Person>& persons, const T& key) -> multimap<T, Person>
	{
		multimap<T, Person> rst;
		for_each(begin(persons), end(persons), [&](const Person& person) {
			// 这里只能按age分类，如果需要city或其它字段分类，就必须要再copy一份
			rst.insert(make_pair(person.age, persons)); // 这里person.age如何换成key呢？key又如何与person相关字段关联呢？
			});
		return rst;
	}

	// template 需要两个类型， Type(key的数据类型), Func(分组key选择函数);功能使用没有问题，但每次调用都必须指明key的类型；
	template <typename T, typename Fn>
	multimap<T, Person> GroupBy1(const vector<Person>& persons, const Fn& key_selector_fn)
		// 回调函数Fn&由于lambda都是临时变量，可以采用F&&右值引用，或者必需使用const;
	{
		multimap<T, Person> map;

		// 由于lambda传过来的Person是临时右值，这里必要const,或者Person&&, 即右值引用
		for_each(persons.begin(), persons.end(), [&](const Person& person) {
			// key_selector_fn(person)实质由回调函数实现类型擦除，类型选择由调用者提供函数处理
			map.insert(make_pair(key_selector_fn(person), person));
			});
		return map;
	}

	// 难点在于确定multimap<...> 的Type需要提前确定，使用方法:
	//  decltype(func(*static_cast<P*>(nullptr))), 这里使用nullptr强制转换为Person*指针再取值，相对于定义了一个Person对象；
	//  decltype(declval<Fn>()(declval<Person>()), 这种用法也是很难理解的。
	//

	template <typename Fn>
	auto GroupBy2(const vector<Person>& persons, const Fn& key_selector_fn)
	{
		// 返回值只能用auto, //TODO 如果不用auto, 返回值该如何写？？？
		//typedef decltype(key_selector_fn(*static_cast<Person*>(nullptr))) KeyTy;
		using KeyTy = decltype(key_selector_fn(*static_cast<Person*>(nullptr)));
		// 使用decltype可解决multimap<key_type? 问题，但*static_cast<Person*>(nullptr)是如何运行的呢？

		//using KeyTy = decltype(key_selector_fn(*((Person*)0)));  //C风格强制转换,
		//((Person*)0)->name;	// C风格0相当于NULL, =>
		//(*((Person*)0)).name = "C static_cast";		// C风格强制转换.
		//(*static_cast<Person*>(nullptr)).name = "static_cast<";	// C++风格，效果相同
		// 这里实质是强制类型转换 (Person*)0 => static_cast<Person*>(nullptr);

		multimap<KeyTy, Person> rst;
		for_each(begin(persons), end(persons), [&rst, &key_selector_fn](const Person& person) {
			rst.insert(make_pair(key_selector_fn(person), person));
			});
		return rst;
	}

	template <typename Fn>
	auto GroupBy3(const vector<Person>& persons, const Fn& key_selector_fn)
	{
		typedef decltype(declval<Fn>()(declval<Person>())) KeyTy;
		/*
		 * 这里需要分4步解：
		 *	1. decltype(....) 由内部declval返回值来确定type;
		 *	2. declval<Fn>() 这里Fn作为类型class<T>, 后面()相当使用默认构造函数创建对象；declval的优点在于不需要构造函数也可以创建对象；
		 *		这里传入CallbackFunc, 那么CallbackFunc也是一种类型；declval<Fn>() ==> Tn()==> object;
		 *	3. declval<Person>() => 同样道理，即使Person没有构造函数，也可同样是创建对象的意思，这在编译时会替换为Person& person;
		 *	4. declval<Fn>( declval<Person>() )  将Person对象做参数， 传入Fn(Person&) 等于后面的lambda => [&](Person& p){...};
		 */

		 // template 编程时核心思路需要改变， template<class ...> 定义函数时其实需要行明白调用模板函数后编译器的动作；

		multimap<KeyTy, Person> rst;
		for_each(persons.begin(), persons.end(), [&rst, &key_selector_fn](const Person& person) {
			rst.insert(make_pair(key_selector_fn(person), person));
			});
		return rst;
	}

	template <typename KeyFn, typename ValueFn>
	auto GroupBy4(const vector<Person>& persons, const KeyFn& key_fn, const ValueFn& value_fn)
	{
		//typedef result_of_t<KeyFn(Person)> KeyTy;
		typedef typename result_of<KeyFn(Person)>::type KeyTy; // result_of<...> 简化后就比较直观
		using ValTy = typename result_of<ValueFn(Person)>::type;

		map<KeyTy, ValTy> rst;
		for_each(begin(persons), end(persons), [&](const Person& person) {
			rst.insert(make_pair(key_fn(person), value_fn(person)));
			});

		return rst;
	}

	const vector<Person> kUsers = {
		{"name1", 20, "ks"}, {"name2", 22, "sz"},
		{"name3", 22, "ks"}, {"name4", 20, "sz"}
	};

	void Grouping01()
	{
		const multimap<int, Person> rst_age =
			GroupBy1<int, function<int(const Person&)>>(
				kUsers, [](const Person& person) { return person.age; });

		const multimap<string, Person> rst_city =
			GroupBy1<string, function<string(const Person&)>>(
				kUsers, [](const Person& person) { return person.city; });

		for (auto it = rst_age.cbegin(); it != rst_age.cend(); ++it) {
			std::cout << "key: " << it->first << ", value: " << it->second.name << "," << it->second.city << "\n";
		}

		auto rst_name = GroupBy2(kUsers, [](const Person& person) { return person.name; });
		for (auto it = rst_age.cbegin(); it != rst_age.cend(); ++it) {
			std::cout << "key: " << it->first << ", value: " << it->second.name << "," << it->second.city << "\n";
		}

		auto result = GroupBy3(kUsers, [](const Person& person) {
			return person.age;
			});

		for (auto p : result) {
			cout << p.first << ": " << p.second.name << ", " << p.second.city << "\n";
		}

		auto result2 = GroupBy4(kUsers,
			[](const Person& person) { return person.age; },
			[](const Person& person) { return person.name; });

		for (auto p : result2) {
			std::cout << "key: " << p.first << ", value: " << p.second << "\n";
		}
	}

	//--------------------------------------------------------------------------------------------//

	/*
	 *  #include <type_traits> 包含大量类型相关的工具
	 *		1. 类型判断工具:	is_void<T>, is_integral<T>, is_floating_point<T>, is_enum<T>, is_array<T>, is_function<T>, is_pointer<T>, is_class<T>, is_const<T>...
	 *		2. 类型检验关系:	is_same<T1, T2>, is_base_of<T, D>, is_convertible<T1, T2>, is_assignable<T1, T2>, uses_allocator<T, Alloc>
	 *		3. 类型修饰符:	add_const<T>::type, add_lvalue_reference<T>, add_rvalue_reference<T>, add_pointer<T>::type,... remove_const<T>, remove_reference<T>...
	 *		4. 特殊查询:		rank<T>, extent<T, I=0>, remove_extent<T>, decay<T>, enable_if<B, T=void>, conditional<B,T,F>, alignment_of<T>
	 *	type_traits使用方法与其它普通函数调用方法不同， type_traits泛化非常彻底， 常用成员变量有 ::value, ::type
	 *		typedef std::add_pointer<decltype(i)>::type IntPtr;	//i 是int，定义了IntPtr，即int pointer类型，
	 *		std::cout << std::boolalpha << std::is_pointer<IntPtr>::value << '\n';
	 *		std::cout << std::boolalpha << std::is_same<IntPtr, int*>::value << '\n';
	 *		using IntAgain = std::remove_pointer<IntPtr>::type;	// IntAgain即int;
	 *
	 */

	 /* std::reference_wrapper? 引用包装器, 主要与模板结合,通常使用std::ref(value), std::cref(value) 来创建reference_wrapper对象。
	 /	reference_wrapper在STL标准库使用非常多，如： make_pair() 就是创建pair<> of reference, make_tuple()创建tuple<>的reference,
	 /	通常使用std::ref, std::cref创建reference_wrapper对象，但在定义时必须使用reference_wrapper如：
	 /		vector<MyClass&> vec_my; /// 创建vec,存入MyClass对象引用，这样定义不能通过编译， 必须使用如下格式：
	 /		vector<std::reference_wrapper<MyClass>> vec_my;   // 变量声明和定义时必须使用reference_wrapper<T>
	 /		MyClass obj1; vec_my.push_back(std::ref(obj1)); vec_my.emplace_back(std::ref<MyClass>(obj2));
	 */

	void RefWrapperDemo()
	{
		std::list<int> l(10);
		std::iota(l.begin(), l.end(), 44);

		std::vector<std::reference_wrapper<int>> v(l.begin(), l.end());
		std::shuffle(v.begin(), v.end(), std::mt19937{ std::random_device{}() });
	}

	/*
	 * Function Type Wrapper(函数包装器), std::function<...>; 可以理解为把可调用对象callable_object(function, member_function, function_obj, lambda)当作最高级对象；
	 * std::function 同函数指针类似；
	 */

	void FuncObject()
	{
		std::vector<function<void(int, int)>> some_tasks; // vector保存函数对象(返回void, 参数为(int, int));
		typedef void(*FuncPtr)(int, int); // 定义函数指针类型
		std::vector<FuncPtr> some_jobs; // 函数指针做为vector， 即将函数对象做为vector成员
		// std::function<void(int, int)>, 当将类成员函数定义成调用对象时，类对象必须作第一参数
		class C
		{
		public:
			void MemberFunc(int x, int y) const {}
		};

		// const C& 相当于成员函数调用时的传递this；因为成员函数调用时实质进行了自动this传递，只是隐藏在传递中
		const std::function<void(const C&, int, int)> member_func = &C::MemberFunc;
		member_func(C(), 88, 88);

		// 成员函数指针，声明方式： return_val(Class::*func)(arg1, arg2...) = &Class::member_func;
		void (C::*mem_func)(int x, int y) const = &C::MemberFunc;
		C c1;
		(c1.*mem_func)(89, 99); //调用方式，涉及运算符优先级:  (obj.*func)(arg...);

		C* c2 = new C;
		(c2->*mem_func)(11, 22); // obj调用方式(obj->*func)(arg...); 总体还是使用std::function<void(const Class&, arg...)> 更优。
		delete c2;
	}
}

namespace stl_simple
{
	/*
	 * STL 组件:  Container(容器),  Iterator(迭代器),  Algorithm(算法), 仿函数(FunctionObject),  Adapter(适配器);
	 *
	 *	SequenceContainer(序列容器): 有序(Ordered)容器，每个元素都有确定位置,位置与插入容器的时机有关，与元素本身无关。 array, vector, deque, list, forward_list;
	 *	AssociativeContainer(关联容器): 已排序(Sorted)容器, 元素位置取决于value, 如果元素key/value pair,则位置取决于key,与插入顺序无关. set, multiset, map, multimap;
	 *	Unordered{associative}Container(无序容器): 无序集合，元素位置无关紧要，元素值与元素值都不会影响排序，而且它位的次序有可能随时间发生变化。
	 *		unordered_set, unordered_multiset, unordered_map, unordered_multimap;   STL含4个预定义无序容器；
	 *
	 *	Adapter(适配器):  Stack(采用LIFO,栈模式)， Queue(FIFO),  PriorityQueue; 对于开发者，它们和普通容器没有区别
	 *	Iterator，实质是一智能指针， 支持*ptr取值，++， --操作。
	 *
	 */
}

int main(int argc, char* argv[])
{
	//keywords_simple::no_explicit_test();
	keywords_simple::test_type_traits();
	keywords_simple::test_sfinae();
	keywords_simple::Grouping01();
}