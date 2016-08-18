#include "stdafx.h"
#include "Time.h"
#include "ILogger.h"
#include <stdio.h>
#include <sstream>
#include <limits>
#include <math.h>
#include "ILogger.h"

/* ==================== TimeSpan class begin ==================== */
Utility::Timing::TimeSpan::TimeSpan() :
	m_value(0.0f),
	m_timeUnit(SECOND)
{
}

Utility::Timing::TimeSpan::TimeSpan(float timeValue, TimeUnit timeUnit) :
	m_value(timeValue),
	m_timeUnit(timeUnit)
{
}

Utility::Timing::TimeSpan::~TimeSpan()
{
}

Utility::Timing::TimeSpan::TimeSpan(const TimeSpan& timeSpan) :
	m_value(timeSpan.m_value),
	m_timeUnit(timeSpan.m_timeUnit)
{
}

Utility::Timing::TimeSpan::TimeSpan(TimeSpan&& timeSpan) :
	m_value(std::move(timeSpan.m_value)),
	m_timeUnit(std::move(timeSpan.m_timeUnit))
{
}

void Utility::Timing::TimeSpan::AdjustUnitToValue()
{
	// This function should make the m_value and m_unit variables easier to read, i.e.
	// if m_value=123456,789 and m_unit=us then we should change m_value to 123,456789 and m_unit=ms.
	WARNING_LOG_UTILITY("The function has not been tested yet.");

	if ((m_value < 1000.0f) && (m_value > 1.0f)) // If value is in range [1; 1000) then we don't need to change the unit.
	{
		return;
	}
	if ((m_value > 1.0f) && (m_timeUnit == SECOND)) // If value is > 1.0 and we have the biggest unit already then there is nothing we can do.
	{
		return;
	}
	if ((m_value < 1.0f) && (m_timeUnit == NANOSECOND)) // If value is < 1.0 and we have the lowest unit already then there is nothing we can do.
	{
		return;
	}

	/* ==================== Calculating the new unit begin ==================== */
	int unitOffset;
	if (m_value >= 1e9f) { unitOffset = 3; }
	else if (m_value >= 1e6f) { unitOffset = 2; }
	else if (m_value >= 1e3f) { unitOffset = 1; }
	else if (m_value >= 1e-3f) { unitOffset = -1; }
	else if (m_value >= 1e-6f) { unitOffset = -2; }
	else if (m_value >= 1e-9f) { unitOffset = -3; }
	else { unitOffset = -4; }
	//TimeUnit newUnit = min(min(m_unit - unitOffset, NANOSECOND), 0);
	int newUnit = m_timeUnit - unitOffset;
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
	float newValueConversionFactor = pow(10.0f, 3.0f * (newUnit - m_timeUnit));
	m_value *= newValueConversionFactor;
	/* ==================== Calculating the new value end ==================== */
	m_timeUnit = static_cast<TimeUnit>(newUnit);
}

Utility::Timing::TimeSpan Utility::Timing::TimeSpan::CalculateTimeSpanWithAdjustedUnitToValue() const
{
	TimeSpan adjustedTimeSpan(*this);
	adjustedTimeSpan.AdjustUnitToValue();
	return adjustedTimeSpan;
}

Utility::Timing::TimeSpan& Utility::Timing::TimeSpan::operator/=(int s)
{
	m_value /= s;
	return (*this);
}

void Utility::Timing::TimeSpan::operator=(const TimeSpan& timeSpan)
{
	m_value = timeSpan.m_value;
	m_timeUnit = timeSpan.m_timeUnit;
}

void Utility::Timing::TimeSpan::operator=(TimeSpan&& timeSpan)
{
	m_value = std::move(timeSpan.m_value);
	m_timeUnit = std::move(timeSpan.m_timeUnit);
}

