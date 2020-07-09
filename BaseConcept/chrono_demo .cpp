#include <chrono>
#include <iostream>
#include <string>
#include <ctime>
#include <type_traits>

// chrono ��C++11�����ʱ��⣬Դ��boost.
// ��Ĺؼ��㣺 Durations(ʱ���)chrono::duration ��  TimerPointers(ʱ���) chrono::time_point�� Clock(ʱ��)
// ʱ��ת������ʽ����ת������hours -> minutes, minutes -> seconds.
// ����ת��������ʽת�� duration_cast<chrono::hours> minutes_i ..

using namespace std;

// �򵥸��Ա�׼��chrono�� Duration��  chrono��һ�ؼ���: Duration, ʱ���
namespace chrono_duration
{
	void chrono_demo01()
	{
		// chrono �����˻�������ʱ�䵥λ��Ҳ���Զ���ʱ��
		using TSec = chrono::duration<int32_t>;
		using TMilli = chrono::duration<uint64_t, ratio<1, 1000>>;
		// �鿴��׼��Ҳ����֪���� ���ڳ��õ�ʱ�䵥λҲ������������ģ� ���磺
		typedef chrono::duration<int64_t, ratio<1, 1>> TSecond; //��׼�ⶨ�����£�
		using TSecondStd = chrono::duration<long long>; // ==> typedef chrono::duration<int64_t, ratio<1,1>> TSEC;
		typedef chrono::duration<int32_t, ratio<60>> TMinute;
		typedef chrono::duration<int32_t, ratio<3600, 1>> THour;
		typedef chrono::duration<int32_t, ratio<3600 * 24, 1>> TDay; // 1����24*3600�롣86400
		typedef chrono::duration<int64_t, ratio<1, 10>> TDeci; // 10��֮1��
		using TCenti = chrono::duration<int64_t, ratio<1, 100>>; // �ٷ�֮һ��
		// ���õı�����ratio�Ѿ�������ˣ� ��ֱ��ʹ�ã�
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
		explicit constexpr Duration(const Duration<Rep2, Period2>& d) // ����<�������캯��>
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

		using NanoCentury = chrono::duration<double, std::ratio<100 * year, 1000000000>>; // ÿ���Ͷ������룬 ���������Ǵ���ģ�Ӧ�������淽ʽ��
		typedef chrono::duration<uint64_t, ratio<100 * year * 1000000000, 1>> NanoCentury2; // ÿ��������

		typedef chrono::duration<int32_t, std::centi /* std::ratio<1, 100> */> Jiffies;

		const chrono::seconds sec(1);

		// �޾�����ʧת������ʽ�Զ�ת��
		std::cout << chrono::microseconds(sec).count() << "  micro_seconds \n"
			<< Shakes(sec).count() << "  Shakes. \n"
			<< Jiffies(sec).count() << "  Jiffies: \n";

		// �о�����ʧת���� duration_cast<chrono::minutes>(sec) Ҳ����С��λ���λת��

		std::cout << chrono::duration_cast<chrono::minutes>(sec).count() << " minutes. \n";
		std::cout << NanoCentury(3).count() << " NanoCentury(3).\n";
		typedef std::ratio<year * 100, 1000000000> RatioCenturyNano;

		std::cout << "ratio century nano: " << RatioCenturyNano::num << "/" << RatioCenturyNano::den << "  .\n";

		RatioCenturyNano::type ra01;
		std::cout << "ratio::type, ra01.num:  " << ra01.num << "/" << ra01.den << ", \n";
	}
}

// chrono �ڶ��ؼ��� time_point;  ʱ���, ��Ҫ����ʱ�ӣ�steady_clock, system_clock;

// chrono �����ؼ���clock
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

		// ���� template<typename Duration2, typename=typename enable_if_t<is_convertible_v<...> 
		// typename = enable_if<is_conf
		// enable_if, enable_if_t ����ģ�����ͼ��,ʹ�ñȽϸ��ӣ���ʱ�ȼ������˵�� 

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

	// ģ�庯����������������棬���Ե÷ǳ����ӣ������ص������ģ�����ʹ�á�
	template <typename Clock, typename Duration>
	template <class ToDuration, class Clock2, class Duration2>
	constexpr auto TimePoint<Clock, Duration>::time_point_cast(
		const TimePoint<Clock2, Duration2>& t) -> TimePoint<Clock2, ToDuration>
	{
		TimePoint<Clock2, ToDuration> res;
		return res;
	}

	// ��Щ������Դ�ڱ�׼��chrono.h�ļ��� ����ʵ��Ŀǰ��û����

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
