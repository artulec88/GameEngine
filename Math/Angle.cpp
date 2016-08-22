#include "StdAfx.h"
#include "Angle.h"
#include "FloatingPoint.h"
#include "Utility\ILogger.h"
#include <sstream>
#include <algorithm>

using namespace Math;

Angle::Angle(const Angle& angle) :
	m_angle(angle.m_angle)
#ifdef CALCULATE_MATH_STATS
	,m_classStats(STATS_STORAGE.GetClassStats("Angle"))
#endif
{
}

Angle::Angle(Angle&& angle) :
	m_angle(std::move(angle.m_angle))
#ifdef CALCULATE_MATH_STATS
	, m_classStats(STATS_STORAGE.GetClassStats("Angle")) // TODO: Maybe use angle.m_classStats?
#endif
{
}


Angle Angle::operator-() const
{
	return Angle(-m_angle, Math::Unit::RADIAN);
}

Angle Angle::operator+(const Angle& angle) const
{
	START_PROFILING("");
	STOP_PROFILING(""); // TODO: This profiling makes no sense, because all of the computation is performed after profiling is stopped.
	return Angle(m_angle + angle.m_angle, Math::Unit::RADIAN);
}

Angle Angle::operator-(const Angle& angle) const
{
	START_PROFILING("");
	STOP_PROFILING(""); // TODO: This profiling makes no sense, because all of the computation is performed after profiling is stopped.
	return Angle(m_angle - angle.m_angle, Math::Unit::RADIAN);
}

Angle Angle::operator*(Real s) const
{
	START_PROFILING("");
	STOP_PROFILING(""); // TODO: This profiling makes no sense, because all of the computation is performed after profiling is stopped.
	return Angle(m_angle * s, Math::Unit::RADIAN);
}

Angle Angle::operator/(Real s) const
{
	START_PROFILING("");
	STOP_PROFILING(""); // TODO: This profiling makes no sense, because all of the computation is performed after profiling is stopped.
	return Angle(m_angle / s, Math::Unit::RADIAN);
}

Angle& Angle::operator+=(const Angle& angle)
{
	START_PROFILING("");
	m_angle += angle.m_angle;
	STOP_PROFILING(""); // TODO: This profiling makes no sense, because all of the computation is performed after profiling is stopped.
	return *this;
}

Angle& Angle::operator-=(const Angle& angle)
{
	START_PROFILING("");
	m_angle -= angle.m_angle;
	STOP_PROFILING(""); // TODO: This profiling makes no sense, because all of the computation is performed after profiling is stopped.
	return *this;
}

Angle& Angle::operator*=(Real s)
{
	START_PROFILING("");
	m_angle *= s;
	STOP_PROFILING(""); // TODO: This profiling makes no sense, because all of the computation is performed after profiling is stopped.
	return *this;
}

Angle& Angle::operator/=(Real s)
{
	START_PROFILING("");
#ifdef _DEBUG
	if (AlmostEqual(s, REAL_ZERO))
	{
		ERROR_LOG_MATH("Cannot divide the angle by 0. Returning the unmodified angle.");
		STOP_PROFILING;
		return *this;
	}
#endif
	m_angle /= s;
	STOP_PROFILING("");
	return *this;
}

Angle& Angle::operator=(const Angle& angle)
{
	m_angle = angle.m_angle;
	return (*this);
}

bool Angle::operator==(const Angle& angle) const
{
	START_PROFILING("");
	bool areAnglesEqual = AlmostEqual(m_angle, angle.m_angle);
	STOP_PROFILING("");
	return areAnglesEqual;
}

bool Angle::operator!=(const Angle& angle) const
{
	return (!((*this) == angle));
}

bool Angle::operator>(const Angle& angle) const
{
	return (m_angle > angle.m_angle);
}

bool Angle::operator>=(const Angle& angle) const
{
	return (! (m_angle < angle.m_angle));
}

bool Angle::operator<(const Angle& angle) const
{
	return (m_angle < angle.m_angle);
}

bool Angle::operator<=(const Angle& angle) const
{
	return (! (m_angle > angle.m_angle));
}

std::string Angle::ToString() const
{
	std::stringstream ss("");
	//ss << GetAngleInDegrees() << L'\u2103'; // TODO: Check whether the correct "degree" sign is displayed.
	//ss << GetAngleInDegrees() << '248'; // TODO: Check whether the correct "degree" sign is displayed.
	ss << ToDeg(m_angle) << " degrees";
	return ss.str();
}