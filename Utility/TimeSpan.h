#ifndef __UTILITY_TIMING_TIME_SPAN_H__
#define __UTILITY_TIMING_TIME_SPAN_H__

#include "Utility.h"

#include <sstream>
#include <chrono>

namespace Utility
{
	namespace Timing
	{

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
		}; /* end enum TimeUnit */

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
			explicit TimeSpan(long long timeValue) :
				m_duration(timeValue)
			{
			}

			/// <summary>
			/// The time span constructor.
			/// </summary>
			/// <param name="timeValue"> The time span value. </param>
			/// <param name="timeUnit"> The time unit in which the value is represented. </param>
			TimeSpan(long long timeValue, TimeUnit timeUnit) :
				m_duration()
			{
				switch (timeUnit)
				{
				case HOUR:
					m_duration = std::chrono::duration_cast<std::chrono::nanoseconds, long long, std::ratio<3600>>(std::chrono::hours(timeValue));
					break;
				case MINUTE:
					m_duration = std::chrono::duration_cast<std::chrono::nanoseconds, long long, std::ratio<60>>(std::chrono::minutes(timeValue));
					break;
				case SECOND:
					m_duration = std::chrono::duration_cast<std::chrono::nanoseconds, long long, std::ratio<1>>(std::chrono::seconds(timeValue));
					break;
				case MILLISECOND:
					m_duration = std::chrono::duration_cast<std::chrono::nanoseconds, long long, std::milli>(std::chrono::milliseconds(timeValue));
					break;
				case MICROSECOND:
					m_duration = std::chrono::duration_cast<std::chrono::nanoseconds, long long, std::micro>(std::chrono::microseconds(timeValue));
					break;
				case NANOSECOND:
					m_duration = std::chrono::nanoseconds(timeValue);
					break;
				default:
					break;
				}
			}
			/// <summary>
			/// The time span destructor.
			/// </summary>
			~TimeSpan()
			{
			}
			/// <summary>
			/// The time span copy constructor.
			/// </summary>
			TimeSpan(const TimeSpan& timeSpan) :
				m_duration(std::chrono::nanoseconds(timeSpan.GetValue()))
			{
			}

			/// <summary>
			/// The time span move constructor.
			/// </summary>
			TimeSpan(TimeSpan&& timeSpan) :
				m_duration(std::move(timeSpan.m_duration))
			{
			}

			/// <summary>
			/// The time span copy assignment operator.
			/// </summary>
			TimeSpan& operator=(const TimeSpan& timeSpan)
			{
				m_duration = std::chrono::nanoseconds(timeSpan.GetValue());
				return *this;
			}

			/// <summary>
			/// The time span move assignment operator.
			/// </summary>
			TimeSpan& operator=(TimeSpan&& timeSpan) = delete;
			/* ==================== Constructors and destructors end ==================== */

			/* ==================== Non-static member functions begin ==================== */
		public:
			/// <summary> Gets the time span value. </summary>
			/// <returns> The value of the time span. </returns>
			inline long long GetValue() const { return m_duration.count(); }

			//TimeSpan& operator/=(double s)
			//{
			//	m_duration /= s;
			//	return *this;
			//}

			bool operator==(const TimeSpan& timeSpan) const
			{
				return m_duration == timeSpan.m_duration;
			}
			bool operator!=(const TimeSpan& timeSpan) const
			{
				return m_duration != timeSpan.m_duration;
			}
			bool operator<(const TimeSpan &timeSpan) const
			{
				return m_duration < timeSpan.m_duration;
			}
			bool operator>(const TimeSpan &timeSpan) const
			{
				return m_duration > timeSpan.m_duration;
			}
			std::string ToString() const
			{
				std::stringstream ss("");
				ss << m_duration.count() << " [ns]";
				return ss.str();
			}
			/* ==================== Non-static member functions end ==================== */

			/* ==================== Non-static member variables begin ==================== */
		private:
			std::chrono::nanoseconds m_duration;
			/* ==================== Non-static member variables end ==================== */
		}; /* end class TimeSpan */

	} /* end namespace Timing */
} /* end namespace Utility */

#endif /* __UTILITY_TIMING_TIME_SPAN_H__ */