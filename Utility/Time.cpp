#include "stdafx.h"
#include "Time.h"
#include "ILogger.h"
#include <stdio.h>
#include <sstream>
#include <limits>
#include <math.h>
#include "ILogger.h"

#include <ctime>
#include <iomanip>

/* ==================== DateTime class begin ==================== */
/* static */ std::array<int, Utility::Timing::DateTime::MONTHS_COUNT> Utility::Timing::DateTime::DAYS_COUNT_IN_MONTH = {
	31, 28 /* We don't account for "przestêpny" years */, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
/* static */ std::array<int, Utility::Timing::DateTime::MONTHS_COUNT> Utility::Timing::DateTime::DAYS_COUNT_FROM_FIRST_DAY_IN_YEAR = {
	0,
	DAYS_COUNT_IN_MONTH[0],
	DAYS_COUNT_IN_MONTH[0] + DAYS_COUNT_IN_MONTH[1],
	DAYS_COUNT_IN_MONTH[0] + DAYS_COUNT_IN_MONTH[1] + DAYS_COUNT_IN_MONTH[2],
	DAYS_COUNT_IN_MONTH[0] + DAYS_COUNT_IN_MONTH[1] + DAYS_COUNT_IN_MONTH[2] + DAYS_COUNT_IN_MONTH[3],
	DAYS_COUNT_IN_MONTH[0] + DAYS_COUNT_IN_MONTH[1] + DAYS_COUNT_IN_MONTH[2] + DAYS_COUNT_IN_MONTH[3] + DAYS_COUNT_IN_MONTH[4],
	DAYS_COUNT_IN_MONTH[0] + DAYS_COUNT_IN_MONTH[1] + DAYS_COUNT_IN_MONTH[2] + DAYS_COUNT_IN_MONTH[3] + DAYS_COUNT_IN_MONTH[4] + DAYS_COUNT_IN_MONTH[5],
	DAYS_COUNT_IN_MONTH[0] + DAYS_COUNT_IN_MONTH[1] + DAYS_COUNT_IN_MONTH[2] + DAYS_COUNT_IN_MONTH[3] + DAYS_COUNT_IN_MONTH[4] + DAYS_COUNT_IN_MONTH[5] + DAYS_COUNT_IN_MONTH[6],
	DAYS_COUNT_IN_MONTH[0] + DAYS_COUNT_IN_MONTH[1] + DAYS_COUNT_IN_MONTH[2] + DAYS_COUNT_IN_MONTH[3] + DAYS_COUNT_IN_MONTH[4] + DAYS_COUNT_IN_MONTH[5] + DAYS_COUNT_IN_MONTH[6] + DAYS_COUNT_IN_MONTH[7],
	DAYS_COUNT_IN_MONTH[0] + DAYS_COUNT_IN_MONTH[1] + DAYS_COUNT_IN_MONTH[2] + DAYS_COUNT_IN_MONTH[3] + DAYS_COUNT_IN_MONTH[4] + DAYS_COUNT_IN_MONTH[5] + DAYS_COUNT_IN_MONTH[6] + DAYS_COUNT_IN_MONTH[7] + DAYS_COUNT_IN_MONTH[8],
	DAYS_COUNT_IN_MONTH[0] + DAYS_COUNT_IN_MONTH[1] + DAYS_COUNT_IN_MONTH[2] + DAYS_COUNT_IN_MONTH[3] + DAYS_COUNT_IN_MONTH[4] + DAYS_COUNT_IN_MONTH[5] + DAYS_COUNT_IN_MONTH[6] + DAYS_COUNT_IN_MONTH[7] + DAYS_COUNT_IN_MONTH[8] + DAYS_COUNT_IN_MONTH[9],
	DAYS_COUNT_IN_MONTH[0] + DAYS_COUNT_IN_MONTH[1] + DAYS_COUNT_IN_MONTH[2] + DAYS_COUNT_IN_MONTH[3] + DAYS_COUNT_IN_MONTH[4] + DAYS_COUNT_IN_MONTH[5] + DAYS_COUNT_IN_MONTH[6] + DAYS_COUNT_IN_MONTH[7] + DAYS_COUNT_IN_MONTH[8] + DAYS_COUNT_IN_MONTH[9] + DAYS_COUNT_IN_MONTH[10],
};
/* static */ std::array<std::array<float, Utility::Timing::TIME_UNITS_COUNT>, Utility::Timing::TIME_UNITS_COUNT> Utility::Timing::DateTime::TIME_UNITS_CONVERSION_FACTORS = { {
	{ 1.0f, // hour to hour conversion
	static_cast<float>(MINUTES_PER_HOUR), // hour to minutes conversion
	static_cast<float>(SECONDS_PER_HOUR), // hour to seconds conversion
	static_cast<float>(SECONDS_PER_HOUR * MILLISECONDS_PER_SECOND), // hour to milliseconds
	static_cast<float>(static_cast<long long>(SECONDS_PER_HOUR) * MILLISECONDS_PER_SECOND * MICROSECONDS_PER_MILLISECOND), // hour to microseconds
	static_cast<float>(static_cast<long long>(SECONDS_PER_HOUR) * MILLISECONDS_PER_SECOND * MICROSECONDS_PER_MILLISECOND * NANOSECONDS_PER_MICROSECOND) }, // hour to nanoseconds
	{ 1.0f / MINUTES_PER_HOUR, // minute to hours conversion
	1.0f, // minute to minute conversion
	static_cast<float>(SECONDS_PER_MINUTE), // minute to seconds
	static_cast<float>(SECONDS_PER_MINUTE * MILLISECONDS_PER_SECOND), // minute to milliseconds
	static_cast<float>(SECONDS_PER_MINUTE * MILLISECONDS_PER_SECOND * MICROSECONDS_PER_MILLISECOND), // minute to microseconds
	static_cast<float>(static_cast<long long>(SECONDS_PER_MINUTE) * MILLISECONDS_PER_SECOND * MICROSECONDS_PER_MILLISECOND * NANOSECONDS_PER_MICROSECOND) }, // Minute to nanoseconds
	{ 1.0f / SECONDS_PER_HOUR, 1.0f / SECONDS_PER_MINUTE, 1.0f, static_cast<float>(MILLISECONDS_PER_SECOND), static_cast<float>(MILLISECONDS_PER_SECOND * MICROSECONDS_PER_MILLISECOND), static_cast<float>(MILLISECONDS_PER_SECOND * MICROSECONDS_PER_MILLISECOND * NANOSECONDS_PER_MICROSECOND) }, // Second
	{ 1.0f / (SECONDS_PER_HOUR * MILLISECONDS_PER_SECOND), 1.0f / (SECONDS_PER_MINUTE * MILLISECONDS_PER_SECOND), 1.0f / MILLISECONDS_PER_SECOND, 1.0f, static_cast<float>(MICROSECONDS_PER_MILLISECOND), static_cast<float>(MICROSECONDS_PER_MILLISECOND * NANOSECONDS_PER_MICROSECOND) }, // Millisecond
	{ 1.0f / (static_cast<long long>(SECONDS_PER_HOUR) * MILLISECONDS_PER_SECOND * MICROSECONDS_PER_MILLISECOND), 1.0f / (SECONDS_PER_MINUTE * MILLISECONDS_PER_SECOND * MICROSECONDS_PER_MILLISECOND), 1.0f / (MILLISECONDS_PER_SECOND * MICROSECONDS_PER_MILLISECOND), 1.0f / MICROSECONDS_PER_MILLISECOND, 1.0f, static_cast<float>(NANOSECONDS_PER_MICROSECOND) }, // Microsecond
	{ 1.0f / (static_cast<long long>(SECONDS_PER_HOUR) * MILLISECONDS_PER_SECOND * MICROSECONDS_PER_MILLISECOND * NANOSECONDS_PER_MICROSECOND), 1.0f / (static_cast<long long>(SECONDS_PER_MINUTE) * MILLISECONDS_PER_SECOND * MICROSECONDS_PER_MILLISECOND * NANOSECONDS_PER_MICROSECOND), 1.0f / (MILLISECONDS_PER_SECOND * MICROSECONDS_PER_MILLISECOND * NANOSECONDS_PER_MICROSECOND), 1.0f / MICROSECONDS_PER_MILLISECOND * NANOSECONDS_PER_MICROSECOND, 1.0f / NANOSECONDS_PER_MICROSECOND, 1.0f } // Nanosecond
	} };

/* static */ Utility::Timing::DateTime Utility::Timing::DateTime::Now()
{
	/* ==================== Calculate local time #1 begin ==================== */
	//SYSTEMTIME st; // See https://msdn.microsoft.com/pl-pl/library/windows/desktop/ms724950(v=vs.85).aspx
	//GetSystemTime(&st);
	//return DateTime(st.wYear, st.wMonth - 1 /* because SYSTEMTIME counts months from 1 */, st.wDay - 1 /* because SYSTEMTIME counts days from 1 */, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	return DateTime(std::chrono::system_clock::now());
	/* ==================== Calculate local time #1 end ==================== */

	/* ==================== Calculate local time #2 begin ==================== */
	//time_t rawtime;
	//struct tm timeinfo;
	//time(&rawtime);
	//localtime_s(&timeinfo, &rawtime);
	//return DateTime(timeinfo.tm_year, static_cast<Month>(timeinfo.tm_mon), timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec, 0);
	/* ==================== Calculate local time #2 end ==================== */
}

/* static */ std::string Utility::Timing::DateTime::ConvertTimeUnitToString(TimeUnit timeUnit)
{
	switch (timeUnit)
	{
	case HOUR: return "[h]";
	case MINUTE: return "[m]";
	case SECOND: return "[s]";
	case MILLISECOND: return "[ms]";
	case MICROSECOND: return "[us]";
	case NANOSECOND: return "[ns]";
	default:
		ERROR_LOG_UTILITY("Unknown time unit: ", timeUnit);
		return "[unknown time unit]";
	}
}

/* static */ float Utility::Timing::DateTime::GetTimeUnitConvertingFactor(TimeUnit fromTimeUnit, TimeUnit toTimeUnit)
{
	return TIME_UNITS_CONVERSION_FACTORS[fromTimeUnit][toTimeUnit];
}

Utility::Timing::DateTime::DateTime(unsigned int year, unsigned int month, unsigned int day, unsigned int hour, unsigned int minute, unsigned int second) :
	m_timePoint()
{
	CHECK_CONDITION_UTILITY(month > 0 && month <= MONTHS_COUNT, Utility::Logging::ERR, "Incorrect month number specified: ", month, ". The month number must lie in range [1; 12].");
	CHECK_CONDITION_UTILITY(day > 0 && day <= DAYS_COUNT_IN_MONTH[month], Utility::Logging::ERR, "Incorrect day number specified: ", day,
		". The day number must lie in range [1; ", DAYS_COUNT_IN_MONTH[month], "].");
	CHECK_CONDITION_UTILITY(hour >= 0 && hour < HOURS_PER_DAY, Utility::Logging::ERR, "Incorrect hour number specified: ", hour, ". The hour number must lie in range [0; 24).");
	CHECK_CONDITION_UTILITY(minute >= 0 && minute < MINUTES_PER_HOUR, Utility::Logging::ERR, "Incorrect minute number specified: ", minute, ". The minute number must lie in range [0; 60).");
	CHECK_CONDITION_UTILITY(second >= 0 && second < SECONDS_PER_MINUTE, Utility::Logging::ERR, "Incorrect second number specified: ", second, ". The second number must lie in range [0; 60).");
	std::stringstream ss("");// Jan 9 2014 12:35 : 34");
	ss << year << "-" << month << "-" << day << " " << hour << ":" << minute << ":" << second;
	std::tm tm = {};
	ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
	m_timePoint = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::from_time_t(std::mktime(&tm)));
}

