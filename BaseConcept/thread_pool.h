#pragma once
#include <string>
#include <vector>
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
		virtual void Debug(const string& msg, string& file, size_t line) = 0;

		virtual void Info(const string& msg, const string& file, size_t line) = 0;

		virtual void Warn(const string& msg, const string& file, size_t line) = 0;

		virtual void Error(const string& msg, const string& file, size_t line) = 0;

	};

	class Logger:public ILogger
	{
	public:
		enum class LogLevel
		{
			kError = 0,
			kWarn = 1,
			kInfo = 2,
			kDebug=3
		};

		Logger(LogLevel level = LogLevel::kInfo);

		~Logger() = default;

		Logger(const Logger&) = default;

		Logger& operator=(const Logger& ) = default;

		void Debug(const string& msg, string& file, size_t line) override;

		void Info(const string& msg, const string& file, size_t line) override;

		void Warn(const string& msg, const string& file, size_t line) override;

		void Error(const string& msg, const string& file, size_t line) override;


	private:
		LogLevel log_level_;
		std::mutex mutex_;
	};

	extern std::unique_ptr<ILogger> active_logger;
}
