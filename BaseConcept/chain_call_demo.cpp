#include <iostream>
#include <functional>
#include <string>
#include <thread>
#include <future>
#include <chrono>

namespace chain_simple
{
	/*
	 *	这里采用3种形式定义模板类Task, 其本质是一样的. 而在理解 Task<R(Args...)>时花了很长时间，经过反复验证，这些都没有关系.
	 *		Demo1:	(常见样式, 主要好处在于Args...有多个类型时比较容易区分, 如: Task<int(int, int)> t2)
	 *			template<typename R, typename... Args>
	 *			class Task<R(Args...)>{.....};
	 *		Demo2:  (开始理解R(Args...) 为函数指针, 验证std::function<>方式进行偏特化)
	 *			template<typename R, typename... Args>
	 *			class Chain<std::function<R(Args...)> {......};
	 *		Demo3: (理解了<R(Args...)> 是个怎么回事, 重新采用普通模板类重新定义)
	 *			template<typename R, typename... Args>
	 *			class Task2{......};
	 *	从这些定义可以看出, 这里的定义只对内部递归调用时创建Task对象时格式有影响，总体来说Demo3方式最简单，但如果多参数时，这些参数直接列出
	 *	没能很好的区分。 采用Demo1时， then的返回值和实际return语句难以理解。
	 *
	 *	链式调用的实质是多个函数按照前一函数输出作为后一函数的输入这样组合串起来，然后推迟至某个需要的时刻再进行计算.
	 *	通常使用这样调用没有什么实质意义, 实现较复杂. 在一些延迟计算的场景下有这方面的需求;
	 *	链式调用的实现关键点： std::function<R(Args...)> fn, 也就是函数做为参数Callback;
	 *		任务整体以Task对象传递, 而Task对象中关键成员变量function<R(Args...)>,
	 *		整个链式调用过程,并没有真正的计算,只在调用Run(...)时，返回fn(...)并调用。
	 *
	 *		初始化任务时： Task(function<R(Args...)>) 即返回值R，参数Args...; 这里的R将是 第1个Then函数的输入
	 *		Then函数的关键点在于返回值: Task<typename std::result_of<F(R)>::type(Args...)> 也就是Task对象;
	 *		Then函数函数再次返回Task对象，对象内部将前一函数包括在内部，形成连环调用：
	 *			return Task<RstType(Args...)>([func, &f](Args&&... args){
	 *				return f(func(std::forward<Args>(args...));
	 *			});
	*		最后形成的整体式:	auto result=then3(then2(then1(task(88)))); 运行过程task(88)->then1->...
	 *
	 *	链式调用在js使用非常多，但C++中不容易使用；但提高程序性能方面却
	 *	总体原则:  不论参数类型如何变, 后一函数的输入参数类型是前一函数的返回值类型, 故lambda表达式里的参数类型可设置为auto,
	 *		但具体类型还是由前一函数返回值推导出来。
	 */
	template <typename T>
	class Task;

	template <typename R, typename ... Args>
	class Task<R(Args ...)> // 常用一种对参数包模板类的定义, 将Args...用()起来,如果多参数时,比较好区分;
	{
	public:
		explicit Task(std::function<R(Args ...)>&& fn) : fn_(std::move(fn)) {}
		explicit Task(std::function<R(Args ...)>& fn) : fn_(fn) {}

		R Run(Args&&... args)
		{
			return fn_(std::forward<Args>(args)...);
		}

		template <typename F>
		auto Then(F&& f) -> Task<typename std::result_of<F(R)>::type(Args ...)>
		{
			using RstType = typename std::result_of<F(R)>::type;
			auto func = std::move(fn_);

			return Task<RstType(Args ...)>([func, &f](Args&&... args) {
				return f(func(std::forward<Args>(args)...));
			});
		}

	private:
		std::function<R(Args ...)> fn_;
	};

