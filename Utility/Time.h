#pragma once

#include "Utility.h"
//#include "ISerializable.h"
#include <time.h>
#include <string>

namespace Utility namespace Time
{
	enum TimeUnit
	{
		MINUTES = 0,
		SECONDS = 1,
		MILISECONDS = 2,
		MICROSECONDS = 3,
		NANOSECONDS = 4
	};

	const char* timeUnitStr[] = 
	{
		"[m]",
		"[s]",
		"[ms]",
		"[us]",
		"[ns]",
		NULL
	};

	class UTILITY_API Time// : public ISerializable
	{
	public: // constructors destructors
		explicit Time(float time);
		Time(float time, TimeUnit timeUnit);
		~Time();
	public:
		float ToReal() const;

		Time operator+(const Time &arg) const;
		Time operator-(const Time &arg) const;
		void Reset();

		bool operator<(const Time &arg) const;
		bool operator>(const Time &arg) const;
		bool operator<=(const Time &arg) const;
		bool operator>=(const Time &arg) const;
		bool operator==(const Time &arg) const;
		bool operator!=(const Time &arg) const;

		const Time &operator=(double value);
		const Time &operator=(const Time &t);

		std::string ToString() const;
		std::string ToDateString(const char *format = "%Y-%m-%d %H:%M:%S") const;
	public:
		float m_time;
		TimeUnit m_timeUnit;
	};

} /* end namespace Time */

} /* end namespace Utility */