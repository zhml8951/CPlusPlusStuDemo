#pragma once
#include <string>
#include <vector>
#include <deque>
#include <mutex>

#ifdef LOGGING_ENABLED
#define LOGGING(level, msg) pool::level(msg, __FILE__, __LINE__)
#else
#define LOGGING(level, msg)
#endif

namespace pool
{
	using std::string;

	class ILogger
	{
	public:
		ILogger(void) = default;
		virtual ~ILogger(void) = default;
		ILogger(const ILogger&) = default;
		ILogger& operator=(const ILogger&) = default;

		//
		virtual void Debug(const string& msg, const string& file, size_t line) = 0;

		virtual void Info(const string& msg, const string& file, size_t line) = 0;

		virtual void Warn(const string& msg, const string& file, size_t line) = 0;

		virtual void Error(const string& msg, const string& file, size_t line) = 0;
	};

	class Logger final : public ILogger
	{
	public:
		enum class LogLevel
		{
			kError = 0,
			kWarn = 1,
			kInfo = 2,
			kDebug = 3
		};

		explicit Logger(LogLevel level = LogLevel::kInfo);

		~Logger() = default;

		Logger(const Logger&) = default;

		Logger& operator=(const Logger&) = default;

		void Debug(const string& msg, const string& file, size_t line) override;

		void Info(const string& msg, const string& file, size_t line) override;

		void Warn(const string& msg, const string& file, size_t line) override;

		void Error(const string& msg, const string& file, size_t line) override;

	private:
		LogLevel log_level_;
		std::mutex mutex_;

	private:
		void OutInfo(const string& msg, const string& file, size_t line, LogLevel level);
	};

	extern std::unique_ptr<ILogger> active_logger;

	void Debug(const string& msg, const string& file, size_t line);

	void Info(const string& msg, const string& file, size_t line);

	void Warn(const string& msg, const string& file, size_t line);

	void Error(const string& msg, const string& file, size_t line);

	class ThreadPool
	{
	public:
		typedef std::function<void()> TaskFunc;
		explicit ThreadPool(int init_size = 3);
		~ThreadPool();
		ThreadPool(const ThreadPool&) = delete;
		const ThreadPool& operator=(const ThreadPool&) = delete;

		void Stop();
		void AddTask(const TaskFunc&);

	private:
		typedef std::vector<std::thread*> ThreadsVec;
		typedef std::deque<TaskFunc> TasksDeque;
		void Start();
		bool IsStarted() const;
		void ThreadLoop();
		TaskFunc Take();

	private:
		int init_threads_size_;
		ThreadsVec threads_;
		TasksDeque tasks_;
		std::mutex mutex_;
		std::condition_variable cond_var_;
		bool is_started_;
	};
}
