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
			HOUR = 0,
			MINUTE,
			SECOND,
			MILLISECOND,
			MICROSECOND,
			NANOSECOND,
			TIME_UNITS_COUNT, // the number of all possible date-time units
		};

		///// <summary> Possible months. </summary>
		//enum Month
		//{
		//	JANUARY = 0,
		//	FEBRUARY,
		//	MARCH,
		//	APRIL,
		//	MAY,
		//	JUNE,
		//	JULY,
		//	AUGUST,
		//	SEPTEMBER,
		//	OCTOBER,
		//	NOVEMBER,
		//	DECEMBER,
		//	MONTHS_COUNT, // the number of months
		//};

		/// <summary>
		/// The time span objects are used particularly for time measurement.
		/// </summary>
		class TimeSpan
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary>
			/// The time span default constructor.
			/// </summary>
			UTILITY_API TimeSpan();
			/// <summary>
			/// The time span constructor.
			/// </summary>
			/// <param name="timeValue"> The time span value. </param>
			/// <param name="timeUnit"> The time unit in which the value is represented. </param>
			UTILITY_API TimeSpan(float timeValue, TimeUnit timeUnit);
			/// <summary>
			/// The time span destructor.
			/// </summary>
			UTILITY_API ~TimeSpan();
			/// <summary>
			/// The time span copy constructor.
			/// </summary>
			UTILITY_API TimeSpan(const TimeSpan& timeSpan);
			/// <summary>
			/// The time span move constructor.
			/// </summary>
			UTILITY_API TimeSpan(TimeSpan&& timeSpan);
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			/// <summary> Gets the time span value. </summary>
			/// <returns> The value of the time span. </returns>
			UTILITY_API inline float GetValue() const { return m_value; }
			/// <summary> Gets the time span unit. </summary>
			/// <returns> The time unit of the time span. </returns>
			UTILITY_API inline TimeUnit GetTimeUnit() const { return m_timeUnit; }
			/// <summary>
			/// Adjusts time unit to the value, e.g. if value is equal to 0.003 and the unit is [s] the function will replace it so that value equals 3 and the unit is [ms].
			/// </summary>
			UTILITY_API void AdjustUnitToValue();
			UTILITY_API TimeSpan CalculateTimeSpanWithAdjustedUnitToValue() const;
			UTILITY_API TimeSpan& operator/=(int s);
			/// <summary>
			/// The time span copy assignment operator.
			/// </summary>
			UTILITY_API void operator=(const TimeSpan& timeSpan);
			/// <summary>
			/// The time span move assignment operator.
			/// </summary>
			UTILITY_API void operator=(TimeSpan&& timeSpan);
			UTILITY_API bool operator==(const TimeSpan& timeSpan) const;
			UTILITY_API bool operator!=(const TimeSpan& timeSpan) const;
			UTILITY_API bool operator<(const TimeSpan &timeSpan) const;
			UTILITY_API bool operator>(const TimeSpan &timeSpan) const;
			UTILITY_API std::string ToString() const;
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			float m_value;
			TimeUnit m_timeUnit;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class TimeSpan */

		class DateTime
		{
			/* ==================== Static variables and functions begin ==================== */
		public:
			static constexpr int MONTHS_COUNT = 12;
			static const std::array<int, MONTHS_COUNT> DAYS_COUNT_IN_MONTH;
			static const std::array<int, MONTHS_COUNT> DAYS_COUNT_FROM_FIRST_DAY_IN_YEAR;
			static const std::array<std::array<float, TIME_UNITS_COUNT>, TIME_UNITS_COUNT> TIME_UNITS_CONVERSION_FACTORS;
			/// <summary> The number of nanoseconds in one microsecond. </summary>
			static const int NANOSECONDS_PER_MICROSECOND;
			/// <summary> The number of microseconds in one millisecond. </summary>
			static const int MICROSECONDS_PER_MILLISECOND;
			/// <summary> The number of milliseconds during one second. </summary>
			static const int MILLISECONDS_PER_SECOND;
			/// <summary> The number of seconds during one minute. </summary>
			static const int SECONDS_PER_MINUTE;
			/// <summary> The number of seconds during one hour. </summary>
			static const int SECONDS_PER_HOUR;
			/// <summary> The number of seconds during one day. </summary>
			static const int SECONDS_PER_DAY;
			/// <summary> The number of minutes during one hour. </summary>
			static const int MINUTES_PER_HOUR;
			/// <summary> The number of hours during one day. </summary>
			static const int HOURS_PER_DAY;
			/// <summary> The number of days during one year. </summary>
			static const int DAYS_PER_YEAR; // We don't account for "przestêpny" years.

			/// <summary>
			/// Calculates and return current local time.
			/// </summary>
			/// <remarks>
			/// See http ://www.cplusplus.com/reference/ctime/localtime/ or http ://www.cplusplus.com/reference/ctime/strftime/.
			/// </remarks>
			UTILITY_API static DateTime Now();
			UTILITY_API static std::string ConvertTimeUnitToString(TimeUnit timeUnit);
			UTILITY_API static float GetTimeUnitConvertingFactor(TimeUnit fromtimeUnit, TimeUnit toTimeUnit);
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary> Date time constructor. </summary>
			/// <param name="year"> The year. </param>
			/// <param name="month"> The month. The value must lie in range from <code>0</code>, which represents January, to <code>11</code>, which represents December. </param>
			/// <param name="day"> The day. The value must lie in range from <code>0</code> to the number of days in the specified <paramref name="month"/> exclusive. </param>
			/// <param name="hour"> The hour. The value must lie in range from <code>0</code> to <code>23</code>. </param>
			/// <param name="minute"> The minute. The value must lie in range from <code>0</code> to <code>59</code>. </param>
			/// <param name="second"> The second. The value must lie in range from <code>0</code> to <code>59</code>. </param>
			/// <param name="millisecond"> The millisecond. The value must lie in range from <code>0</code> to <code>999</code>. </param>
			UTILITY_API DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond = 0);
			UTILITY_API ~DateTime();
			UTILITY_API DateTime(const DateTime& dateTime);
			UTILITY_API DateTime(DateTime&& dateTime);
			UTILITY_API DateTime& operator=(const DateTime& dateTime);
			UTILITY_API DateTime& operator=(DateTime&& dateTime);
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			//DateTime operator+(const DateTime& dateTime);
			//DateTime operator-(const DateTime& dateTime);
			//DateTime& operator+=(const DateTime& dateTime);

			UTILITY_API DateTime operator+(const TimeSpan& timeSpan);
			UTILITY_API DateTime operator-(const TimeSpan& timeSpan);
			UTILITY_API DateTime& operator+=(const TimeSpan& timeSpan);
			UTILITY_API DateTime& operator-=(const TimeSpan& timeSpan);

			UTILITY_API bool operator==(const DateTime& dateTime) const;
			UTILITY_API bool operator!=(const DateTime& dateTime) const;
			UTILITY_API bool operator<(const DateTime& dateTime) const;
			UTILITY_API bool operator>(const DateTime& dateTime) const;

			/// <summary>
			/// Returns the number of days that has passed since the beginning of the year. We assume the year has 365 days (we don't take into account the "przestêpny" years).
			/// </summary>
			/// <returns> Number of days counting from the beginning of the year. </returns>
			UTILITY_API int GetDayInYear() const;
			/// <summary>
			/// Returns the number of milliseconds that has passed since the beginning of the day.
			/// </summary>
			/// <returns> Number of milliseconds counting from the beginning of the day. </returns>
			UTILITY_API int GetDayTime() const;

			/// <summary>
			/// Converts the <code>Time</code> object into the string in a given date/time format.
			/// </summary>
			/// <remarks>
			/// Visit the page http://www.cplusplus.com/reference/ctime/strftime/ for additional supported formats.
			/// </remarks>
			UTILITY_API std::string ToString(const char *format = "%Y-%m-%d %H:%M:%S") const;
		private:
			DateTime& AddTimeSpan(float timeSpanValue, TimeUnit timeUnit);
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			int m_year;
			int m_month;
			int m_day;
			int m_hour;
			int m_minute;
			int m_second;
			int m_millisecond;
			//int m_microsecond;
			//int m_nanosecond;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class DateTime */

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
				return static_cast<float>(1000.0f * (m_stopTime.QuadPart - m_startTime.QuadPart)) / GetFrequency().QuadPart; // in [s]
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