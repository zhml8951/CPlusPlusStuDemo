#include <chrono>
#include <iostream>
#include <string>
#include <ctime>
#include <type_traits>

// chrono 是C++11引入的时间库，源于boost.
// 库的关键点： Durations(时间段)chrono::duration ，  TimerPointers(时间点) chrono::time_point， Clock(时钟)
// 时间转换可隐式向下转换，即hours -> minutes, minutes -> seconds.
// 向上转换必须显式转换 duration_cast<chrono::hours> minutes_i ..

using namespace std;

// 简单复显标准库chrono的 Duration；  chrono第一关键点: Duration, 时间段
namespace chrono_duration
{
	void chrono_demo01()
	{
		// chrono 内置了基本常用时间单位，也可自定义时间
		using TSec = chrono::duration<int32_t>;
		using TMilli = chrono::duration<uint64_t, ratio<1, 1000>>;
		// 查看标准库也可以知道， 库内常用的时间单位也是这这样定义的， 例如：
		typedef chrono::duration<int64_t, ratio<1, 1>> TSecond; //标准库定义如下：
		using TSecondStd = chrono::duration<long long>; // ==> typedef chrono::duration<int64_t, ratio<1,1>> TSEC;
		typedef chrono::duration<int32_t, ratio<60>> TMinute;
		typedef chrono::duration<int32_t, ratio<3600, 1>> THour;
		typedef chrono::duration<int32_t, ratio<3600 * 24, 1>> TDay; // 1天有24*3600秒。86400
		typedef chrono::duration<int64_t, ratio<1, 10>> TDeci; // 10分之1秒
		using TCenti = chrono::duration<int64_t, ratio<1, 100>>; // 百分之一秒
		// 常用的比例在ratio已经定义好了， 可直接使用，
		using TMicro = chrono::duration<int64_t, micro>; // typedef ratio<1, 1000000> micro
	}

	template <typename Rep, typename Period = ratio<1>>
	class Duration
	{
	public:
		using period = Period;
		using rep = Rep;
		constexpr Duration() = default;

		template <class Rep2>
		constexpr explicit Duration(const Rep2& r)
		{
		}

		template <class Rep2, class Period2>
		explicit constexpr Duration(const Duration<Rep2, Period2>& d) // 概念<拷贝构造函数>
		{
		}

		Duration(const Duration&) = default;

		Duration& operator=(const Duration&) = default;

		constexpr rep count() const
		{
			rep a;
			return a;
		};

		static constexpr Duration zero()
		{
			return {};
		}

		static constexpr Duration min()
		{
			return {};
		}

		static constexpr Duration max()
		{
			return {};
		}

		~Duration() = default;

		template <typename ToDuration, typename Rep3, typename Period3>
		static constexpr ToDuration duration_cast(const Duration<Rep3, Period3>& d)
		{
			return {};
		}
	};

	void duration_transform()
	{
		constexpr uint64_t year = 365 * 24 * 60 * 60 + 5 * 60 * 60 + 49 * 60 + 12;
		using namespace std;
		using Shakes = chrono::duration<int64_t, ratio<1, 100000000>>;
		using Weeks = ::chrono::duration<int32_t, std::ratio<7 * 24 * 3600, 1>>;
		using Years = chrono::duration<int32_t, ratio<year, 1>>;

		using NanoCentury = chrono::duration<double, std::ratio<100 * year, 1000000000>>; // 每世纪多少纳秒， 这样计算是错误的，应该用下面方式；
		typedef chrono::duration<uint64_t, ratio<100 * year * 1000000000, 1>> NanoCentury2; // 每世纪纳秒

		typedef chrono::duration<int32_t, std::centi /* std::ratio<1, 100> */> Jiffies;

		const chrono::seconds sec(1);

		// 无精度损失转换，隐式自动转换
		std::cout << chrono::microseconds(sec).count() << "  micro_seconds \n"
			<< Shakes(sec).count() << "  Shakes. \n"
			<< Jiffies(sec).count() << "  Jiffies: \n";

		// 有精度损失转换， duration_cast<chrono::minutes>(sec) 也就是小单位向大单位转换

		std::cout << chrono::duration_cast<chrono::minutes>(sec).count() << " minutes. \n";
		std::cout << NanoCentury(3).count() << " NanoCentury(3).\n";
		typedef std::ratio<year * 100, 1000000000> RatioCenturyNano;

		std::cout << "ratio century nano: " << RatioCenturyNano::num << "/" << RatioCenturyNano::den << "  .\n";

		RatioCenturyNano::type ra01;
		std::cout << "ratio::type, ra01.num:  " << ra01.num << "/" << ra01.den << ", \n";
	}
}

