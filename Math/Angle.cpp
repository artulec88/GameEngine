#include "StdAfx.h"
#include "Angle.h"
#include "FloatingPoint.h"
#include "Utility\ILogger.h"
#include <sstream>
#include <algorithm>

Math::Angle::Angle(const Angle& angle) :
	m_angle(angle.m_angle)
#ifdef PROFILING_MATH_MODULE_ENABLED
	,m_classStats(STATS_STORAGE.GetClassStats("Angle"))
#endif
{
}

Math::Angle::Angle(Angle&& angle) :
	m_angle(std::move(angle.m_angle))
#ifdef PROFILING_MATH_MODULE_ENABLED
	, m_classStats(STATS_STORAGE.GetClassStats("Angle")) // TODO: Maybe use angle.m_classStats?
#endif
{
}


Math::Angle Math::Angle::operator-() const
{
	return Angle(-m_angle, Math::Unit::RADIAN);
}

Math::Angle Math::Angle::operator+(const Angle& angle) const
{
#ifdef PROFILING_MATH_MODULE_ENABLED
	START_PROFILING_MATH(false, "");
	Angle resultAngle(m_angle + angle.m_angle, Unit::RADIAN);
	STOP_PROFILING_MATH("");
	return resultAngle;
#else
	return Angle(m_angle + angle.m_angle, Math::Unit::RADIAN);
#endif
}

Math::Angle Math::Angle::operator-(const Angle& angle) const
{
#ifdef PROFILING_MATH_MODULE_ENABLED
	START_PROFILING_MATH(false, "");
	Angle resultAngle(m_angle - angle.m_angle, Unit::RADIAN);
	STOP_PROFILING_MATH("");
	return resultAngle;
#else
	return Angle(m_angle - angle.m_angle, Unit::RADIAN);
#endif
}

Math::Angle Math::Angle::operator*(Real s) const
{
#ifdef PROFILING_MATH_MODULE_ENABLED
	START_PROFILING_MATH(false, "");
	Angle angle(m_angle * s, Unit::RADIAN);
	STOP_PROFILING_MATH("");
	return angle;
#else
	return Angle(m_angle * s, Unit::RADIAN);
#endif
}

Math::Angle Math::Angle::operator/(Real s) const
{
#ifdef PROFILING_MATH_MODULE_ENABLED
	START_PROFILING_MATH(false, "");
	Angle angle(m_angle / s, Unit::RADIAN);
	STOP_PROFILING_MATH("");
	return angle;
#else
	return Angle(m_angle / s, Unit::RADIAN);
#endif
}

Math::Angle& Math::Angle::operator+=(const Angle& angle)
{
	START_PROFILING_MATH(false, "");
	m_angle += angle.m_angle;
	STOP_PROFILING_MATH("");
	return *this;
}

Math::Angle& Math::Angle::operator-=(const Angle& angle)
{
	START_PROFILING_MATH(false, "");
	m_angle -= angle.m_angle;
	STOP_PROFILING_MATH("");
	return *this;
}

Math::Angle& Math::Angle::operator*=(Real s)
{
	START_PROFILING_MATH(false, "");
	m_angle *= s;
	STOP_PROFILING_MATH("");
	return *this;
}

Math::Angle& Math::Angle::operator/=(Real s)
{
	START_PROFILING_MATH(false, "");
	CHECK_CONDITION_MATH(!AlmostEqual(s, REAL_ZERO), Utility::Logging::ERR, "Cannot divide the angle by 0. Returning the unmodified angle.");
	m_angle /= s;
	STOP_PROFILING_MATH("");
	return *this;
}

Math::Angle& Math::Angle::operator=(const Angle& angle)
{
	m_angle = angle.m_angle;
	return (*this);
}

Math::Angle& Math::Angle::operator=(Angle&& angle)
{
	m_angle = std::move(angle.m_angle);
	return (*this);
}

bool Math::Angle::operator==(const Angle& angle) const
{
#ifdef PROFILING_MATH_MODULE_ENABLED
	START_PROFILING_MATH(false, "");
	bool areAnglesEqual = AlmostEqual(m_angle, angle.m_angle);
	STOP_PROFILING_MATH("");
	return areAnglesEqual;
#else
	return AlmostEqual(m_angle, angle.m_angle);
#endif
}

bool Math::Angle::operator!=(const Angle& angle) const
{
	return (!((*this) == angle));
}

bool Math::Angle::operator>(const Angle& angle) const
{
	return (m_angle > angle.m_angle);
}

bool Math::Angle::operator>=(const Angle& angle) const
{
	return (! (m_angle < angle.m_angle));
}

bool Math::Angle::operator<(const Angle& angle) const
{
	return (m_angle < angle.m_angle);
}

bool Math::Angle::operator<=(const Angle& angle) const
{
	return (! (m_angle > angle.m_angle));
}

std::string Math::Angle::ToString() const
{
	std::stringstream ss("");
	//ss << GetAngleInDegrees() << L'\u2103'; // TODO: Check whether the correct "degree" sign is displayed.
	//ss << GetAngleInDegrees() << '248'; // TODO: Check whether the correct "degree" sign is displayed.
	ss << ToDeg(m_angle) << " degrees";
	return ss.str();
}