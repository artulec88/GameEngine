#pragma once

#include "Utility.h"
//#include "ISerializable.h"
#include <time.h>
#include <string>

namespace Utility namespace Timing {
{
	/// <summary> Possible time units. </summary>
	enum TimeUnit
	{
		SECOND = 0, // The values here are important. Don't mess with them.
		MILISECOND = 3,
		MICROSECOND = 6,
		NANOSECOND = 9
	};

	template <typename T>
	class UTILITY_API Time
	{
	/* ==================== Constructors and destructors begin ==================== */
	public:
		Time(T timeValue, TimeUnit timeUnit) :
		  m_value(timeValue),
		  m_unit(timeUnit)
		{
		}
		~Time()
		{
		}
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		T GetValue() const { return m_value; }
		TimeUnit GetUnit() const { return m_unit; }
		bool operator<(const Time &time) const
		{
			if (m_unit == time.GetUnit())
			{
				return m_value < time.GetValue();
			}
			// TODO: Finish the function
			return true;
		}
		bool operator>(const Time &arg) const
		{
			if (m_unit == time.GetUnit())
			{
				return m_value > time.GetValue();
			}
			// TODO: Finish the function
			return true;
		}
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
		T m_value;
		TimeUnit m_unit;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class Time<T> */

	class UTILITY_API Timer// : public ISerializable
	{
	/* ==================== Static variables and functions begin ==================== */
	private:
		static LARGE_INTEGER FREQUENCY;
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
		Time ToReal();
		Time ToReal(TimeUnit timeUnit) const;

		void Start();
		void Reset();
		void Stop();

		std::string ToString() const;
		std::string ToDateString(const char *format = "%Y-%m-%d %H:%M:%S") const;

	private:
		template <typename T>
		T CalculateElapsedTimeInSeconds()
		{
			//if (m_isRunning)
			//{
			//}
			// TODO: Finish this function. Add logging.
			return static_cast<T>((m_stopTime.QuadPart - m_startTime.QuadPart)) / FREQUENCY.QuadPart; // in [s]
		}
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
		bool m_isRunning;
		LARGE_INTEGER m_startTime;
		LARGE_INTEGER m_stopTime;
	/* ==================== Non-static member variables end ==================== */
	};

} /* end namespace Timing */

} /* end namespace Utility */