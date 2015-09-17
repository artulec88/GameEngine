#include "stdafx.h"
#include "Time.h"
#include <Windows.h>
#include <stdio.h>
#include <sstream>
#include <limits>

using namespace Utility::Timing;
using namespace std;

/* ==================== Time<T> class end ==================== */
template UTILITY_API class Time<float>;
template UTILITY_API class Time<double>;
/* ==================== Time<T> class end ==================== */

/* ==================== Timer class begin ==================== */
/* static */ LARGE_INTEGER Time::FREQUENCY = 0;
/* static */ LARGE_INTEGER Time::GetFrequency()
{
	if (Time::FREQUENCY == 0)
	{
		QueryPerformanceFrequency(&Time::FREQUENCY);
	}
	return Time::FREQUENCY;
}

Timer::Timer() :
	m_isRunning(false),
	m_startTime(0),
	m_stopTime(0)
{
}

Timer::~Timer()
{
}

Time Timer::ToReal() const
{
	static int ONE_THOUSAND = 1000;
	static int ONE_MILLION = 1000 * ONE_THOUSAND;
	static int ONE_BILLION = 1000 * ONE_MILION;

	float elapsedTimeInSeconds = CalculateElapsedTimeInSeconds();
	if (elapsedTimeInSeconds > 1)
	{
		return Time(elapsedTimeInSeconds, TimeUnit::SECOND);
	}
	else if (elapsedTimeInSeconds > 0.001)
	{
		return Time(elapsedTimeInSeconds * ONE_THOUSAND, TimeUnit::MILISECOND);
	}
	else if (elapsedTimeInSeconds > 0.000001)
	{
		return Time(elapsedTimeInSeconds * ONE_MILION, TimeUnit::MICROSECOND);
	}
	return Time(elapsedTimeInSeconds * ONE_BILLION, TimeUnit::NANOSECOND);
}

Time Timer::ToReal(TimeUnit timeUnit) const
{
	float elapsedTimeInSeconds = CalculateElapsedTimeInSeconds();
	const int timeUnitConverter = pow(10, timeUnit - TimeUnit::SECOND);
	return Time(elapsedTimeInSeconds * timeUnitConverter, timeUnit);
}

void Timer::Start()
{
	QueryPerformanceCounter(&m_startTime);
	m_isRunning = true;
}

void Timer::Reset()
{
	Start();
}

void Timer::Stop()
{
	if (!m_isRunning)
	{
		// TODO: WARNING_LOG
		return;
	}
	QueryPerformanceCounter(&m_stopTime);
	m_isRunning = false;
}

std::string Timer::ToString() const
{
	// TODO: Finish the function
	return "";
}

std::string Timer::ToDateString(const char *format /* = "%Y-%m-%d %H:%M:%S" */) const
{
	// TODO: Finish the function
	return "";
}

//std::string Timer::ToString() const
//{
//	//char buffer[30];
//	//sprintf(buffer, "%d:%06d", seconds, micros);
//	//return std::string(buffer);
//
//	std::stringstream ss("");
//	ss << seconds;
//	ss << ":";
//	ss << micros;
//	return ss.str();
//}

//std::string Timer::ToDateString(const char *format) const
//{
//	//char buffer[80];
//	//memset(buffer, 0, 80);
//	//time_t t = seconds;
//	//int result = strftime(buffer, 79, format, localtime(&t));
//	//if (result == 0)
//	//{
//	//	return std::string("time-error");
//	//}
//	//buffer[result] = '\0';
//	//return string(buffer);
//
//	time_t rawtime = seconds;
//	struct tm timeinfo;
//	char buffer [80];
//
//	time (&rawtime);
//	localtime_s(&timeinfo, &rawtime);
//
//	if ( (strftime (buffer, 80, format, &timeinfo)) == 0)
//	{
//		return std::string("time-error");
//	}
//	return string(buffer);
//}