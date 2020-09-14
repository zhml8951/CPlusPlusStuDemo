#include "thread_pool.h"
#include <iostream>
#include <memory>
#include <sstream>
#include <cassert>
#include <mutex>
#include <condition_variable>
#include <deque>


namespace pool
{
	using std::cout;
	std::unique_ptr<ILogger> active_logger = nullptr;

	static const char kBlack[] = {0x1b, '[', '1', ';', '3', '0', 'm', 0};
	static const char kRed[] = {0x1b, '[', '1', ';', '3', '1', 'm', 0};
	static const char kYellow[] = {0x1b, '[', '1', ';', '3', '3', 'm', 0};
	static const char kBlue[] = {0x1b, '[', '1', ';', '3', '4', 'm', 0};
	static const char kNormal[] = {0x1b, '[', '1', ';', '3', '9', 'm', 0};

	Logger::Logger(const LogLevel level) : log_level_(level) { }

	void Logger::Debug(const string& msg, const string& file, const size_t line)
	{
		if (this->log_level_ >= LogLevel::kDebug)
			this->OutInfo(msg, file, line, LogLevel::kDebug);
	}

	void Logger::Info(const string& msg, const string& file, size_t line)
	{
		if (this->log_level_ >= LogLevel::kInfo) {
			OutInfo(msg, file, line, LogLevel::kInfo);
		}
	}

	void Logger::Warn(const string& msg, const string& file, size_t line)
	{
		if (this->log_level_ >= LogLevel::kWarn)
			OutInfo(msg, file, line, LogLevel::kWarn);
	}

	void Logger::Error(const string& msg, const string& file, size_t line)
	{
		if (this->log_level_ >= LogLevel::kError)
			OutInfo(msg, file, line, LogLevel::kError);
	}

	void Logger::OutInfo(const string& msg, const string& file, const size_t line, const LogLevel level)
	{
		const char* color;
		const char* info;
		switch (level) {
		case LogLevel::kDebug:
			color = kBlack;
			info = "DEBUG";
			break;
		case LogLevel::kInfo:
			color = kBlue;
			info = "INFO";
			break;
		case LogLevel::kWarn:
			color = kYellow;
			info = "WARN";
			break;
		case LogLevel::kError:
			color = kRed;
			info = "ERROR";
			break;
		default:
			info = "DEFAULT";
			color = kNormal;
		}

		std::lock_guard<std::mutex> lock(this->mutex_);
		cout << "[" << color << info << kNormal << "][thread_pool][" << file << ":" << line << "]" << msg << "\n";
	}

	void Debug(const string& msg, const string& file, const size_t line)
	{
		if (active_logger)
			active_logger->Debug(msg, file, line);
	}

	void Info(const string& msg, const string& file, size_t line)
	{
		if (active_logger)
			active_logger->Info(msg, file, line);
	}

	void Warn(const string& msg, const string& file, size_t line)
	{
		if (active_logger)
			active_logger->Warn(msg, file, line);
	}

	void Error(const string& msg, const string& file, size_t line)
	{
		if (active_logger)
			active_logger->Error(msg, file, line);
	}

	static std::string GetId()
	{
		std::stringstream ss;
		ss << std::this_thread::get_id();
		return ss.str();
	}

	ThreadPool::ThreadPool(const int init_size) : init_threads_size_(init_size), mutex_(), cond_var_(),
	                                              is_started_(false)
	{
		Start();
	}

	ThreadPool::~ThreadPool() { }

	void ThreadPool::Start()
	{
		assert(threads_.empty(), "Threads empty.");
		is_started_ = true;
		threads_.reserve(init_threads_size_);
		for (auto i = 0; i < init_threads_size_; ++i) {
			threads_.push_back(new std::thread(std::bind(&ThreadPool::ThreadLoop, this)));
		}
	}

	bool ThreadPool::IsStarted() const { return this->is_started_; }

	void ThreadPool::ThreadLoop()
	{
		LOGGING(Debug, "ThreadLoop() tid: " + GetId() + " start. ");
		while (is_started_) {
			TaskFunc task = Take();
		}
	}

	auto ThreadPool::Take() -> ThreadPool::TaskFunc
	{
		std::unique_lock<std::mutex> lock(this->mutex_);
		while(tasks_.empty() && is_started_) {
			LOGGING(Debug, "ThreadPool::Take() tid: " + GetId() + " wait. ");
			cond_var_.wait(lock);
		}
		LOGGING(Debug, "ThreadPool::Take() tid: " + GetId() + " wakeup. ");
		TaskFunc task;
	}
}

/*
 *
 * 	private:
		int init_threads_size_;
		ThreadsVec threads_;
		TasksDeque tasks_;
		std::mutex mutex_;
		std::condition_variable cond_var_;
		bool is_started_;
 */
