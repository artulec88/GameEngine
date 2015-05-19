#include "stdafx.h"
#include "Time.h"
#include <Windows.h>
#include <stdio.h>
#include <sstream>
#include <limits>

using namespace Utility::Time;
using namespace std;

Time::Time(float time) :
	m_time(time),
	m_timeUnit(MILISECONDS)
{
}

Time::Time(float time, TimeUnit timeUnit) :
	m_time(time),
	m_timeUnit(timeUnit)
{
}

Time::~Time()
{
}

double Time::ToReal() const
{
	return seconds + static_cast<double>(micros) / 1000000;
}

bool Time::operator==(const Time &arg) const
{
	return false;
}

bool Time::operator!=(const Time &arg) const
{
	return true;
}

bool Time::operator<(const Time &arg) const
{
	return false;
}

bool Time::operator>(const Time &arg) const
{
	return false;
}

bool Time::operator<=(const Time &arg) const
{
	return false;
}

bool Time::operator>=(const Time &arg) const
{
	return false;
}

Time Time::operator+(const Time &arg) const
{
	Time result;
	result.seconds = seconds + arg.seconds;
	result.micros = micros + arg.micros;
	if (result.micros >= 1000000)
	{
		result.micros -= 1000000;
		result.seconds++;
	}

	return result;
}


Time Time::operator-(const Time &arg) const
{
	Time result;
	result.seconds = seconds - arg.seconds;
	result.micros = micros - arg.micros;
	if (result.micros < 0)
	{
		result.micros += 1000000;
		result.seconds--;
	}

	return result;	

}

const Time& Time::operator=(float value)
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