	void task_test()
	{
		Task<int(int)> task([](const int i) { return i; });

		auto f = task.Then([](const int i) { return i + 1; })
		             .Then([](const int i) { return i + 2; })
		             .Then([](const int i) { return i + 3; });

		const auto result = f.Run(3);
		std::cout << "run task result: " << result << "\n";

		Task<double(double, double)> task2([](const double x, const double y)-> double {
			return x + y;
		});
		// 1. 创建task, return double类型
		auto proc =
			task2.Then([](const double x) -> auto {
				// 2. Then1, 接收tas返回值double作为参数类型， return tuple<string, double>;
				return std::make_tuple(std::to_string(x), x + 100);
			}).Then([](std::tuple<std::string, double> tuple) {
				// 3. Then2, 接收Then1返回类型tuple<string, double>; 返回类型pair<string, double>
				return std::make_pair(std::get<0>(tuple), std::get<1>(tuple));
			}).Then([](std::pair<std::string, double> const p) {
				// 4. Then3, 接收Then2返回类型pair<string, double>; 返回值类型initializer_list<double>;
				const std::initializer_list<double> rst{p.second, p.second + 1.1, p.second + 2.2};
				return rst;
			}).Then([](std::initializer_list<double> numbers) {
				// 5. Then4, 接收Then3返回值initializer_list<double>; 返回值double;
				auto rst = 0.0;
				for (auto num : numbers) {
					rst += num;
				}
				return rst;
			});
		const auto rst = proc.Run(11.0, 22.0);
		std::cout << "Result: " << rst << "\n";
	}

	template <typename R, typename... Args>
	class Task2
	{
	public:
		explicit Task2(std::function<R(Args ...)>&& fn) : fn_(std::move(fn))
		{
			std::cout << "Task2 move construct init. " << "\n";
		}

		explicit Task2(std::function<R(Args ...)>& fn) : fn_(fn)
		{
			std::cout << "Task2 construct init. " << "\n";
		}

		R run(Args&&... args)
		{
			return this->fn_(std::forward<Args>(args)...);
		}

		template <typename F>
		auto then(F&& f) -> Task2<typename std::result_of<F(R)>::type, Args...>
		{
			using RstType = typename std::result_of<F(R)>::type;
			auto func = std::move(this->fn_);
			return Task2<RstType, Args...>([func, &f](Args&&... args) {
				return f(func(std::forward<Args>(args)...));
			});
		}

	private:
		std::function<R(Args ...)> fn_;
	};

	void task2_test()
	{
		Task2<int, int> t2([](int const i)-> int {
			std::cout << "t2 task init; " << "\n";
			return i + 1;
		});

		auto ts = t2.then([](const int i) { return i + 1; })
		            .then([](const int i) { return i + 2; })
		            .then([](const int i) { return i + 3; })
		            .then([](const int i) { return i + 4; });

		const auto rst = ts.run(1);
		std::cout << "chain called result:  " << rst << "\n";

		Task2<int, int, int, int> t3([](const int a, const int b, const int c)-> int {
			std::cout << "t3(int,int,int)->int" << "\n";
			return a + b + c;
		});
	}

	template <typename T>
	class Chain;

	template <typename R, typename... Args>
	class Chain<std::function<R(Args ...)>>
	{
	public:
		explicit Chain(std::function<R(Args ...)>&& fun) : fun_(std::move(fun))
		{
			std::cout << "move construct fun name: " << __func__ << "\n";
		}

		explicit Chain(std::function<R(Args ...)>& fun) : fun_(fun)
		{
			std::cout << "construct fun name: " << __func__ << "\n";
		}

		R run(Args&&... args)
		{
			std::cout << "exec function. func name: " << __func__ << "\n";
			return this->fun_(std::forward<Args>(args)...);
		}

		/*
		 *	Chain偏特化采用 Chain<std::function<R(Args...)>的形式，故 then 函数的返回值类型：
		 *		auto then(F&& f) -> Chain<typename std::result_of<F(R)>::type(Args...)> 必须与偏特化形式相同:
		 *		auto then(F&& f) -> Chain<std::function<typename std::result_of<F(R)>::type(Args...)>>
		 *	从这些样式可以看出,前面进行偏特化定义时,如何设置都可以, 主要是这里做递归时的形式相同即可;
		 *	后面 采用 class Task2<R, Args...>也可以。详细查看 class Task2;
		 */

		template <typename F>
		auto then(F&& f) -> Chain<std::function<typename std::result_of<F(R)>::type(Args ...)>>
		{
			/*		//-> Chain<typename std::result_of<F(R)>::type(Args ...)>
			 *	-> Chain<typename std::result_of<F(R)>::type (Args...)> 这里理解花了不少时间
			 *		总体结果要满足模板类偏特化要求即:  Chain<std::function<R(Args...)>>
			 *		std::result_of<F(R)>::type() = std::function<R()> + (Args...)
			 *		结合Args... 即构成类偏特化class Chain<std::function<R(Args...)> 即: <R(Args...)>
			 */

			using RstType = typename std::result_of<F(R)>::type;
			auto func = std::move(this->fun_);

			return Chain<std::function<RstType(Args ...)>>([func, &f](Args&&... args) {
				return f(func(std::forward<Args>(args)...));
			});
		}