Utility::Timing::DateTime::DateTime(const std::chrono::system_clock::time_point& timePoint) :
	m_timePoint(std::chrono::time_point_cast<std::chrono::seconds>(timePoint))
{
}

Utility::Timing::DateTime::~DateTime()
{
}

Utility::Timing::DateTime::DateTime(const DateTime& dateTime) :
	m_timePoint(dateTime.m_timePoint)
{
}

Utility::Timing::DateTime::DateTime(DateTime&& dateTime) :
	m_timePoint(std::move(dateTime.m_timePoint))
{
}

Utility::Timing::DateTime& Utility::Timing::DateTime::operator=(const DateTime& dateTime)
{
	m_timePoint = dateTime.m_timePoint;
	return *this;
}

Utility::Timing::DateTime& Utility::Timing::DateTime::operator=(DateTime&& dateTime)
{
	m_timePoint = std::move(dateTime.m_timePoint);
	return *this;
}

bool Utility::Timing::DateTime::operator==(const DateTime& dateTime) const
{
	return m_timePoint == dateTime.m_timePoint;
}

bool Utility::Timing::DateTime::operator!=(const DateTime& dateTime) const
{
	return !(operator==(dateTime));
}

bool Utility::Timing::DateTime::operator<(const DateTime& dateTime) const
{
	return m_timePoint < dateTime.m_timePoint;
}

