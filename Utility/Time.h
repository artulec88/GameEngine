#ifndef __UTILITY_TIME_H__
#define __UTILITY_TIME_H__

#include "Utility.h"
//#include "ISerializable.h"
#include <time.h>
#include <string>
#include "ILogger.h"

#define START_TIMER(timerID) Timing::Timer timerID; timerID.Start();
#define RESET_TIMER(timerID) do { if (true) { timerID.Reset(); } } while(0)
#define STOP_TIMER(timerID, countStats, minMaxTime, timeSum) do { if (true) timerID.Stop(); } while (0)

namespace Utility { namespace Timing
{
	/// <summary> Possible time units. </summary>
	enum TimeUnit
	{
		SECOND = 0,
		MILLISECOND,
		MICROSECOND,
		NANOSECOND
	};

	class UTILITY_API TimeSpan
	{
	/* ==================== Static variables and functions begin ==================== */
	/* ==================== Static variables and functions end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		TimeSpan(float timeValue, TimeUnit timeUnit) :
		  m_value(timeValue),
		  m_unit(timeUnit)
		{
		}
		~TimeSpan()
		{
		}
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		float GetValue() const { return m_value; }
		TimeUnit GetUnit() const { return m_unit; }
		void AdjustUnitToValue();
		TimeSpan& operator/=(int s);
		bool operator<(const TimeSpan &timeSpan) const;
		bool operator>(const TimeSpan &timeSpan) const;
		std::string ToString() const;
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
		float m_value;
		TimeUnit m_unit;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class TimeSpan<T> */

	class UTILITY_API Time
	{
	/* ==================== Static variables and functions begin ==================== */
	public:
		static const float ONE_OVER_BILLION;
		static const float ONE_OVER_MILLION;
		static const float ONE_OVER_THOUSAND;
		static const float ONE;
		static const float ONE_THOUSAND;
		static const float ONE_MILLION;
		static const float ONE_BILLION;
		static Time Now();
		static std::string ConvertTimeUnitToString(TimeUnit timeUnit);
		static float TimeUnitConvertingFactor(TimeUnit fromTimeUnit, TimeUnit toTimeUnit);
	/* ==================== Static variables and functions end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		Time(int seconds, int microseconds) :
		  m_seconds(seconds),
		  m_microseconds(microseconds)
		{
		}
		~Time()
		{
		}
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		std::string ToDateString(const char *format = "%Y-%m-%d %H:%M:%S") const;
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
		int m_seconds;
		int m_microseconds;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class Time */

	class UTILITY_API Timer// : public ISerializable
	{
	/* ==================== Static variables and functions begin ==================== */
	private:
		static bool isFrequencyInitialized;
		static LARGE_INTEGER frequency;
	public:
		static LARGE_INTEGER GetFrequency();
	/* ==================== Static variables and functions end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		Timer();
		~Timer();
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		TimeSpan GetTimeSpan() const;
		TimeSpan GetTimeSpan(TimeUnit timeUnit) const;

		void Start();
		void Reset();
		void Stop();
		bool IsRunning() const { return m_isRunning; }
	private:
		float CalculateElapsedTimeInMilliseconds() const
		{
			//LONGLONG diff = m_stopTime.QuadPart - m_startTime.QuadPart;
			//INFO_LOG("diff = %d, frequency = %d", diff, GetFrequency().QuadPart);
			return static_cast<float>(Time::ONE_THOUSAND * (m_stopTime.QuadPart - m_startTime.QuadPart)) / GetFrequency().QuadPart; // in [s]
		}
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
		bool m_isRunning;
		LARGE_INTEGER m_startTime;
		LARGE_INTEGER m_stopTime;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class Timer */

} /* end namespace Timing */

} /* end namespace Utility */

#endif /* __UTILITY_TIME_H__ */