	private:
		std::function<R(Args ...)> fun_;
	};

	void chain_test()
	{
		Chain<std::function<int(int)>> chain([](int const i) -> int {
			std::cout << "chain01 init: " << i << "\n";
			return i;
		});

		std::string str = "join the lambda then: ";
		auto res =
			chain.then([&str](const int i) {
				str += "lambda_1\t";
				std::cout << "first: " << i << "\n";
				return i + 1;
			}).then([&str](const int i) {
				str += "lambda_2\t";
				std::cout << "second: " << i << "\n";
				return i + 2;
			}).then([&str](const int i) {
				str += "lambda_3\t";
				std::cout << "third: " << i << "\n";
				return std::make_tuple(i + 3, "n+3");
			}).then([&str](auto n) {
				// auto实质为前一个then的返回值类型即:std::tuple<int, string>;
				str += "lambda_4\t";
				std::cout << "fourth: " << std::get<0>(n) << ", string: " << std::get<1>(n) << "\n";
				return std::make_tuple(std::get<0>(n) + 4, "n+4");
			}).run(1);

		std::cout << "result value: " << std::get<0>(res)
			<< ", result string: " << std::get<1>(res) << "\n"
			<< "str_result:  " << str << "\n";
	}

	/*
	 *	链式调用过程使用异步async, 这里难点在于程序的运行顺序. 
	 *		链式调用可使程序将所有计算推迟到需要计算的时候进行, 而异步可以使主线程不用等待子函数完成即可进行后面运行. 
	 *		但在链式调用时,作用不大. 实际项目不太需要这么使用. 
	 *	这里仅供参考, 不推荐使用
	 */

	auto FuAsync(int i)
	{
		auto f = std::async([](const int n) {
			std::cout << "Sleep [" << n << "] seconds..." << std::endl;
			const std::chrono::milliseconds dre(n * 1000);
			std::this_thread::sleep_for(dre);
			return n * n;
		}, i);
		return std::forward<decltype(f)>(f);
	}

	void chain_future_test()
	{
		std::cout << "Chain test future func thread: " << std::this_thread::get_id() << std::endl;
		typedef std::future<int> FutureInt;
		typedef std::result_of<decltype(FuAsync)&(int)>::type FuType;
		static_assert(std::is_same<FutureInt, FuType>::value, "type not equal.");

		Task<FuType(int)> task(FuAsync);

		auto t1 = task.Then([](FuType n) {
			std::cout << "Then1 wait value..." << "\n";
			std::cout << "At first then func thread: " << std::this_thread::get_id() << std::endl;
			auto i_tmp = n.get() + 100;
			auto f = std::async([](const int i) {
				std::cout << "Number new value: " << i << " .." << "\n";
				std::cout << "Then1 async in thread: " << std::this_thread::get_id() << "\n";
				const std::chrono::milliseconds dre(1 * 1000);
				std::this_thread::sleep_for(dre);
				return i * i;
			}, i_tmp);
			return std::forward<decltype(f)>(f);
		}).Then([](FuType n) {
			std::cout << "Then2 wait value... \n";
			std::cout << "At second then func thread: " << std::this_thread::get_id() << "\n";
			std::chrono::milliseconds dre(1000);
			std::this_thread::sleep_for(dre);
			std::cout << "wait last value...,,,,,,,," << "\n";
			return n.get() + 1000;
		}).Then([](int n) {
			std::cout << "Then3 out lambda func" << "\n";
			auto fu = std::async([](int const i) {
				std::cout << "In lambda wait value...\n";
				std::cout << "At third then func thread: " << std::this_thread::get_id() << "\n";
				auto num = 2134;
				for (auto j = 0; j < i; j++) {
					num += (j % 3 == 0 && j > num * 1.5) ? 1 : 2;
				}
				std::cout << "[" << num << "]\n";
				return num;
			}, n);
			return std::forward<decltype(fu)>(fu);
		});

		std::cout << "[Future get]: \n" << t1.Run(2).get() << "\n";
	}
}

int main(int argc, char* argv[])
{
	//chain_simple::task_test();
	//chain_simple::task2_test();
	//chain_simple::chain_test();
	chain_simple::chain_future_test();
}