bool Utility::Timing::DateTime::operator>(const DateTime& dateTime) const
{
	return m_timePoint > dateTime.m_timePoint;
}

//int Utility::Timing::DateTime::GetDayInYear() const
//{
//	//CRITICAL_LOG_UTILITY("Day in year: ", DAYS_COUNT_FROM_FIRST_DAY_IN_YEAR[m_month] + m_day);
//	return DAYS_COUNT_FROM_FIRST_DAY_IN_YEAR[m_month] + m_day;
//}
//
//int Utility::Timing::DateTime::GetDayTime() const
//{
//	return m_hour * SECONDS_PER_HOUR + m_minute * SECONDS_PER_MINUTE + m_second + m_millisecond * MILLISECONDS_PER_SECOND;
//}

std::string Utility::Timing::DateTime::ToString(const char *format /* = "%Y-%m-%d %H:%M:%S" */) const
{
	std::time_t time = std::chrono::system_clock::to_time_t(m_timePoint);
	std::stringstream ss("");
	ss << std::put_time(std::localtime(&time), "%F %T");
	return ss.str();
}
/* ==================== DateTime class end ==================== */

/* ==================== Timer class begin ==================== */
Utility::Timing::Timer::Timer() :
	m_isRunning(false),
	m_startTime(),
	m_stopTime()
{
}

