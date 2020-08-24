#include <iostream>
#include <chrono>
#include <string>
#include <deque>
#include <vector>
#include <numeric>
#include <queue>

#include <thread>  // 程序创建管理主接口
#include <atomic>	// 细粒度的原子操作
#include <mutex>	// 线程独占式资源访问能力的互斥算法，保证共享资源的同步访问
#include <condition_variable> // 定量的线程等待被另一线程唤醒，然后继续执行。
#include <future>		// 异步任务返回值获取及相关异常处理。
#include <random>

/*
 * 并发： 同一时间段内交替处理多个操作，交替发生。(看起来同时)  相对应： 顺序
 * 并行： 同一时刻同时处理多个操作，强调的是同时刻同时发生。 相对应：串行。
 * 任务： 一系列共同达到某一目标的操作，可以是一个线程也可以是一个进程。
 * 进程： 资源分配的基本单位，独立拥有独立完备的数据空间和代码空间。系统创建一个进程并为它分配资源，放入进程队列，由进程调度程序处理。 进程是系统中并发执行的单位
 * 线程： 执行处理器调度的基本单位，一个进程由一个或多个线程构成，各线程共享代码和全局数据，但有其各自的堆栈。线程间共享资源更容易，因为系统分配给进程的资源对该进程的所有线程可用。
 * 线程与进程的处理方式与系统内核有关，Mac， Windows这类采用微内核的操作系统中，进程只是资源的分配单位，真正调度运行的是线程。
 * Linux宏内核对线程和进程并没有特别的区分，线程只不过是一种特殊的进程，多线程编程与多进程编程的主要区别在于有没有共享数据。多进程间的通信较复杂且代价较大，
 * C++ 并没有提供多进程通信的原生支持。
 */

// RAII ==> Resource Acquisition Is Initialization 即 资源获取即初始化，主要实现方式即采用智能指针，资源采用类的方式管理，
// 最典型的RAII使用如：  std::lock_guard<std::mutex>

// ReSharper disable CppUseAuto

namespace thread_sample
{
	// 函数的方式创建线程调用
	void ThreadFunc01(const int n)
	{
		const std::thread::id this_id = std::this_thread::get_id();
		for (int i = 0; i < 5; i++) {
			std::cout << "Child function thread:" << this_id << " running: " << i + 1 << "\n";
			std::this_thread::sleep_for(std::chrono::seconds(n));
		}
	}

	// 类方式创建线程调用，仿函数(重载operator())
	class ThreadClsFunc
	{
	public:
		void operator()(const int n) const
		{
			const std::thread::id this_id = std::this_thread::get_id();
			for (int i = 0; i < 5; i++) {
				std::cout << "Child cls func thread " << this_id << " running: " << i + 1 << "\n";
				std::this_thread::sleep_for(std::chrono::seconds(n));
			}
		}
	};

	void ThreadMain01()
	{
		std::thread thread01(ThreadFunc01, 1);

		if (thread01.joinable()) thread01.join();

		ThreadClsFunc cls_func01;

		std::thread thread02(cls_func01, 3);
		if (thread02.joinable()) thread02.detach();

		auto thread_lambda = [](const int n) {
			// lambda 同函数方式相同。
			const std::thread::id this_id = std::this_thread::get_id();
			for (int i = 0; i < 5; i++) {
				std::cout << "Child lambda thread " << this_id << " running:  " << i + 1 << "\n";
				std::this_thread::sleep_for(std::chrono::seconds(n));
			}
		};
		std::thread thread03(thread_lambda, 4);

		if (thread03.joinable()) thread03.join();

		const std::thread::id this_id = std::this_thread::get_id(); // 主线程id
		for (int i = 0; i < 5; i++) {
			std::cout << "main thread " << this_id << " running: " << i + i << "\n";
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}

		getchar();
	}