bool Utility::Timing::TimeSpan::operator==(const TimeSpan& timeSpan) const
{
	const float eps = 1e-12f;
	if (m_timeUnit == timeSpan.GetTimeUnit())
	{
		return abs(m_value - timeSpan.GetValue()) < eps;
	}
	float timeUnitConvertingFactor = DateTime::GetTimeUnitConvertingFactor(m_timeUnit, timeSpan.GetTimeUnit());
	DELOCUST_LOG_UTILITY("Value: ", m_value, "; time unit factor: ", timeUnitConvertingFactor, "; Other value: ", timeSpan.GetValue());
	return abs(m_value * timeUnitConvertingFactor - timeSpan.GetValue()) < eps;
}

bool Utility::Timing::TimeSpan::operator!=(const TimeSpan& timeSpan) const
{
	return !(this->operator==(timeSpan));
}

bool Utility::Timing::TimeSpan::operator<(const TimeSpan &timeSpan) const
{
	//WARNING_LOG_UTILITY("The function has not been tested yet.");
	if (m_timeUnit == timeSpan.GetTimeUnit())
	{
		return m_value < timeSpan.GetValue();
	}
	float timeUnitConvertingFactor = DateTime::GetTimeUnitConvertingFactor(m_timeUnit, timeSpan.GetTimeUnit());
	return (m_value * timeUnitConvertingFactor) < timeSpan.GetValue();
}

bool Utility::Timing::TimeSpan::operator>(const TimeSpan &timeSpan) const
{
	//WARNING_LOG_UTILITY("The function has not been tested yet");
	if (m_timeUnit == timeSpan.GetTimeUnit())
	{
		return m_value > timeSpan.GetValue();
	}
	float timeUnitConvertingFactor = DateTime::GetTimeUnitConvertingFactor(m_timeUnit, timeSpan.GetTimeUnit());
	return (m_value * timeUnitConvertingFactor) > timeSpan.GetValue();
}

std::string Utility::Timing::TimeSpan::ToString() const
{
	std::stringstream ss("");
	ss << m_value << " " << DateTime::ConvertTimeUnitToString(m_timeUnit);
	return ss.str();
}
/* ==================== TimeSpan class end ==================== */

