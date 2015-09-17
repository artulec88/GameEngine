#include "stdafx.h"
#include "Time.h"
#include <Windows.h>
#include <stdio.h>
#include <sstream>
#include <limits>
#include "ILogger.h"

using namespace Utility::Timing;
using namespace std;

/* ==================== TimeSpan class begin ==================== */
bool TimeSpan::operator<(const TimeSpan &timeSpan) const
{
	CRITICAL_LOG("The function is not yet implemented");
	if (m_unit == timeSpan.GetUnit())
	{
		return m_value < timeSpan.GetValue();
	}
	// TODO: Finish the function
	return true;
}

bool TimeSpan::operator>(const TimeSpan &timeSpan) const
{
	CRITICAL_LOG("The function is not yet implemented");
	if (m_unit == timeSpan.GetUnit())
	{
		return m_value > timeSpan.GetValue();
	}
	// TODO: Finish the function
	return true;
}

std::string TimeSpan::ToString() const
{
	CRITICAL_LOG("The functions is not yet implemented");
	return "";
}
/* ==================== TimeSpan class end ==================== */

/* ==================== Time class begin ==================== */
/* static */ Time Time::Now()
{
	SYSTEMTIME st;
	GetSystemTime(&st);
	return Time(st.wSecond, st.wMilliseconds);
}

/* static */ std::string Time::ConvertTimeUnitToString(TimeUnit timeUnit)
{
	switch (timeUnit)
	{
	case SECOND:
		return "[s]";
	case MILLISECOND:
		return "[ms]";
	case MICROSECOND:
		return "[us]";
	case NANOSECOND:
		return "[ns]";
	default:
		ERROR_LOG("Unknown time unit (%d)", timeUnit);
		return "Unknown time unit";
	}
}

std::string Time::ToDateString(const char *format /* = "%Y-%m-%d %H:%M:%S" */) const
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

	time_t rawtime = m_seconds;
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
/* ==================== Time class end ==================== */

/* ==================== Timer class begin ==================== */
/* static */ bool Timer::isFrequencyInitialized = false;
/* static */ LARGE_INTEGER Timer::frequency;
/* static */ LARGE_INTEGER Timer::GetFrequency()
{
	if (!Timer::isFrequencyInitialized)
	{
		QueryPerformanceFrequency(&Timer::frequency);
		Timer::isFrequencyInitialized = true;
	}
	return Timer::frequency;
}

Timer::Timer() :
	m_isRunning(false),
	m_startTime(),
	m_stopTime()
{
}

Timer::~Timer()
{
}

TimeSpan Timer::ToReal() const
{
	static int ONE_THOUSAND = 1000;
	static int ONE_MILLION = 1000 * ONE_THOUSAND;
	static int ONE_BILLION = 1000 * ONE_MILLION;

	float elapsedTimeInSeconds = CalculateElapsedTimeInSeconds();
	if (elapsedTimeInSeconds > 1)
	{
		return TimeSpan(elapsedTimeInSeconds, TimeUnit::SECOND);
	}
	else if (elapsedTimeInSeconds > 0.001)
	{
		return TimeSpan(elapsedTimeInSeconds * ONE_THOUSAND, TimeUnit::MILLISECOND);
	}
	else if (elapsedTimeInSeconds > 0.000001)
	{
		return TimeSpan(elapsedTimeInSeconds * ONE_MILLION, TimeUnit::MICROSECOND);
	}
	return TimeSpan(elapsedTimeInSeconds * ONE_BILLION, TimeUnit::NANOSECOND);
}

TimeSpan Timer::ToReal(TimeUnit timeUnit) const
{
	float elapsedTimeInSeconds = CalculateElapsedTimeInSeconds();
	const int timeUnitConverter = pow(10, static_cast<float>(timeUnit - TimeUnit::SECOND));
	return TimeSpan(elapsedTimeInSeconds * timeUnitConverter, timeUnit);
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
		WARNING_LOG("Stopping the timer which is already stopped.");
		return;
	}
	QueryPerformanceCounter(&m_stopTime);
	m_isRunning = false;
}