	void MutexDemo01()
	{
		std::chrono::milliseconds interval(100);
		std::mutex mutex;
		int job_shared = 0;
		int job_exclusive = 0;

		auto job01 = [&]() -> void {
			const auto this_id = std::this_thread::get_id();
			//std::lock_guard<std::mutex> lock_g(mutex); //智能锁lock_guard， 自动锁和释放；
			mutex.lock();
			std::this_thread::sleep_for(5 * interval);
			++job_shared;
			std::cout << "job01 shared (" << job_shared << "). thread_id:" << this_id << "\n";
			mutex.unlock();
		};

		auto job02 = [&]() -> void {
			const auto this_id = std::this_thread::get_id();
			while (true) {
				//std::unique_lock<std::mutex> u_lock(mutex, std::try_to_lock); // unique_lock 支持try_lock模式；后面可以不用unlock;
				if (mutex.try_lock()) {
					++job_shared;
					std::cout << "job02 shared (" << job_shared << "). thread_id: " << this_id << "\n";
					mutex.unlock();
					exit(0);
				}
				else {
					++job_exclusive;
					std::cout << "job02 exclusive (" << job_exclusive << "). thread_id:" << this_id << "\n";
					std::this_thread::sleep_for(interval);
				}
			}
		};

		const auto main_id = std::this_thread::get_id();

		std::cout << "main thread_id:  " << main_id << "\n";
		std::thread thread01(job01);
		std::thread thread02(job02);

		//std::this_thread::sleep_for(10 * interval);
		if (thread01.joinable()) thread01.join();
		if (thread02.joinable()) thread02.join();
		//thread01.detach();
		//thread02.detach();
	}

	/*
	 * 互斥(Mutual Exclusion)mutual 提供了普通锁lock/unlock,  智能锁lock_guard/unique_lock;
	 * lock_guard 严格高效，功能简单，不允许手动加锁和释放; 锁策略std::adopt_lock
	 * unique_lock 功能强大灵活，可以手动加锁和释放 锁策略： adopt_lock, defer_lock, try_to_lock;
	 * std::mutex 同一时间只可被一个线程锁定，任何其它lock()都会阻塞。
	 * std::recursive_mutex 允许同一时间多次被同一线程获得lock；
	 * std::timed_mutex 额外传递一个时间点或时间段，用来定义多长时间它可以尝试一个lock;它提供try_lock_for(duration) try_lock_until(time_point)
	 * std::recursive_timed_mutex;
	 */

	void ConditionMutex01()
	{
		std::deque<int> deque_int;
		std::mutex mutex01;

		const auto put_data = [&]() -> void {
			int count = 10;
			while (count > 0) {
				std::unique_lock<std::mutex> locker(mutex01);
				deque_int.push_front(count);
				std::cout << "put_data thread_id: " << std::this_thread::get_id() << "\n";
				locker.unlock();
				std::this_thread::sleep_for(std::chrono::seconds(1));
				count--;
			}
		};

		const auto get_data = [&]() -> void {
			int data = 0;
			while (data != 1) {
				std::unique_lock<std::mutex> locker(mutex01);
				if (!deque_int.empty()) {
					data = deque_int.back();
					deque_int.pop_back();
					locker.unlock();
					std::cout << "t1 got a value from t2:  " << data << "\n";
				}
				else {
					locker.unlock();
					std::this_thread::sleep_for(std::chrono::milliseconds(500));
					// 由于put_data生产者push数据时有1seconds的时间，在这时间段get_data会一直循环，判断到deque为空，也就会不停运行locker.unlock();
					// 产生的结果会导致CPU做了非常多的无用功，CPU使用率飙升，这里采用让线程暂停500 milliseconds方式，
					// 这种方式可以缓解CPU使用问题，但无条件判断下，让线程暂停多长时间要怎么确定呢。
					// 这就是后面方法采用condition_variable 方法的优势，push时lock(mutex), 互斥锁定deque, 完成数据push
					// push后locker.unlock(); 然后使用condition.notify_one()进行通知。
				}
			}
		};

		std::cout << "Main thread id: " << std::this_thread::get_id() << "\n";

		std::thread t1(put_data);
		std::thread t2(get_data);
		std::cout << "Main deque_int.back1():  " << deque_int.back() << "\n";
		//std::this_thread::sleep_for(std::chrono::seconds(5));
		t1.join();
		t2.join();
		std::cout << "Main deque_int.back2():  " << deque_int.back() << "\n";
	}

