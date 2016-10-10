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
	case TIME_UNITS_COUNT:
	default:
		ERROR_LOG_UTILITY("Unknown time unit: ", timeUnit);
		return "[unknown time unit]";
	}
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
	// TODO: Format parameter not used.
	std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::time_point_cast<std::chrono::seconds>(m_timePoint));
	std::stringstream ss("");
	ss << std::put_time(std::localtime(&time), "%F %T");
	return ss.str();
}
/* ==================== DateTime class end ==================== */

/* ==================== Timer class begin ==================== */
Utility::Timing::Timer::Timer() :
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
	//CHECK_CONDITION_RETURN_VOID_UTILITY(!m_isRunning, Logging::ERR, "Starting the timer which is already running");
	m_startTime = std::chrono::system_clock::now();
}

void Utility::Timing::Timer::Reset()
{
	Start();
}

void Utility::Timing::Timer::Stop()
{
	//CHECK_CONDITION_RETURN_VOID_UTILITY(m_isRunning, Logging::ERR, "Stopping the timer which is already stopped.");
	m_stopTime = std::chrono::system_clock::now();
}