// chrono 第二关键点 time_point;  时间点, 主要关联时钟：steady_clock, system_clock;

// chrono 第三关键点clock
//std::chrono::steady_clock, chrono::system_clock, chrono::high_resolution_clock

namespace clock_time_point
{
	class SystemClock
	{
	public:
		using Rep = long long;
		using Period = std::ratio_multiply<ratio<100, 1>, std::nano>;
		using Duration = std::chrono::duration<Rep, Period>;
		using TimePoint = std::chrono::time_point<std::chrono::system_clock>;
		static constexpr bool is_steady_ = false;

		static TimePoint now() noexcept
		{
			//return (TimePoint(Duration(_Xtime_get_ticks())));
			return (TimePoint(Duration(8000ll)));
		}

		static Rep to_time_t(const TimePoint& time) noexcept
		{
			return (Rep(time.time_since_epoch().count()));
		}

		static TimePoint from_time_t(const Rep tm) noexcept
		{
			//return (TimePoint(Duration(tm * _XTIME_NSECS_PER_TICK)));
			return {};
		}
	};

	template <typename Clock, typename Duration = typename Clock::duration>
	class TimePoint
	{
		//std::chrono::time_point<>
	public:
		using Rep = typename Duration::rep;
		using Period = typename Duration::period;

		constexpr TimePoint() = default;

		constexpr explicit TimePoint(const Duration& other) : my_dur_(other)
		{
		}

		// 这里 template<typename Duration2, typename=typename enable_if_t<is_convertible_v<...> 
		// typename = enable_if<is_conf
		// enable_if, enable_if_t 用于模板类型检查,使用比较复杂，暂时先简单理解再说。 

		template <typename Duration2, typename = enable_if_t<is_convertible<Duration2, Duration>::value>>
		explicit constexpr TimePoint(const TimePoint<Clock, Duration2>& tp) : my_dur_(tp.time_since_epoch())
		{
		}

		constexpr Duration time_since_epoch() const
		{
			return (my_dur_);
		}

		//static constexpr TimePoint (min)() noexcept
		//{
		//	return (TimePoint((Duration::min)()));
		//}

		static constexpr TimePoint min() noexcept
		{
			return (TimePoint((Duration::min)()));
		}

		static constexpr TimePoint (max)() noexcept
		{
			return (TimePoint((Duration::max())));
		}

		template <class ToDuration, class Clock2, class Duration2>
		static constexpr auto time_point_cast(const TimePoint<Clock2, Duration2>& t) -> TimePoint<Clock2, ToDuration>;

	private:
		Duration my_dur_{typename Duration::zero()};
	};

	// 模板函数定义放在声明外面，会显得非常复杂，这里重点是理解模板类的使用。
	template <typename Clock, typename Duration>
	template <class ToDuration, class Clock2, class Duration2>
	constexpr auto TimePoint<Clock, Duration>::time_point_cast(
		const TimePoint<Clock2, Duration2>& t) -> TimePoint<Clock2, ToDuration>
	{
		TimePoint<Clock2, ToDuration> res;
		return res;
	}

	// 这些内容来源于标准库chrono.h文件， 具体实现目前还没做。

	// time_point demo from csdn;
	using Ms = std::chrono::milliseconds;
	using Sec = std::chrono::seconds;
	template <class Duration2>
	using TimePoint2 = std::chrono::time_point<std::chrono::high_resolution_clock, Duration2>;

	inline void print_ms(const TimePoint2<Ms>& t)
	{
		std::cout << t.time_since_epoch().count() << "ms. \n";
	}

	void ctime_demo01()
	{
		time_t sec01;
		auto sec02 = time(&sec01);
		std::cout << "timestamp(sec01): " << sec01 << "\n";
		// ReSharper disable once CppDeprecatedEntity
		std::cout << "ctime(sec02): " << ctime(&sec02) << "\n";
		// ReSharper disable once CppDeprecatedEntity
		auto info = localtime(&sec02);
		printf_s("info: %d:%d:%d", info->tm_hour, info->tm_min, info->tm_sec);
	}
}

int main(int argc, char* argv[])
{
	chrono_duration::duration_transform();
	clock_time_point::ctime_demo01();
}