	void ConditionVarMutex02()
	{
		std::deque<int> deque_int;
		std::mutex mu;
		std::condition_variable cv;

		auto producer = [&]() {
			// 生产者，往队列存入数据
			int count = 10;
			while (count > 0) {
				std::unique_lock<std::mutex> locker(mu);
				deque_int.push_front(count);
				locker.unlock(); // 本线程unlock，customer会进行切换运行。
				cv.notify_one(); // 通知<条件满足>
				std::this_thread::sleep_for(std::chrono::seconds(1));
				count--;
			}
		};

		auto customer = [&]() {
			// 消费者，从队列提取数据
			int data = 0;
			while (data != 1) {
				std::unique_lock<std::mutex> locker(mu);
				while (deque_int.empty()) {
					cv.wait(locker);
				} // 采用while运行，condition.wait一直等到notify_one后才会走到后面。 这里不能使用if。相当于deque.empty条件下，线程一直休眠等待。
				//cv.wait(locker, [&]()-> bool { return !deque_int.empty(); }); //可使用condition.wait(locker, 条件lambda; 效果相同。

				data = deque_int.back();
				deque_int.pop_back();
				locker.unlock(); // 取得数据后，从这里切回到producer.
				std::cout << "T2 got a value from T1: " << data << "\n";
			}
		};

		std::cout << "Main thread id:  " << std::this_thread::get_id() << "\n";
		std::thread t1(producer);
		std::thread t2(customer);
		t1.join();
		t2.join();
		printf("Main thread end.\n");
	}

	typedef std::vector<int>::iterator VecIter;

	void AsyncUseCondition() // 使用condition_variable 实现异步；
	{
		using VecIter = std::vector<int>::iterator;
		int rst = 0;
		std::mutex mu;
		std::condition_variable cond;

		auto accumulate_lam = [&](const VecIter first, const VecIter last) -> void {
			const int sum = std::accumulate(first, last, 0);
			std::unique_lock<std::mutex> locker(mu);
			rst = sum;
			locker.unlock();
			cond.notify_one();
			std::this_thread::sleep_for(std::chrono::seconds(1));
		};

		std::vector<int> numbers = {1, 2, 3, 4, 5, 6};
		std::thread work_thread(accumulate_lam, numbers.begin(), numbers.end());
		std::unique_lock<std::mutex> main_locker(mu);

		cond.wait(main_locker, [&]() -> bool { return rst; }); // 从thread创建开始异步执行，使用condition_variable.wait等待结果返回。

		std::cout << "Result: " << rst << "\n";
		main_locker.unlock();
		work_thread.join();
	}

	/*
	 * 使用condition_variable可完成大部分异步任务，最大缺点则是数据耦合度太高，很多全局量需要引入到线程函数；
	 * 采用Future， promise, future 实现在数据松耦合；从下例可以看到， 使用promise<int> future<int> 可以将全部全局变量去除；
	 */
	void AsyncUseFuture()
	{
		class Accumulate
		{
		public:
			void operator()(const VecIter first, const VecIter last, std::promise<int> accumulate_promise) const
			{
				const int sum = std::accumulate(first, last, 0);
				accumulate_promise.set_value(sum); // 将返回值填入promise;
			}
		};

		Accumulate accumulate01;
		std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7};
		std::promise<int> accumulate_promise;
		std::future<int> accumulate_future = accumulate_promise.get_future();
		// 创建线程时，需要使用move(future)右值引用，通promise返回。
		std::thread work_thread(accumulate01, numbers.begin(), numbers.end(), std::move(accumulate_promise));

