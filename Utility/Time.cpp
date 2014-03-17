#include "stdafx.h"
#include "Time.h"
#include <Windows.h>
#include <stdio.h>
#include <sstream>
#include <limits>

using namespace Utility;
using namespace std;

#ifdef INFINITE
#undef INFINITE
#endif
#define INFINITE (1000001)
#define DISABLED (1000002)

const Time Time::Infinite(0, INFINITE);
const Time Time::Disabled(0, DISABLED);
const Time Time::Short(0, 100);

Time::Time() :
	seconds(0),
	micros(0)
{
}

Time::Time(int seconds, int micros) :
	seconds(seconds),
	micros(micros)
{
}

Time::Time(double value)
{
	operator=(value);
}

Time::~Time()
{
}

double Time::ToReal() const
{
	return seconds + static_cast<double>(micros) / 1000000;
}

int Time::Signum(const Time &arg) const
{
	if (arg.seconds > seconds) return 1;
	if (arg.seconds < seconds) return -1;

	if (arg.micros > micros) return 1;
	if (arg.micros < micros) return -1;

	return 0;

}

void Time::Reset()
{
	seconds = 0;
	micros = 0;
}

bool Time::IsInfinite() const
{
	return micros == INFINITE;
}

bool Time::IsDisabled() const
{
	return micros == DISABLED; 
}

void Time::MakeDisabled()
{
	micros = INFINITE;
}

void Time::MakeInfinite()
{
	micros = DISABLED;
}

bool Time::operator==(const Time &arg) const
{
	return Signum(arg) == 0;
}

bool Time::operator!=(const Time &arg) const
{
	return Signum(arg) != 0;
}

bool Time::operator<(const Time &arg) const
{
	return Signum(arg) == 1;
}

bool Time::operator>(const Time &arg) const
{
	return Signum(arg) == -1;
}

bool Time::operator<=(const Time &arg) const
{
	return Signum(arg) >= 0;
}

bool Time::operator>=(const Time &arg) const
{
	return Signum(arg) <= 0;
}

Time Time::operator+(const Time &arg) const
{
	Time result;
	if (IsDisabled() || IsDisabled())
	{
		result.MakeDisabled();
	}
	else if (IsInfinite() || arg.IsInfinite())
	{
		result.MakeInfinite();
	}
	else
	{
		result.seconds = seconds + arg.seconds;
		result.micros = micros + arg.micros;
		if (result.micros >= 1000000)
		{
			result.micros -= 1000000;
			result.seconds++;
		}
	}

	return result;	
}


Time Time::operator-(const Time &arg) const
{
	Time result;
	if (IsDisabled() || IsDisabled())
	{
		result.MakeDisabled();
	}
	else if (IsInfinite() || arg.IsInfinite())
	{
		result.MakeInfinite();
	}
	else
	{
		result.seconds = seconds - arg.seconds;
		result.micros = micros - arg.micros;
		if (result.micros < 0)
		{
			result.micros += 1000000;
			result.seconds--;
		}
	}

	return result;	

}

Time Time::Now()
{
	//struct timespec time;
	//clock_gettime(CLOCK_REALTIME, &time);
	//return Time(time.tv_sec, time.tv_nsec / 1000);

	SYSTEMTIME st;
	GetSystemTime(&st);
	return Time(st.wSecond, st.wMilliseconds);
}

Time Time::Eternity()
{
	return Time(INT_MAX, 0);
	//return Time(2000000000, 0);
}

Time Time::Chaos()
{
	return Time(0, 0);
}

Time Time::Second()
{
	return Time(1, 0);
}

const Time& Time::operator=(double value)
{
	seconds = (int)value;
	micros = (int)((value - seconds) * 1000000);
	return *this;
}

const Time& Time::operator=(const Time &t)
{
	seconds = t.seconds;
	micros = t.micros;
	return *this;
}

std::string Time::ToString() const
{
	//char buffer[30];
	//sprintf(buffer, "%d:%06d", seconds, micros);
	//return std::string(buffer);

	std::stringstream ss("");
	ss << seconds;
	ss << ":";
	ss << micros;
	return ss.str();
}

std::string Time::ToDateString(const char *format) const
{
	//char buffer[80];
	//memset(buffer, 0, 80);
	//time_t t = seconds;
	//int result = strftime(buffer, 79, format, localtime(&t));
	//if (result == 0)
	//{
	//	return std::string("time-error");
	//}
	//buffer[result] = '\0';
	//return string(buffer);

	time_t rawtime = seconds;
	struct tm timeinfo;
	char buffer [80];

	time (&rawtime);
	localtime_s(&timeinfo, &rawtime);

	if ( (strftime (buffer, 80, format, &timeinfo)) == 0)
	{
		return std::string("time-error");
	}
	return string(buffer);
}