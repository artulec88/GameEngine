#include "stdafx.h"
#include "Time.h"
#include "ILogger.h"
#include <stdio.h>
#include <sstream>
#include <limits>
#include <math.h>
#include "ILogger.h"

/* ==================== TimeSpan class begin ==================== */
void Utility::Timing::TimeSpan::AdjustUnitToValue()
{
	// This function should make the m_value and m_unit variables easier to read, i.e.
	// if m_value=123456,789 and m_unit=us then we should change m_value to 123,456789 and m_unit=ms.
	WARNING_LOG_UTILITY("The function has not been tested yet.");

	if ((m_value < Time::ONE_THOUSAND) && (m_value > Time::ONE)) // If value is in range [1; 1000) then we don't need to change the unit.
	{
		return;
	}
	if ((m_value > Time::ONE) && (m_unit == SECOND)) // If value is > 1.0 and we have the biggest unit already then there is nothing we can do.
	{
		return;
	}
	if ((m_value < Time::ONE) && (m_unit == NANOSECOND)) // If value is < 1.0 and we have the lowest unit already then there is nothing we can do.
	{
		return;
	}
	
	/* ==================== Calculating the new unit begin ==================== */
	int unitOffset;
	if (m_value >= Time::ONE_BILLION) { unitOffset = 3; }
	else if (m_value >= Time::ONE_MILLION) { unitOffset = 2; }
	else if (m_value >= Time::ONE_THOUSAND) { unitOffset = 1; }
	else if (m_value >= Time::ONE_OVER_THOUSAND) { unitOffset = -1; }
	else if (m_value >= Time::ONE_OVER_MILLION) { unitOffset = -2; }
	else if (m_value >= Time::ONE_OVER_BILLION) { unitOffset = -3; }
	else { unitOffset = -4; }
	//TimeUnit newUnit = min(min(m_unit - unitOffset, NANOSECOND), 0);
	int newUnit = m_unit - unitOffset;
	if (newUnit > NANOSECOND)
	{
		newUnit = NANOSECOND;
	}
	else if (newUnit < SECOND)
	{
		newUnit = SECOND;
	}
	/* ==================== Calculating the new unit end ==================== */

	/* ==================== Calculating the new value begin ==================== */
	float newValueConversionFactor = pow(10.0f, 3.0f * (newUnit - m_unit));
	m_value *= newValueConversionFactor;
	/* ==================== Calculating the new value end ==================== */
	m_unit = static_cast<TimeUnit>(newUnit);
}

Utility::Timing::TimeSpan& Utility::Timing::TimeSpan::operator/=(int s)
{
	m_value /= s;
	return (*this);
}

void Utility::Timing::TimeSpan::operator=(const TimeSpan& timeSpan)
{
	m_value = timeSpan.GetValue();
	m_unit = timeSpan.GetUnit();
}

bool Utility::Timing::TimeSpan::operator<(const TimeSpan &timeSpan) const
{
	//WARNING_LOG_UTILITY("The function has not been tested yet.");
	if (m_unit == timeSpan.GetUnit())
	{
		return m_value < timeSpan.GetValue();
	}
	float timeUnitConvertingFactor = Time::TimeUnitConvertingFactor(m_unit, timeSpan.GetUnit());
	return (m_value * timeUnitConvertingFactor) < timeSpan.GetValue();
}

bool Utility::Timing::TimeSpan::operator>(const TimeSpan &timeSpan) const
{
	//WARNING_LOG_UTILITY("The function has not been tested yet");
	if (m_unit == timeSpan.GetUnit())
	{
		return m_value > timeSpan.GetValue();
	}
	float timeUnitConvertingFactor = Time::TimeUnitConvertingFactor(m_unit, timeSpan.GetUnit());
	return (m_value * timeUnitConvertingFactor) > timeSpan.GetValue();
}

std::string Utility::Timing::TimeSpan::ToString() const
{
	std::stringstream ss("");
	ss << m_value << " " << Time::ConvertTimeUnitToString(m_unit);
	return ss.str();
}
/* ==================== TimeSpan class end ==================== */

/* ==================== Time class begin ==================== */
/* static */ const float Utility::Timing::Time::ONE_OVER_BILLION = 0.000000001f;
/* static */ const float Utility::Timing::Time::ONE_OVER_MILLION = 0.000001f;
/* static */ const float Utility::Timing::Time::ONE_OVER_THOUSAND = 0.001f;
/* static */ const float Utility::Timing::Time::ONE = 1.0f;
/* static */ const float Utility::Timing::Time::ONE_THOUSAND = 1000.0f;
/* static */ const float Utility::Timing::Time::ONE_MILLION = 1000000.0f;
/* static */ const float Utility::Timing::Time::ONE_BILLION = 1000000000.0f;
/* static */ const int Utility::Timing::Time::SECONDS_PER_MINUTE = 60;
/* static */ const int Utility::Timing::Time::SECONDS_PER_HOUR = 3600;
/* static */ const int Utility::Timing::Time::SECONDS_PER_DAY = 86400;
/* static */ const int Utility::Timing::Time::DAYS_PER_YEAR = 365;