		accumulate_future.wait(); // 等待future返回；
		std::cout << "result: " << accumulate_future.get() << "\n";
		work_thread.join();
	}

	void AsyncUsePackagedTask()
	{
		class AccumulateCls
		{
		public:
			int operator()(const VecIter first, const VecIter last) const
			{
				const int sum = std::accumulate(first, last, 0);
				return sum;
			}
		};

		AccumulateCls accumulate_obj;
		std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8};

		std::packaged_task<int(VecIter, VecIter)> accumulate_task(accumulate_obj); // 同promise方式相似。

		std::future<int> accumulate_future = accumulate_task.get_future();
		std::thread work_thread(std::move(accumulate_task), numbers.begin(), numbers.end());
		accumulate_future.wait();
		std::cout << "Result: " << accumulate_future.get() << "\n";
		work_thread.join();
	}

	void AsyncUseAsync()
	{
		auto accumulate = [](const VecIter first, const VecIter last) -> int {
			const int sum = std::accumulate(first, last, 0);
			return sum;
		};

		std::vector<int> numbers{1, 2, 3, 4, 5, 6, 7, 8};
		//auto accumulate_future = std::async(std::launch::async, accumulate, numbers.begin(), numbers.end());
		std::future<int> accumulate_future = std::async(std::launch::async, accumulate, numbers.begin(), numbers.end());
		std::cout << "Rst: " << accumulate_future.get() << "\n";
	}

	void MultiThreadUseAtomic()
	{
		std::chrono::milliseconds interval(100);
		std::atomic<int> job_shared(0);
		std::atomic<bool> ready_flag(false);
		int job_exclusive = 0;

		auto job01 = [&]() -> void {
			const auto this_id = std::this_thread::get_id();
			std::this_thread::sleep_for(interval * 5);
			job_shared.fetch_add(1);
			std::cout << "job01 shared (" << job_shared.load() << "). " << "this_id: " << this_id << '\n';
			ready_flag.store(true);
		};

		auto job02 = [&]()-> void {
			while (true) {
				if (ready_flag.load()) {
					job_shared.fetch_add(1);
					std::cout << "job02 shared (" << job_shared.load() << "). \n";
					return;
				}
				else {
					++job_exclusive;
					std::cout << "job02 exclusive (" << job_exclusive << "). \n";
					std::this_thread::sleep_for(interval);
				}
			}
		};

		std::thread thread01(job01);
		std::thread thread02(job02);
		thread01.join();
		thread02.join();
	}
}

namespace thread_csdn
{
	void AdditionDemo01()
	{
		const auto func = [&](const double var) -> double {
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			return var;
		};

		std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now(); //单线程计时开始
		double rst = func(1.) + func(2.) + func(3.) + func(4.);
		std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
		double time_cost = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count() * 1000.;
		std::cout << std::fixed << "Single-Thread result: " << rst << ", and time cost: " << time_cost << "\n";

		t1 = std::chrono::steady_clock::now(); //4线程运算 计时开始
		std::future<double> f1(std::async(std::launch::async, func, 1.));
		auto f2 = std::async(std::launch::async, func, 2.);
		auto f3 = std::async(std::launch::async, func, 3.);
		rst = func(4.) + f1.get() + f2.get() + f3.get();
		t2 = std::chrono::steady_clock::now(); // 计时结束
		time_cost = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count() * 1000.;
		std::cout << std::fixed << "Multi-Thread result: " << rst << ", and time cost: " << time_cost;
		std::cout << std::endl;
	}