Utility::Timing::Timer::~Timer()
{
}

//long long Utility::Timing::Timer::GetDuration(TimeUnit timeUnit) const
//{
//	switch (timeUnit)
//	{
//	case HOUR:
//		return std::chrono::duration_cast<std::chrono::hours>(m_stopTime - m_startTime).count();
//	case MINUTE:
//		return std::chrono::duration_cast<std::chrono::minutes>(m_stopTime - m_startTime).count();
//	case SECOND:
//		return std::chrono::duration_cast<std::chrono::seconds>(m_stopTime - m_startTime).count();
//	case MILLISECOND:
//		return std::chrono::duration_cast<std::chrono::milliseconds>(m_stopTime - m_startTime).count();
//	case MICROSECOND:
//		return std::chrono::duration_cast<std::chrono::microseconds>(m_stopTime - m_startTime).count();
//	case NANOSECOND:
//		return std::chrono::duration_cast<std::chrono::nanoseconds>(m_stopTime - m_startTime).count();
//	default:
//		ERROR_LOG_UTILITY("Cannot get correct time duration. Incorrect time unit specified: ", timeUnit);
//		return std::chrono::duration_cast<std::chrono::milliseconds>(m_stopTime - m_startTime).count();
//	}
//}

void Utility::Timing::Timer::Start()
{
	CHECK_CONDITION_RETURN_VOID_ALWAYS_UTILITY(!m_isRunning, Logging::ERR, "Starting the timer which is already running");
	m_startTime = std::chrono::system_clock::now();
	m_isRunning = true;
}

void Utility::Timing::Timer::Reset()
{
	Start();
}

void Utility::Timing::Timer::Stop()
{
	CHECK_CONDITION_RETURN_VOID_ALWAYS_UTILITY(m_isRunning, Logging::ERR, "Stopping the timer which is already stopped.");
	m_stopTime = std::chrono::system_clock::now();
	m_isRunning = false;
}
