#include "stdafx.h"
#include "Time.h"
#include "TimeSpan.h"
#include "ILogger.h"

#include <sstream>
#include <ctime>
#include <iomanip>

/* ==================== DateTime class begin ==================== */
/* static */ std::array<int, utility::timing::DateTime::MONTHS_COUNT> utility::timing::DateTime::DAYS_COUNT_IN_MONTH = {
	31, 28 /* We don't account for "przestêpny" years */, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
/* static */ std::array<int, utility::timing::DateTime::MONTHS_COUNT> utility::timing::DateTime::DAYS_COUNT_FROM_FIRST_DAY_IN_YEAR = {
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

/* static */ utility::timing::DateTime utility::timing::DateTime::Now()
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

/* static */ std::string utility::timing::DateTime::ConvertTimeUnitToString(TimeUnit timeUnit)
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

utility::timing::DateTime::DateTime(const unsigned int year, const unsigned int month, const unsigned int day, const unsigned int hour, const unsigned int minute, const unsigned int second) :
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
	tm timePoint = {};
	ss >> std::get_time(&timePoint, "%Y-%m-%d %H:%M:%S");
	m_timePoint = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::from_time_t(mktime(&timePoint)));
}

utility::timing::DateTime::DateTime(const std::chrono::system_clock::time_point& timePoint) :
	m_timePoint(std::chrono::time_point_cast<std::chrono::seconds>(timePoint))
{
}

utility::timing::DateTime::~DateTime()
{
}

utility::timing::DateTime::DateTime(const DateTime& dateTime) :
	m_timePoint(dateTime.m_timePoint)
{
}

utility::timing::DateTime::DateTime(DateTime&& dateTime) noexcept :
	m_timePoint(move(dateTime.m_timePoint))
{
}

utility::timing::DateTime& utility::timing::DateTime::operator=(const DateTime& dateTime)
{
	m_timePoint = dateTime.m_timePoint;
	return *this;
}

utility::timing::DateTime& utility::timing::DateTime::operator=(DateTime&& dateTime) noexcept
{
	m_timePoint = move(dateTime.m_timePoint);
	return *this;
}

utility::timing::DateTime& utility::timing::DateTime::operator+=(const TimeSpan& timeSpan)
{
	m_timePoint += std::chrono::nanoseconds(timeSpan.GetValue());
	//m_timePoint += timeSpan.GetDuration();
	return *this;
}

utility::timing::DateTime& utility::timing::DateTime::operator-=(const TimeSpan& timeSpan)
{
	m_timePoint -= std::chrono::nanoseconds(timeSpan.GetValue());
	return *this;
}

bool utility::timing::DateTime::operator==(const DateTime& dateTime) const
{
	return m_timePoint == dateTime.m_timePoint;
}

bool utility::timing::DateTime::operator!=(const DateTime& dateTime) const
{
	return !operator==(dateTime);
}

bool utility::timing::DateTime::operator<(const DateTime& dateTime) const
{
	return m_timePoint < dateTime.m_timePoint;
}

bool utility::timing::DateTime::operator>(const DateTime& dateTime) const
{
	return m_timePoint > dateTime.m_timePoint;
}

//int Utility::timing::DateTime::GetDayInYear() const
//{
//	//CRITICAL_LOG_UTILITY("Day in year: ", DAYS_COUNT_FROM_FIRST_DAY_IN_YEAR[m_month] + m_day);
//	return DAYS_COUNT_FROM_FIRST_DAY_IN_YEAR[m_month] + m_day;
//}
//
//int Utility::timing::DateTime::GetDayTime() const
//{
//	return m_hour * SECONDS_PER_HOUR + m_minute * SECONDS_PER_MINUTE + m_second + m_millisecond * MILLISECONDS_PER_SECOND;
//}

std::string utility::timing::DateTime::ToString(const char *format /* = "%Y-%m-%d %H:%M:%S" */) const
{
	// TODO: Format parameter not used.
	auto time = std::chrono::system_clock::to_time_t(std::chrono::time_point_cast<std::chrono::seconds>(m_timePoint));
	std::stringstream ss("");
	ss << std::put_time(localtime(&time), "%F %T");
	return ss.str();
}
/* ==================== DateTime class end ==================== */

/* ==================== Timer class begin ==================== */
utility::timing::Timer::Timer() :
	m_startTime(),
	m_stopTime()
{
}

utility::timing::Timer::~Timer()
{
}

utility::timing::TimeSpan utility::timing::Timer::GetTimeSpan() const
{
	return TimeSpan(std::chrono::duration_cast<std::chrono::nanoseconds>(m_stopTime - m_startTime));
}

//long long Utility::timing::Timer::GetDuration(TimeUnit timeUnit) const
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

void utility::timing::Timer::Start()
{
	//CHECK_CONDITION_RETURN_VOID_UTILITY(!m_isRunning, Logging::ERR, "Starting the timer which is already running");
	m_startTime = std::chrono::system_clock::now();
}

void utility::timing::Timer::Reset()
{
	Start();
}

void utility::timing::Timer::Stop()
{
	//CHECK_CONDITION_RETURN_VOID_UTILITY(m_isRunning, Logging::ERR, "Stopping the timer which is already stopped.");
	m_stopTime = std::chrono::system_clock::now();
}

std::string utility::timing::Timer::ToString(const TimeUnit timeUnit /* = NANOSECOND */) const
{
	std::stringstream ss("");
	ss << GetDuration(timeUnit) << " " << DateTime::ConvertTimeUnitToString(timeUnit);
	return ss.str();
}