	void FrontBackEnd()
	{
		class BackEnd
		{
		public:
			BackEnd()
			{
				thread_ptr_ = std::make_unique<std::thread>(&BackEnd::Process, this);
			}

			~BackEnd()
			{
				if (this->thread_ptr_->joinable()) thread_ptr_->join();
			}

			BackEnd(const BackEnd&) = delete; // 禁止拷贝构造; 也可将此设置成private,效果相同
			BackEnd& operator=(const BackEnd&) = delete; // 禁卡拷贝赋值

			void Process()
			{
				thread_running_.store(true);
				while (thread_running_.load()) {
					int data;
					{
						// Wait for data;
						std::unique_lock<std::mutex> ul(data_queue_mutex_);
						cond_var_.wait(ul, [this] { return !data_queue_.empty(); });
						data = data_queue_.front();
						data_queue_.pop_front();
						ul.unlock();
					} // Release lock;
					std::cout << "[BackEnd]: Receive data: " << data << "\n";
					cond_var_.notify_one();
				}
			}

			void AddData(const int data)
			{
				{
					//std::lock_guard<std::mutex> lg(data_queue_mutex_);
					std::unique_lock<std::mutex> ul(this->data_queue_mutex_);
					//cond_var_.wait(ul, [this] { return !data_queue_.empty(); });
					while (!data_queue_.empty()) cond_var_.wait(ul);
					data_queue_.push_back(data);
					std::cout << "[FrontEnd]: Add Var " << data << "\n";
					ul.unlock();
				}
				cond_var_.notify_one();
			}

		private:
			std::atomic<bool> thread_running_;
			std::mutex data_queue_mutex_;
			std::deque<int> data_queue_;
			std::condition_variable cond_var_;
			std::unique_ptr<std::thread> thread_ptr_;
		};

		// Front end  ==> Main()
		{
			BackEnd back_end;
			std::default_random_engine generator;
			// ReSharper disable once CppLocalVariableMayBeConst
			std::uniform_int_distribution<int> distribution(0, 1000);
			for (int i = 0; i < 10; i++) {
				const int random_var = distribution(generator);
				back_end.AddData(random_var);
				std::this_thread::sleep_for(std::chrono::milliseconds(500));
			}
		}
		return;
	}

	template <typename Data>
	class ConCurrentQueue
	{
	public:
		void push(const Data& data)
		{
			std::unique_lock<std::mutex> ul(mutex_);
			data_queue_.push(data);
			ul.unlock();
			cond_var_.notify_one();
		}

		bool empty() const
		{
			std::lock_guard<std::mutex> lg(mutex_);
			return data_queue_.empty();
		}

		Data& front()
		{
			std::unique_lock<std::mutex> ul(this->mutex_);
			return data_queue_.front();
		}

		Data const& front() const
		{
			std::unique_lock<std::mutex> ul(mutex_);
			return data_queue_.front();
		}

		void pop()
		{
			std::unique_lock<std::mutex> ul(mutex_);
			data_queue_.pop();
		}

		void wait_pop(Data& popped_value)
		{
			std::unique_lock<std::mutex> ul(mutex_);
			while (data_queue_.empty()) {
				cond_var_.wait(ul);
			}
			popped_value = data_queue_.front();
			data_queue_.pop();
		}

		bool try_pop(Data& popped_value)
		{
			std::unique_lock<std::mutex> ul(mutex_);
			if (data_queue_.empty())return false;
			popped_value = data_queue_.front();
			data_queue_.pop();
			return true;
		}

	private:
		std::queue<Data> data_queue_;
		mutable std::mutex mutex_;
		std::condition_variable cond_var_;
	};

	template <typename Type>
	class BlockingStream
	{
	public:
		explicit BlockingStream(const size_t max_buff) : max_buffer_size_(max_buff)
		{
		}

		BlockingStream(BlockingStream const&) = delete;
		BlockingStream& operator=(const BlockingStream&) = delete;

		BlockingStream& operator<<(Type& other)
		{
			std::unique_lock<std::mutex> ul(mutex_);
			while (buffer_.size() >= max_buffer_size_)
				stop_if_full_.wait(ul);
			buffer_.push(std::move(other));
			ul.unlock();
			stop_if_empty_.notify_one();
			return *this;
		}

