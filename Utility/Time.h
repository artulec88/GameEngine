#ifndef __UTILITY_TIMING_TIME_H__
#define __UTILITY_TIMING_TIME_H__

#include "Utility.h"
//#include "ISerializable.h"
#include "TimeSpan.h"

#include <string>
#include <chrono>

#define TIME_MEASUREMENT_ENABLED

#ifdef TIME_MEASUREMENT_ENABLED
#define START_TIMER(timerID) Utility::Timing::Timer timerID; do { timerID.Start(); } while(0)
#define RESET_TIMER(timerID) do { if (true) { timerID.Reset(); } } while(0)
#define STOP_TIMER(timerID, countStats, minMaxTime, timeSum) do { if (true) timerID.Stop(); } while (0)
#else
#define START_TIMER(timerID)
#define RESET_TIMER(timerID)
#define STOP_TIMER(timerID, countStats, minMaxTime, timeSum)
#endif

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

		class DateTime
		{
			/* ==================== Static variables and functions begin ==================== */
		public:
			/// <summary> The number of nanoseconds in one microsecond. </summary>
			static constexpr int NANOSECONDS_PER_MICROSECOND = 1000;
			/// <summary> The number of microseconds in one millisecond. </summary>
			static constexpr int MICROSECONDS_PER_MILLISECOND = 1000;
			/// <summary> The number of milliseconds during one second. </summary>
			static constexpr int MILLISECONDS_PER_SECOND = 1000;
			/// <summary> The number of seconds during one minute. </summary>
			static constexpr int SECONDS_PER_MINUTE = 60;
			/// <summary> The number of seconds during one hour. </summary>
			static constexpr int SECONDS_PER_HOUR = 3600;
			/// <summary> The number of seconds during one day. </summary>
			static constexpr int SECONDS_PER_DAY = 86400;
			/// <summary> The number of minutes during one hour. </summary>
			static constexpr int MINUTES_PER_HOUR = 60;
			/// <summary> The number of hours during one day. </summary>
			static constexpr int HOURS_PER_DAY = 24;
			/// <summary> The number of days during one year. </summary>
			static constexpr int DAYS_PER_YEAR = 365; // We don't account for "przestêpny" years.
			static constexpr int MONTHS_COUNT = 12;
			static /* constexpr: problem- see https://connect.microsoft.com/VisualStudio/Feedback/Details/2092790 */ std::array<int, MONTHS_COUNT> DAYS_COUNT_IN_MONTH;
			static /* constexpr: problem- see https://connect.microsoft.com/VisualStudio/Feedback/Details/2092790 */ std::array<int, MONTHS_COUNT> DAYS_COUNT_FROM_FIRST_DAY_IN_YEAR;

			/// <summary>
			/// Calculates and return current local time.
			/// </summary>
			/// <remarks>
			/// See http ://www.cplusplus.com/reference/ctime/localtime/ or http ://www.cplusplus.com/reference/ctime/strftime/.
			/// </remarks>
			UTILITY_API static DateTime Now();
			UTILITY_API static std::string ConvertTimeUnitToString(TimeUnit timeUnit);
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary> Date time constructor. </summary>
			/// <param name="year"> The year. </param>
			/// <param name="month"> The month. The value must lie in range from <code>1</code>, which represents January, to <code>12</code>, which represents December. </param>
			/// <param name="day"> The day. The value must lie in range from <code>1</code> to the number of days in the specified <paramref name="month"/> exclusive. </param>
			/// <param name="hour"> The hour. The value must lie in range from <code>0</code> to <code>23</code>. </param>
			/// <param name="minute"> The minute. The value must lie in range from <code>0</code> to <code>59</code>. </param>
			/// <param name="second"> The second. The value must lie in range from <code>0</code> to <code>59</code>. </param>
			/// <param name="millisecond"> The millisecond. The value must lie in range from <code>0</code> to <code>999</code>. </param>
			UTILITY_API DateTime(unsigned int year, unsigned int month, unsigned int day, unsigned int hour, unsigned int minute, unsigned int second);
			UTILITY_API DateTime(const std::chrono::system_clock::time_point& timePoint);
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

			DateTime operator+(const TimeSpan& timeSpan)
			{
				return DateTime(*this) += timeSpan;
			}
			DateTime operator-(const TimeSpan& timeSpan)
			{
				return DateTime(*this) -= timeSpan;
			}
			DateTime& operator+=(const TimeSpan& timeSpan)
			{
				m_timePoint += std::chrono::nanoseconds(timeSpan.GetValue());
				//m_timePoint += timeSpan.GetDuration();
				return *this;
			}
			DateTime& operator-=(const TimeSpan& timeSpan)
			{
				m_timePoint -= std::chrono::nanoseconds(timeSpan.GetValue());
				return *this;
			}

			UTILITY_API bool operator==(const DateTime& dateTime) const;
			UTILITY_API bool operator!=(const DateTime& dateTime) const;
			UTILITY_API bool operator<(const DateTime& dateTime) const;
			UTILITY_API bool operator>(const DateTime& dateTime) const;

			/// <summary>
			/// Returns the number of days that has passed since the beginning of the year. We assume the year has 365 days (we don't take into account the "przestêpny" years).
			/// </summary>
			/// <returns> Number of days counting from the beginning of the year. </returns>
			//UTILITY_API int GetDayInYear() const;
			/// <summary>
			/// Returns the number of milliseconds that has passed since the beginning of the day.
			/// </summary>
			/// <returns> Number of milliseconds counting from the beginning of the day. </returns>
			//UTILITY_API int GetDayTime() const;

			/// <summary>
			/// Converts the <code>Time</code> object into the string in a given date/time format.
			/// </summary>
			/// <remarks>
			/// Visit the page http://www.cplusplus.com/reference/ctime/strftime/ for additional supported formats.
			/// </remarks>
			UTILITY_API std::string ToString(const char *format = "%Y-%m-%d %H:%M:%S") const;
		private:
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds /* std::chrono::system_clock::duration */> m_timePoint;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class DateTime */

		/// <summary>
		/// The timer which can be used to measure the passing time.
		/// </summary>
		class Timer
		{
			/* ==================== Static variables and functions begin ==================== */
			/* ==================== Static variables and functions end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			/// <summary> Timer constructor. </summary>
			UTILITY_API Timer();
			/// <summary> Timer destructor. </summary>
			UTILITY_API ~Timer();
			Timer(const Timer& timer) = delete;
			Timer(Timer&& timer) = delete;
			Timer& operator=(const Timer& timer) = delete;
			Timer& operator=(Timer&& timer) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			UTILITY_API inline TimeSpan GetTimeSpan() const
			{
				return TimeSpan(std::chrono::duration_cast<std::chrono::nanoseconds>(m_stopTime - m_startTime));
			}

			UTILITY_API inline long long GetDuration(TimeUnit timeUnit) const
			{
				switch (timeUnit)
				{
				case HOUR: return std::chrono::duration_cast<std::chrono::hours>(m_stopTime - m_startTime).count();
				case MINUTE: return std::chrono::duration_cast<std::chrono::minutes>(m_stopTime - m_startTime).count();
				case SECOND: return std::chrono::duration_cast<std::chrono::seconds>(m_stopTime - m_startTime).count();
				case MILLISECOND: return std::chrono::duration_cast<std::chrono::milliseconds>(m_stopTime - m_startTime).count();
				case MICROSECOND: return std::chrono::duration_cast<std::chrono::microseconds>(m_stopTime - m_startTime).count();
				case NANOSECOND: return std::chrono::duration_cast<std::chrono::nanoseconds>(m_stopTime - m_startTime).count();
				case TIME_UNITS_COUNT:
				default:
					//ERROR_LOG_UTILITY("Incorrect time unit specified: ", timeUnit);
					return std::chrono::duration_cast<std::chrono::nanoseconds>(m_stopTime - m_startTime).count();
				}
			}

			UTILITY_API void Start();
			UTILITY_API void Reset();
			UTILITY_API void Stop();
			UTILITY_API bool IsRunning() const { return m_isRunning; }
			UTILITY_API std::string ToString(TimeUnit timeUnit = NANOSECOND) const
			{
				std::stringstream ss("");
				ss << GetDuration(timeUnit) << " " << DateTime::ConvertTimeUnitToString(timeUnit);
				return ss.str();
			}
		private:
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			std::chrono::time_point<std::chrono::system_clock> m_startTime;
			std::chrono::time_point<std::chrono::system_clock> m_stopTime;
			bool m_isRunning;
			// TODO: Compiler warning C4820: '7' bytes padding added after data member m_isRunning.
			/* ==================== Non-static member variables end ==================== */
		}; /* end class Timer */

	} /* end namespace Timing */

} /* end namespace Utility */

#endif /* __UTILITY_TIMING_TIME_H__ */