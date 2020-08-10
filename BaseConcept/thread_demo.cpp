﻿#include <iostream>
#include <chrono>
#include <string>
#include <deque>
#include <vector>
#include <numeric>

#include <thread>  // 程序创建管理主接口
#include <atomic>	// 细粒度的原子操作
#include <mutex>	// 线程独占式资源访问能力的互斥算法，保证共享资源的同步访问
#include <condition_variable> // 定量的线程等待被另一线程唤醒，然后继续执行。
#include <future>		// 异步任务返回值获取及相关异常处理。

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

	// 类方式创建线程调用，伪函数(重载operator())
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
				while (deque_int.empty()) cv.wait(locker);
				// 采用while运行，condition.wait一直等到notify_one后才会走到后面。 这里不能使用if。相当于deque.empty条件下，线程一直休眠等待。

				//condition.wait(locker, [&]()-> bool { return !deque_int.empty(); }); //可使用condition.wait(locker, 条件lambda; 效果相同。
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

	//
	void AsyncUseCondition()
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

		std::vector<int> numbers = { 1, 2, 3, 4, 5, 6 };
		std::thread work_thread(accumulate_lam, numbers.begin(), numbers.end());
		std::unique_lock<std::mutex> main_locker(mu);
		cond.wait(main_locker, [&]() { return rst; });
		std::cout << "Result: " << rst << "\n";
		main_locker.unlock();
		work_thread.join();
	}

	void AsyncUseFuture()
	{
		class Accumulate
		{
		public:
			void operator()(const VecIter first, const VecIter last, std::promise<int> accumulate_promise) const
			{
				const int sum = std::accumulate(first, last, 0);
				accumulate_promise.set_value(sum);
			}
		};

		Accumulate accumulate01;
		std::vector<int> numbers = { 1, 2, 3, 4, 5, 6, 7 };
		std::promise<int> accumulate_promise;
		std::future<int> accumulate_future = accumulate_promise.get_future();
		std::thread work_thread(accumulate01, numbers.begin(), numbers.end(), std::move(accumulate_promise));
		accumulate_future.wait();
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
		std::vector<int> numbers = { 1, 2, 3, 4, 5, 6, 7, 8 };
		std::packaged_task<int(VecIter, VecIter)> accumulate_task(accumulate_obj);
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

		std::vector<int> numbers{ 1, 2, 3, 4, 5, 6, 7, 8 };
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
			auto this_id = std::this_thread::get_id();
			std::this_thread::sleep_for(interval * 5);
			job_shared.fetch_add(1);
			std::cout << "job01 shared (" << job_shared.load() << "). \n";
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
		
		std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
		double rst = func(1.) + func(2.) + func(3.) + func(4.);
		std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
		double time_cost = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count() * 1000.;
		std::cout << std::fixed << "Single-Thread result: " << rst << ", and time cost: " << time_cost << "\n";

		t1 = std::chrono::steady_clock::now();
		std::future<double> f1(std::async(std::launch::async, func, 1.));
		auto f2 = std::async(std::launch::async, func, 2.);
		auto f3 = std::async(std::launch::async, func, 3.);
		rst = func(4.) + f1.get() + f2.get() + f3.get();
		t2 = std::chrono::steady_clock::now();
		time_cost = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count() * 1000.;
		std::cout << std::fixed << "Multi-Thread result: " << rst << ", and time cost: " << time_cost;
		std::cout << std::endl;
	}
}

int main(int argc, char* argv[])
{
	//thread_sample::ThreadMain01();
	//thread_sample::MutexDemo01();
	//thread_sample::ConditionMutex01();
	//thread_sample::ConditionVarMutex02();
	//thread_sample::AsyncUseCondition();
	thread_sample::AsyncUseFuture();
	thread_sample::AsyncUsePackagedTask();
	thread_csdn::AdditionDemo01();
}