		BlockingStream& operator>>(Type& other)
		{
			std::unique_lock<std::mutex> ul(mutex_);
			stop_if_empty_.wait(ul, [this]()-> bool { return this->buffer_.empty(); });
			std::swap(other, buffer_.front());
			buffer_.pop();
			ul.unlock();
			stop_if_full_.notify_one();
			return *this;
		}

	private:
		size_t max_buffer_size_;
		std::queue<Type> buffer_;
		std::mutex mutex_;
		std::condition_variable stop_if_empty_, stop_if_full_;
		bool eof_ = false;
	};
}

namespace thread_github
{
	/*
	 *	Code 来源于book <<C++ Concurrency in Action 2ed>>
	 *	url:   https://downdemo.gitbook.io/cpp-concurrency-in-action-2ed/
	 *	https://github.com/downdemo/Cpp-Concurrency-in-Action-2ed
	 */

	void Func01()
	{
		std::cout << "Func01 first cout." << std::endl;
	}

	struct S01
	{
		// 重载operator() 实现仿函数
		void operator()() const
		{
			std::cout << "struct S01:  " << 1 << "\n";
		}
	};

	void TestMain01()
	{
		S01 s1;
		std::thread t1(s1);
		std::thread t2{S01()};
		std::thread t3((S01())); // 注意(()) 解决 most vexing parse ;
		std::thread t4([] { std::cout << "lambda call t4" << "\n"; });
		t1.join();
		t2.join();
		t3.join();
		t4.join();
	}

	struct S02
	{
		int& i;
		explicit S02(int& x) : i(x) { std::cout << "S2(int&) called.\n" << "\n"; }

		void operator()() const
		{
			for (int j = 0; j < 1000000; j++) {
				std::cout << "j:  " << j << ",  i: " << i << " ";
				// something... ;这里存在安全隐患，对象析构后i会成  空悬
			}
		}
	};

	void TestS02()
	{
		int x = 0;
		S02 s02{x};
		std::thread t1(s02); //调用仿函数operator()()const;
		t1.detach(); //主程序直接结束不等待
	}

	void TestS03()
	{
		int x = 0;
		S02 s02{x};
		std::thread t(s02);
		/*
			try{ 主线程操作， 如果发生异常，会导致线程没能执行join(); 故需要在catch(...){ 执行 t.join(); }；
		 同时也要保证在主线程没发生异常时执行catch(...)后续语句时，也需要join(); 这样的代码在结构、性能方面需要改进。
		 解决此类问题，后面的  ThreadGuard 比较优雅；
		*/
		try {
			printf("Here do some thing...\n");
			/* some coding...*/
		}
		catch (...) {
			t.join();
			throw;
		}
		t.join();
	}

	/*
	 * 创建线程守护类， 防止线程出现异常时不进行join操作，当然可以采用try...catch...操作，但结构性更差
	 */
	class ThreadGuard
	{
	public:
		explicit ThreadGuard(std::thread& t) : t_(t)
		{
		}

		~ThreadGuard() { if (t_.joinable()) t_.join(); }
		ThreadGuard(ThreadGuard const&) = delete;
		ThreadGuard& operator=(ThreadGuard const&) = delete;
	private:
		std::thread& t_;
	};

	void Func02(int& n) { ++n; }
	// std::thread 默认情况下不能传引用的，这里需要使用std::ref(...)将参数值传递变成引用传递；

