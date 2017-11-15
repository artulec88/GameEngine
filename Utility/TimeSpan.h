#ifndef __UTILITY_TIMING_TIME_SPAN_H__
#define __UTILITY_TIMING_TIME_SPAN_H__

#include "Utility.h"

#include <sstream>
#include <chrono>

namespace utility
{
	namespace timing
	{

		// TODO: Make this enum scoped.
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
			/// <param name="timeValue"> The amount of time specified in nanoseconds. Default value is <code>0</code>.</param>
			explicit TimeSpan(const long long timeValue = 0L) :
				m_duration(timeValue)
			{
			}

			/// <summary>
			/// The time span constructor.
			/// </summary>
			/// <param name="timeValue"> The time span value. </param>
			/// <param name="timeUnit"> The time unit in which the value is represented. </param>
			TimeSpan(const long long timeValue, const TimeUnit timeUnit) :
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
				case TIME_UNITS_COUNT:
				default:
					break;
				}
			}
			explicit TimeSpan(const std::chrono::nanoseconds& duration) :
				m_duration(duration)
			{
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
			TimeSpan(TimeSpan&& timeSpan) noexcept:
				m_duration(move(timeSpan.m_duration))
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
			/// <summary> Gets the time span value. </summary>
			/// <returns> The value of the time span. </returns>
			long long GetValue() const { return m_duration.count(); }

			//TimeSpan& operator/=(double s)
			//{
			//	m_duration /= s;
			//	return *this;
			//}

			TimeSpan operator+(const TimeSpan& timeSpan) const
			{
				return TimeSpan(m_duration + timeSpan.m_duration);
			}
			TimeSpan operator-(const TimeSpan& timeSpan) const
			{
				return TimeSpan(m_duration - timeSpan.m_duration);
			}
			TimeSpan& operator+=(const TimeSpan& timeSpan)
			{
				m_duration += timeSpan.m_duration;
				return *this;
			}
			TimeSpan& operator-=(const TimeSpan& timeSpan)
			{
				m_duration -= timeSpan.m_duration;
				return *this;
			}
			TimeSpan operator*(const long long s) const
			{
				return TimeSpan(m_duration.count() * s);
			}
			TimeSpan& operator*=(const long long s)
			{
				m_duration *= s;
				return *this;
			}
			float operator/(const TimeSpan& timeSpan) const
			{
				return static_cast<float>(m_duration.count()) / timeSpan.m_duration.count();
			}
			/// <summary> Time span division operator. The returned time span may have rounding errors. </summary>
			/// <param name="s"> The floating-point value representing the divisor. </param>
			/// <returns> The time span which is a result of dividing the source time span by the floating-point value. </returns>
			TimeSpan operator/(const long long s) const
			{
				return TimeSpan(m_duration.count() / s);
			}
			TimeSpan& operator/=(const long long s)
			{
				m_duration /= s;
				return *this;
			}

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
			bool operator<(const TimeUnit timeUnit) const
			{
				switch (timeUnit)
				{
				case HOUR:
					return m_duration < std::chrono::hours(1);
				case MINUTE:
					return m_duration < std::chrono::minutes(1);
				case SECOND:
					return m_duration < std::chrono::seconds(1);
				case MILLISECOND:
					return m_duration < std::chrono::milliseconds(1);
				case MICROSECOND:
					return m_duration < std::chrono::microseconds(1);
				case NANOSECOND:
					return m_duration < std::chrono::nanoseconds(1);
				case TIME_UNITS_COUNT:
				default:
					// TODO: Print message about incorrect time unit
					return true;
				}
			}
			bool operator>(const TimeSpan &timeSpan) const
			{
				return m_duration > timeSpan.m_duration;
			}
			bool operator>(const TimeUnit timeUnit) const
			{
				switch (timeUnit)
				{
				case HOUR:
					return m_duration > std::chrono::hours(1);
				case MINUTE:
					return m_duration > std::chrono::minutes(1);
				case SECOND:
					return m_duration > std::chrono::seconds(1);
				case MILLISECOND:
					return m_duration > std::chrono::milliseconds(1);
				case MICROSECOND:
					return m_duration > std::chrono::microseconds(1);
				case NANOSECOND:
					return m_duration > std::chrono::nanoseconds(1);
				case TIME_UNITS_COUNT:
				default:
					return true;
				}
			}
			std::string ToString(const TimeUnit timeUnit) const
			{
				std::stringstream ss("");
				switch (timeUnit)
				{
				case HOUR:
					ss << std::chrono::duration_cast<std::chrono::hours>(m_duration).count() << " [h]";
					break;
				case MINUTE:
					ss << std::chrono::duration_cast<std::chrono::minutes>(m_duration).count() << " [m]";
					break;
				case SECOND:
					ss << std::chrono::duration_cast<std::chrono::seconds>(m_duration).count() << " [s]";
					break;
				case MILLISECOND:
					ss << std::chrono::duration_cast<std::chrono::milliseconds>(m_duration).count() << " [ms]";
					break;
				case MICROSECOND:
					ss << std::chrono::duration_cast<std::chrono::microseconds>(m_duration).count() << " [us]";
					break;
				case NANOSECOND:
					ss << m_duration.count() << " [ns]";
				case TIME_UNITS_COUNT:
				default:
					// TODO: Error message
					break;
				}
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