/* static */ Utility::Timing::Time Utility::Timing::Time::Now()
{
	SYSTEMTIME st;
	GetSystemTime(&st);
	return Time(st.wSecond, st.wMilliseconds);
}

/* static */ std::string Utility::Timing::Time::ConvertTimeUnitToString(TimeUnit timeUnit)
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
		ERROR_LOG_UTILITY("Unknown time unit: ", timeUnit);
		return "Unknown time unit";
	}
}

/* static */ float Utility::Timing::Time::TimeUnitConvertingFactor(TimeUnit fromTimeUnit, TimeUnit toTimeUnit)
{
	int timeUnitDiff = toTimeUnit - fromTimeUnit;
	switch (timeUnitDiff)
	{
	case -3:
		return ONE_OVER_BILLION;
	case -2:
		return ONE_OVER_MILLION;
	case -1:
		return ONE_OVER_THOUSAND;
	case 0:
		return ONE;
	case 1:
		return ONE_THOUSAND;
	case 2:
		return ONE_MILLION;
	case 3:
		return ONE_BILLION;
	default:
		ERROR_LOG_UTILITY("Failed determining the time unit conversion factor (fromTimeUnit = ", fromTimeUnit, "; toTimeUnit = ", toTimeUnit, ")");
		return ONE;
	}
}

std::string Utility::Timing::Time::ToDateString(const char *format /* = "%Y-%m-%d %H:%M:%S" */) const
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
	return std::string(buffer);
}
/* ==================== Time class end ==================== */

/* ==================== Timer class begin ==================== */
/* static */ bool Utility::Timing::Timer::isFrequencyInitialized = false;
/* static */ LARGE_INTEGER Utility::Timing::Timer::frequency;
/* static */ LARGE_INTEGER Utility::Timing::Timer::GetFrequency()
{
	if (!Timer::isFrequencyInitialized)
	{
		QueryPerformanceFrequency(&Timer::frequency);
		Timer::isFrequencyInitialized = true;
	}
	return Timer::frequency;
}

Utility::Timing::Timer::Timer() :
	m_isRunning(false),
	m_startTime(),
	m_stopTime()
{
}

Utility::Timing::Timer::~Timer()
{
}

Utility::Timing::TimeSpan Utility::Timing::Timer::GetTimeSpan() const
{
	float elapsedTimeInMillieconds = CalculateElapsedTimeInMilliseconds();
	if (elapsedTimeInMillieconds > Time::ONE_THOUSAND)
	{
		return TimeSpan(elapsedTimeInMillieconds * Time::ONE_OVER_THOUSAND, SECOND);
	}
	else if (elapsedTimeInMillieconds > Time::ONE)
	{
		return TimeSpan(elapsedTimeInMillieconds, MILLISECOND);
	}
	else if (elapsedTimeInMillieconds > Time::ONE_OVER_THOUSAND)
	{
		return TimeSpan(elapsedTimeInMillieconds * Time::ONE_THOUSAND, MICROSECOND);
	}
	return TimeSpan(elapsedTimeInMillieconds * Time::ONE_MILLION, NANOSECOND);
}

Utility::Timing::TimeSpan Utility::Timing::Timer::GetTimeSpan(TimeUnit timeUnit) const
{
	float elapsedTimeInMilliseconds = CalculateElapsedTimeInMilliseconds();
	//INFO_LOG_UTILITY("Elapsed time in seconds = ", elapsedTimeInMilliseconds);
	return TimeSpan(elapsedTimeInMilliseconds * Time::TimeUnitConvertingFactor(MILLISECOND, timeUnit), timeUnit);
}

void Utility::Timing::Timer::Start()
{
	//if (m_isRunning)
	//{
	//	WARNING_LOG_UTILITY("Starting the timer which is already running");
	//	return;
	//}
	QueryPerformanceCounter(&m_startTime);
	m_isRunning = true;
}

void Utility::Timing::Timer::Reset()
{
	Start();
}

void Utility::Timing::Timer::Stop()
{
	//if (!m_isRunning)
	//{
	//	WARNING_LOG_UTILITY("Stopping the timer which is already stopped.");
	//	return;
	//}
	QueryPerformanceCounter(&m_stopTime);
	m_isRunning = false;
}
