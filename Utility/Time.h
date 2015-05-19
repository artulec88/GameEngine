#pragma once

#include "Utility.h"
//#include "ISerializable.h"
#include <time.h>
#include <string>

namespace Utility
{
	class UTILITY_API Time// : public ISerializable
	{
	public:
		int seconds;
		int micros;
	public: // constructors destructors
		Time();
		Time(int seconds, int micros);
		Time(double value);
		~Time();
	public: /* static methods variables */
		static Time Now();
		static Time Eternity();
		static Time Chaos();
		static Time Second();

		static const Time Infinite;
		static const Time Disabled;
		static const Time Short;
	public:
		double ToReal() const;

		Time operator+(const Time &arg) const;
		Time operator-(const Time &arg) const;
			
		int Signum(const Time &arg) const;
		void Reset();

		bool operator<(const Time &arg) const;
		bool operator>(const Time &arg) const;
		bool operator<=(const Time &arg) const;
		bool operator>=(const Time &arg) const;
		bool operator==(const Time &arg) const;
		bool operator!=(const Time &arg) const;

		const Time &operator=(double value);
		const Time &operator=(const Time &t);

		bool IsInfinite() const;
		bool IsDisabled() const;
		void MakeInfinite();
		void MakeDisabled();

		std::string ToString() const;
		std::string ToDateString(const char *format = "%Y-%m-%d %H:%M:%S") const;
	};
} /* end namespace Utility */