	void ThreadGuardTest01()
	{
		auto x = 0;
		S02 s2{x};
		std::thread t2{s2};
		ThreadGuard tg2{t2};

		std::thread t1(Func02, std::ref(x)); // Func02 传递int&; 引用传递；
		ThreadGuard tg1(t1);

		std::unique_ptr<std::thread> t3 = std::make_unique<std::thread>(Func02, std::ref(x));
		auto func03 = [](const std::unique_ptr<int> p) -> void { (*p)++; };
		t3->join();

		std::unique_ptr<int> num{new int(8)};
		std::unique_ptr<std::thread> t4 = std::make_unique<std::thread>(func03, std::move(num));
		t4->join();

		/*
		 * some code....;
		 */
		// 这里使用类管理线程，函数结束时自动调用析构函数，实现自动线程join; 避免了try...catch(...){...}；
	}

	class ScopedThread
	{
	public:
		explicit ScopedThread(std::thread t) : t_(std::move(t))
		{
			if (t_.joinable()) {
				throw std::logic_error("no thread");
			}
		}

		~ScopedThread() { this->t_.join(); }

		ScopedThread(ScopedThread const&) = delete;
		ScopedThread& operator=(const ScopedThread&) = delete;

	private:
		std::thread t_;
	};

	void ScopedThreadTest()
	{
		// 直接将线程传入类中，由类对象进行自动管理线程
		ScopedThread st(std::thread{S01()});
	}

	/*
	 * 完备的线程管理类
	 */
	class ThreadMgr
	{
	public:
		ThreadMgr() noexcept = default;

		/*
		 * std::forward<T> 完美转发; 函数模板在向其它函数传递自身形参时，保留其自身左右值属性，
		 * 这里使用了可变参数模板(参数包)
		 */
		template <typename Func, typename... Ts>
		explicit ThreadMgr(Func&& f, Ts&&... args): t_(std::forward<Func>(f), std::forward<Ts>(args)...)
		{
		}

		explicit ThreadMgr(std::thread t) noexcept : t_(std::move(t))
		{
		}

		// 移动构造(右值引用)
		ThreadMgr(ThreadMgr&& rhs) noexcept: t_(std::move(rhs.t_))
		{
		}

		ThreadMgr& operator=(ThreadMgr&& rhs) noexcept
		{
			if (JoinAble()) Join();
			t_ = std::move(rhs.t_);
			return *this;
		}

		ThreadMgr& operator=(std::thread rhs) noexcept
		{
			if (JoinAble()) Join();
			t_ = std::move(rhs);
			return *this;
		}

		~ThreadMgr() noexcept
		{
			if (JoinAble()) Join();
		}

		void Swap(ThreadMgr&& rhs) noexcept { t_.swap(rhs.t_); }

		std::thread::id GetId() const noexcept { return this->t_.get_id(); }

		void Detach() { this->t_.detach(); }

		std::thread& AsThread() noexcept { return t_; }

		const std::thread& AsThread() const noexcept { return this->t_; }

		bool JoinAble() const noexcept { return t_.joinable(); }

		void Join() { t_.join(); }

		// 拷贝构造(左值引用)
		ThreadMgr(ThreadMgr const&) = delete;
		// 拷贝赋值(左值引用)
		ThreadMgr& operator=(ThreadMgr const&) = delete;

	private:
		std::thread t_;
	};

	class ClsArgs
	{
	public:
		ClsArgs() = default;

		ClsArgs(const int i = 0): num_(i)
		{
		}

		void func(const int i)
		{
			this->num_ = i;
			std::cout << i << "\n";
		}

		void func(int& i)
		{
			this->num_ = i;
			i++;
			std::cout << "num: " << this->num_ << ", i: " << i << "\n";
		}

	private:
		int num_;
	};
}

int main(int argc, char* argv[])
{
	//thread_sample::ThreadMain01();
	//thread_sample::MutexDemo01();
	//thread_sample::ConditionMutex01();
	//thread_sample::ConditionVarMutex02();
	//thread_sample::AsyncUseCondition();
	//thread_sample::AsyncUseFuture();
	//thread_sample::AsyncUsePackagedTask();
	//thread_csdn::AdditionDemo01();
	//thread_csdn::FrontBackEnd();
	thread_github::ThreadGuardTest01();
}
