#ifndef __UTILITY_TIMING_TIME_H__
#define __UTILITY_TIMING_TIME_H__

#include "Utility.h"
//#include "ISerializable.h"
#include <string>
#include <Windows.h> // TODO: This is a platform-dependent header. It shouldn't be directly included here.
//#include <winnt.h>

#define START_TIMER(timerID) Utility::Timing::Timer timerID; timerID.Start();
#define RESET_TIMER(timerID) do { if (true) { timerID.Reset(); } } while(0)
#define STOP_TIMER(timerID, countStats, minMaxTime, timeSum) do { if (true) timerID.Stop(); } while (0)

namespace Utility {
	namespace Timing
	{
		enum Daytime
		{
			NIGHT = 0,
			BEFORE_DAWN,
			SUNRISE,
			DAY,
			SUNSET,
			AFTER_DUSK
		};

		/// <summary> Possible time units. </summary>
		enum TimeUnit
		{
			SECOND = 0,
			MILLISECOND,
			MICROSECOND,
			NANOSECOND
		};

		class TimeSpan
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			UTILITY_API TimeSpan() :
				m_value(0.0f),
				m_unit(SECOND)
			{
			}

			UTILITY_API TimeSpan(float timeValue, TimeUnit timeUnit) :
				m_value(timeValue),
				m_unit(timeUnit)
			{
			}
			
			UTILITY_API ~TimeSpan()
			{
			}
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			UTILITY_API float GetValue() const { return m_value; }
			UTILITY_API TimeUnit GetUnit() const { return m_unit; }
			UTILITY_API void AdjustUnitToValue();
			UTILITY_API TimeSpan& operator/=(int s);
			UTILITY_API void operator=(const TimeSpan& timeSpan);
			UTILITY_API bool operator<(const TimeSpan &timeSpan) const;
			UTILITY_API bool operator>(const TimeSpan &timeSpan) const;
			UTILITY_API std::string ToString() const;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			float m_value;
			TimeUnit m_unit;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class TimeSpan */

		class Time
		{
			/* ==================== Static variables and functions begin ==================== */
		public:
			UTILITY_API static const float ONE_OVER_BILLION;
			UTILITY_API static const float ONE_OVER_MILLION;
			UTILITY_API static const float ONE_OVER_THOUSAND;
			UTILITY_API static const float ONE;
			UTILITY_API static const float ONE_THOUSAND;
			UTILITY_API static const float ONE_MILLION;
			UTILITY_API static const float ONE_BILLION;

			/// <summary> The number of seconds during one minute. </summary>
			UTILITY_API static const int SECONDS_PER_MINUTE;
			/// <summary> The number of seconds during one hour. </summary>
			UTILITY_API static const int SECONDS_PER_HOUR;
			/// <summary> The number of seconds during one day. </summary>
			UTILITY_API static const int SECONDS_PER_DAY;
			/// <summary> The number of days during one year. </summary>
			UTILITY_API static const int DAYS_PER_YEAR; // We don't account for "przestêpny" years.

			UTILITY_API static Time Now();
			UTILITY_API static std::string ConvertTimeUnitToString(TimeUnit timeUnit);
			UTILITY_API static float TimeUnitConvertingFactor(TimeUnit fromTimeUnit, TimeUnit toTimeUnit);
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			UTILITY_API Time(int seconds, int microseconds) :
				m_seconds(seconds),
				m_microseconds(microseconds)
			{
			}
			UTILITY_API ~Time()
			{
			}
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			/// <summary>
			/// Converts the <code>Time</code> object into the string in a given date/time format.
			/// </summary>
			/// <remarks>
			/// Visit the page http://www.cplusplus.com/reference/ctime/strftime/ for additional supported formats.
			/// </remarks>
			UTILITY_API std::string ToDateString(const char *format = "%Y-%m-%d %H:%M:%S") const;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			int m_seconds;
			int m_microseconds;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class Time */

		class Timer
		{
			/* ==================== Static variables and functions begin ==================== */
		private:
			static bool isFrequencyInitialized;
			static LARGE_INTEGER frequency;
		public:
			UTILITY_API static LARGE_INTEGER GetFrequency();
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			UTILITY_API Timer();
			UTILITY_API ~Timer();
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			UTILITY_API TimeSpan GetTimeSpan() const;
			UTILITY_API TimeSpan GetTimeSpan(TimeUnit timeUnit) const;

			UTILITY_API void Start();
			UTILITY_API void Reset();
			UTILITY_API void Stop();
			UTILITY_API bool IsRunning() const { return m_isRunning; }
		private:
			float CalculateElapsedTimeInMilliseconds() const
			{
				//LONGLONG diff = m_stopTime.QuadPart - m_startTime.QuadPart;
				//INFO_LOG_UTILITY("diff = ", diff, ", frequency = ", GetFrequency().QuadPart);
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

#endif /* __UTILITY_TIMING_TIME_H__ */