/* ==================== DateTime class begin ==================== */
/* static */ const std::array<int, Utility::Timing::DateTime::MONTHS_COUNT> Utility::Timing::DateTime::DAYS_COUNT_IN_MONTH = { 31, 28 /* We don't account for "przestêpny" years */, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
/* static */ const std::array<int, Utility::Timing::DateTime::MONTHS_COUNT> Utility::Timing::DateTime::DAYS_COUNT_FROM_FIRST_DAY_IN_YEAR = {
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

/* static */ const std::array<std::array<float, Utility::Timing::TIME_UNITS_COUNT>, Utility::Timing::TIME_UNITS_COUNT> Utility::Timing::DateTime::TIME_UNITS_CONVERSION_FACTORS = { {
	{ 1.0f, // hour to hour conversion
	static_cast<float>(MINUTES_PER_HOUR), // hour to minutes conversion
	static_cast<float>(SECONDS_PER_HOUR), // hour to seconds conversion
	static_cast<float>(SECONDS_PER_HOUR * MILLISECONDS_PER_SECOND), // hour to milliseconds
	static_cast<float>(SECONDS_PER_HOUR * MILLISECONDS_PER_SECOND * MICROSECONDS_PER_MILLISECOND), // hour to microseconds
	static_cast<float>(SECONDS_PER_HOUR * MILLISECONDS_PER_SECOND * MICROSECONDS_PER_MILLISECOND * NANOSECONDS_PER_MICROSECOND) }, // hour to nanoseconds
	{ 1.0f / MINUTES_PER_HOUR, // minute to hours conversion
	1.0f, // minute to minute conversion
	static_cast<float>(SECONDS_PER_MINUTE), // minute to seconds
	static_cast<float>(SECONDS_PER_MINUTE * MILLISECONDS_PER_SECOND), // minute to milliseconds
	static_cast<float>(SECONDS_PER_MINUTE * MILLISECONDS_PER_SECOND * MICROSECONDS_PER_MILLISECOND), // minute to microseconds
	static_cast<float>(SECONDS_PER_MINUTE * MILLISECONDS_PER_SECOND * MICROSECONDS_PER_MILLISECOND * NANOSECONDS_PER_MICROSECOND) }, // Minute to nanoseconds
	{ 1.0f / SECONDS_PER_HOUR, 1.0f / SECONDS_PER_MINUTE, 1.0f, static_cast<float>(MILLISECONDS_PER_SECOND), static_cast<float>(MILLISECONDS_PER_SECOND * MICROSECONDS_PER_MILLISECOND), static_cast<float>(MILLISECONDS_PER_SECOND * MICROSECONDS_PER_MILLISECOND * NANOSECONDS_PER_MICROSECOND) }, // Second
	{ 1.0f / (SECONDS_PER_HOUR * MILLISECONDS_PER_SECOND), 1.0f / (SECONDS_PER_MINUTE * MILLISECONDS_PER_SECOND), 1.0f / MILLISECONDS_PER_SECOND, 1.0f, static_cast<float>(MICROSECONDS_PER_MILLISECOND), static_cast<float>(MICROSECONDS_PER_MILLISECOND * NANOSECONDS_PER_MICROSECOND) }, // Millisecond
	{ 1.0f / (SECONDS_PER_HOUR * MILLISECONDS_PER_SECOND * MICROSECONDS_PER_MILLISECOND), 1.0f / (SECONDS_PER_MINUTE * MILLISECONDS_PER_SECOND * MICROSECONDS_PER_MILLISECOND), 1.0f / (MILLISECONDS_PER_SECOND * MICROSECONDS_PER_MILLISECOND), 1.0f / MICROSECONDS_PER_MILLISECOND, 1.0f, static_cast<float>(NANOSECONDS_PER_MICROSECOND) }, // Microsecond
	{ 1.0f / (SECONDS_PER_HOUR * MILLISECONDS_PER_SECOND * MICROSECONDS_PER_MILLISECOND * NANOSECONDS_PER_MICROSECOND), 1.0f / (SECONDS_PER_MINUTE * MILLISECONDS_PER_SECOND * MICROSECONDS_PER_MILLISECOND * NANOSECONDS_PER_MICROSECOND), 1.0f / (MILLISECONDS_PER_SECOND * MICROSECONDS_PER_MILLISECOND * NANOSECONDS_PER_MICROSECOND), 1.0f / MICROSECONDS_PER_MILLISECOND * NANOSECONDS_PER_MICROSECOND, 1.0f / NANOSECONDS_PER_MICROSECOND, 1.0f } // Nanosecond
} };
/* static */ const int Utility::Timing::DateTime::NANOSECONDS_PER_MICROSECOND = 1000;
/* static */ const int Utility::Timing::DateTime::MICROSECONDS_PER_MILLISECOND = 1000;
/* static */ const int Utility::Timing::DateTime::MILLISECONDS_PER_SECOND = 1000;
/* static */ const int Utility::Timing::DateTime::SECONDS_PER_MINUTE = 60;
/* static */ const int Utility::Timing::DateTime::SECONDS_PER_HOUR = 3600;
/* static */ const int Utility::Timing::DateTime::SECONDS_PER_DAY = 86400;
/* static */ const int Utility::Timing::DateTime::MINUTES_PER_HOUR = 60;
/* static */ const int Utility::Timing::DateTime::HOURS_PER_DAY = 24;
/* static */ const int Utility::Timing::DateTime::DAYS_PER_YEAR = 365;

/* static */ Utility::Timing::DateTime Utility::Timing::DateTime::Now()
{
	/* ==================== Calculate local time #1 begin ==================== */
	SYSTEMTIME st; // See https://msdn.microsoft.com/pl-pl/library/windows/desktop/ms724950(v=vs.85).aspx
	GetSystemTime(&st);
	return DateTime(st.wYear, st.wMonth - 1 /* because SYSTEMTIME counts months from 1 */, st.wDay - 1 /* because SYSTEMTIME counts days from 1 */, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
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

Utility::Timing::DateTime::DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond /* = 0 */) :
	m_year(year),
	m_month(month),
	m_day(day),
	m_hour(hour),
	m_minute(minute),
	m_second(second),
	m_millisecond(millisecond)
{
	// TODO: Check ranges.
}

Utility::Timing::DateTime::~DateTime()
{
}

Utility::Timing::DateTime::DateTime(const DateTime& dateTime) :
	m_year(dateTime.m_year),
	m_month(dateTime.m_month),
	m_day(dateTime.m_day),
	m_hour(dateTime.m_hour),
	m_minute(dateTime.m_minute),
	m_second(dateTime.m_second),
	m_millisecond(dateTime.m_millisecond)
{
}

Utility::Timing::DateTime::DateTime(DateTime&& dateTime) :
	m_year(std::move(dateTime.m_year)),
	m_month(std::move(dateTime.m_month)),
	m_day(std::move(dateTime.m_day)),
	m_hour(std::move(dateTime.m_hour)),
	m_minute(std::move(dateTime.m_minute)),
	m_second(std::move(dateTime.m_second)),
	m_millisecond(std::move(dateTime.m_millisecond))
{
}

Utility::Timing::DateTime& Utility::Timing::DateTime::operator=(const DateTime& dateTime)
{
	m_year = dateTime.m_year;
	m_month = dateTime.m_month;
	m_day = dateTime.m_day;
	m_hour = dateTime.m_hour;
	m_minute = dateTime.m_minute;
	m_second = dateTime.m_second;
	m_millisecond = dateTime.m_millisecond;
	return *this;
}

Utility::Timing::DateTime& Utility::Timing::DateTime::operator=(DateTime&& dateTime)
{
	m_year = std::move(dateTime.m_year);
	m_month = std::move(dateTime.m_month);
	m_day = std::move(dateTime.m_day);
	m_hour = std::move(dateTime.m_hour);
	m_minute = std::move(dateTime.m_minute);
	m_second = std::move(dateTime.m_second);
	m_millisecond = std::move(dateTime.m_millisecond);
	return *this;
}

Utility::Timing::DateTime Utility::Timing::DateTime::operator+(const TimeSpan& timeSpan)
{
	return DateTime(*this) += timeSpan;
}

Utility::Timing::DateTime Utility::Timing::DateTime::operator-(const TimeSpan& timeSpan)
{
	return DateTime(*this) -= timeSpan;
}

Utility::Timing::DateTime& Utility::Timing::DateTime::operator+=(const TimeSpan& timeSpan)
{
	return AddTimeSpan(timeSpan.GetValue(), timeSpan.GetTimeUnit());
}

Utility::Timing::DateTime& Utility::Timing::DateTime::operator-=(const TimeSpan& timeSpan)
{
	return AddTimeSpan(-timeSpan.GetValue(), timeSpan.GetTimeUnit());
}

Utility::Timing::DateTime& Utility::Timing::DateTime::AddTimeSpan(float timeSpanValue, TimeUnit timeUnit)
{
	const bool isNegative = (timeSpanValue < 0.0f);
	int timeSpanValueInt = static_cast<int>(timeSpanValue);
	//ERROR_LOG_UTILITY("Before: TimeSpanValue = ", timeSpanValue, "; timeSpanValueInt = ", timeSpanValueInt, "; dateTime = [",
	//	m_year, ", ", m_month, ", ", m_day, ", ", m_hour, ", ", m_minute, ", ", m_second, ", ", m_millisecond, "].");
	switch (timeUnit)
	{
	case HOUR:
		m_hour += timeSpanValueInt;
		timeSpanValue = (timeSpanValue - static_cast<float>(timeSpanValueInt)) * MINUTES_PER_HOUR;
		timeSpanValueInt = static_cast<int>(timeSpanValue);
	case MINUTE:
		m_minute += timeSpanValueInt;
		timeSpanValue = (timeSpanValue - static_cast<float>(timeSpanValueInt)) * SECONDS_PER_MINUTE;
		timeSpanValueInt = static_cast<int>(timeSpanValue);
	case SECOND:
		m_second += timeSpanValueInt;
		timeSpanValue = (timeSpanValue - static_cast<float>(timeSpanValueInt)) * MILLISECONDS_PER_SECOND;
		timeSpanValueInt = static_cast<int>(timeSpanValue);
	case MILLISECOND:
		m_millisecond += timeSpanValueInt;
		timeSpanValue = (timeSpanValue - static_cast<float>(timeSpanValueInt)) * MICROSECONDS_PER_MILLISECOND;
	case MICROSECOND:
	case NANOSECOND:
	default:
		break;
	}

	if (isNegative)
	{
		while (m_millisecond < 0)
		{
			--m_second;
			m_millisecond += MILLISECONDS_PER_SECOND;
		}
		while (m_second < 0)
		{
			--m_minute;
			m_second += SECONDS_PER_MINUTE;
		}
		while (m_minute < 0)
		{
			--m_hour;
			m_minute += MINUTES_PER_HOUR;
		}
		while (m_hour < 0)
		{
			--m_day;
			m_hour += HOURS_PER_DAY;
		}
		while (m_day < 0)
		{
			//CRITICAL_LOG_UTILITY("After: TimeSpanValue = ", timeSpanValue, "; timeSpanValueInt = ", timeSpanValueInt, "; dateTime = [",
			//	m_year, ", ", m_month, ", ", m_day, ", ", m_hour, ", ", m_minute, ", ", m_second, ", ", m_millisecond, "].");
			m_day += DAYS_COUNT_IN_MONTH[m_month];
			--m_month;
			if (m_month < 0)
			{
				--m_year;
				m_month = MONTHS_COUNT - 1;
			}
		}
	}
	else
	{
		while (m_millisecond >= MILLISECONDS_PER_SECOND)
		{
			++m_second;
			m_millisecond -= MILLISECONDS_PER_SECOND;
		}
		while (m_second >= SECONDS_PER_MINUTE)
		{
			++m_minute;
			m_second -= SECONDS_PER_MINUTE;
		}
		while (m_minute >= MINUTES_PER_HOUR)
		{
			++m_hour;
			m_minute -= MINUTES_PER_HOUR;
		}
		while (m_hour >= HOURS_PER_DAY)
		{
			++m_day;
			m_hour -= HOURS_PER_DAY;
		}
		while (m_day >= DAYS_COUNT_IN_MONTH[m_month])
		{
			m_day -= DAYS_COUNT_IN_MONTH[m_month];
			++m_month;
			if (m_month == MONTHS_COUNT)
			{
				++m_year;
				m_month = 0;
			}
		}
	}
	//ERROR_LOG_UTILITY("After: TimeSpanValue = ", timeSpanValue, "; timeSpanValueInt = ", timeSpanValueInt, "; dateTime = [",
	//	m_year, ", ", m_month, ", ", m_day, ", ", m_hour, ", ", m_minute, ", ", m_second, ", ", m_millisecond, "].");
	return *this;
}

bool Utility::Timing::DateTime::operator==(const DateTime& dateTime) const
{
	return (m_year == dateTime.m_year && m_month == dateTime.m_month && m_day == dateTime.m_day &&
		m_hour == dateTime.m_hour && m_minute == dateTime.m_minute && m_second == dateTime.m_second &&
		m_millisecond == dateTime.m_millisecond);
}

bool Utility::Timing::DateTime::operator!=(const DateTime& dateTime) const
{
	return !(operator==(dateTime));
}

bool Utility::Timing::DateTime::operator<(const DateTime& dateTime) const
{
	if (m_year < dateTime.m_year)
	{
		return true;
	}
	else if (m_year == dateTime.m_year)
	{
		if (m_month < dateTime.m_month)
		{
			return true;
		}
		else if (m_month == dateTime.m_month)
		{
			if (m_day < dateTime.m_day)
			{
				return true;
			}
			else if (m_day == dateTime.m_day)
			{
				if (m_hour < dateTime.m_hour)
				{
					return true;
				}
				else if (m_hour == dateTime.m_hour)
				{
					if (m_minute < dateTime.m_minute)
					{
						return true;
					}
					else if (m_minute == dateTime.m_minute)
					{
						if (m_second < dateTime.m_second)
						{
							return true;
						}
						else if (m_second == dateTime.m_second)
						{
							if (m_millisecond < dateTime.m_millisecond)
							{
								return true;
							}
						}
					}
				}
			}
		}
	}
	return false;
}

bool Utility::Timing::DateTime::operator>(const DateTime& dateTime) const
{
	if (m_year > dateTime.m_year)
	{
		return true;
	}
	else if (m_year == dateTime.m_year)
	{
		if (m_month > dateTime.m_month)
		{
			return true;
		}
		else if (m_month == dateTime.m_month)
		{
			if (m_day > dateTime.m_day)
			{
				return true;
			}
			else if (m_day == dateTime.m_day)
			{
				if (m_hour > dateTime.m_hour)
				{
					return true;
				}
				else if (m_hour == dateTime.m_hour)
				{
					if (m_minute > dateTime.m_minute)
					{
						return true;
					}
					else if (m_minute == dateTime.m_minute)
					{
						if (m_second > dateTime.m_second)
						{
							return true;
						}
						else if (m_second == dateTime.m_second)
						{
							if (m_millisecond > dateTime.m_millisecond)
							{
								return true;
							}
						}
					}
				}
			}
		}
	}
	return false;
}

int Utility::Timing::DateTime::GetDayInYear() const
{
	//CRITICAL_LOG_UTILITY("Day in year: ", DAYS_COUNT_FROM_FIRST_DAY_IN_YEAR[m_month] + m_day);
	return DAYS_COUNT_FROM_FIRST_DAY_IN_YEAR[m_month] + m_day;
}

int Utility::Timing::DateTime::GetDayTime() const
{
	return m_hour * SECONDS_PER_HOUR + m_minute * SECONDS_PER_MINUTE + m_second + m_millisecond * MILLISECONDS_PER_SECOND;
}

std::string Utility::Timing::DateTime::ToString(const char *format /* = "%Y-%m-%d %H:%M:%S" */) const
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

	time_t rawtime = m_second;
	struct tm timeinfo;
	timeinfo.tm_year = m_year - 1900;
	timeinfo.tm_mon = m_month;
	timeinfo.tm_mday = m_day + 1; /* because we count days from 0 */
	timeinfo.tm_hour = m_hour;
	timeinfo.tm_min = m_minute;
	timeinfo.tm_sec = m_second;
	char buffer[80];
	if ((strftime(buffer, 80, format, &timeinfo)) == 0)
	{
		return std::string("time-error");
	}
	return std::string(buffer);
}
/* ==================== DateTime class end ==================== */

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
	if (elapsedTimeInMillieconds > 1e3f)
	{
		return TimeSpan(elapsedTimeInMillieconds * 1e-3f, SECOND);
	}
	else if (elapsedTimeInMillieconds > 1.0f)
	{
		return TimeSpan(elapsedTimeInMillieconds, MILLISECOND);
	}
	else if (elapsedTimeInMillieconds > 1e-3f)
	{
		return TimeSpan(elapsedTimeInMillieconds * 1e3f, MICROSECOND);
	}
	return TimeSpan(elapsedTimeInMillieconds * 1e6f, NANOSECOND);
}

Utility::Timing::TimeSpan Utility::Timing::Timer::GetTimeSpan(TimeUnit timeUnit) const
{
	float elapsedTimeInMilliseconds = CalculateElapsedTimeInMilliseconds();
	//INFO_LOG_UTILITY("Elapsed time in seconds = ", elapsedTimeInMilliseconds);
	return TimeSpan(elapsedTimeInMilliseconds * DateTime::GetTimeUnitConvertingFactor(MILLISECOND, timeUnit), timeUnit);
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
