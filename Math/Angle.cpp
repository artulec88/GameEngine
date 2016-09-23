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
	START_PROFILING_MATH(false, "2");
	STOP_PROFILING_MATH("2");
}

Math::Angle::Angle(Angle&& angle) :
	m_angle(std::move(angle.m_angle))
#ifdef PROFILING_MATH_MODULE_ENABLED
	, m_classStats(STATS_STORAGE.GetClassStats("Angle")) // TODO: Maybe use angle.m_classStats?
#endif
{
	START_PROFILING_MATH(false, "3");
	STOP_PROFILING_MATH("3");
}

Math::Angle& Math::Angle::operator=(const Angle& angle)
{
	START_PROFILING_MATH(false, "");
	m_angle = angle.m_angle;
	STOP_PROFILING_MATH("");
	return (*this);
}

Math::Angle& Math::Angle::operator=(Angle&& angle)
{
	START_PROFILING_MATH(false, "");
	m_angle = std::move(angle.m_angle);
	STOP_PROFILING_MATH("");
	return (*this);
}

Math::Angle Math::Angle::operator-() const
{
	START_PROFILING_MATH(false, "1");
	STOP_PROFILING_MATH("1");
	return Angle(-m_angle, Math::Unit::RADIAN);
}

Math::Angle Math::Angle::operator+(const Angle& angle) const
{
	START_PROFILING_MATH(false, "");
	STOP_PROFILING_MATH("");
	return Angle(m_angle + angle.m_angle, Math::Unit::RADIAN);
}

Math::Angle Math::Angle::operator-(const Angle& angle) const
{
	START_PROFILING_MATH(false, "2");
	STOP_PROFILING_MATH("2");
	return Angle(m_angle - angle.m_angle, Unit::RADIAN);
}

Math::Angle Math::Angle::operator*(Real s) const
{
	START_PROFILING_MATH(false, "");
	STOP_PROFILING_MATH("");
	return Angle(m_angle * s, Unit::RADIAN);
}

Math::Angle Math::Angle::operator/(Real s) const
{
	START_PROFILING_MATH(false, "");
	STOP_PROFILING_MATH("");
	return Angle(m_angle / s, Unit::RADIAN);
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

//Math::Angle& Math::Angle::operator=(Angle angle)
//{
//	swap(*this, angle);
//	return *this;
//}

bool Math::Angle::operator==(const Angle& angle) const
{
	START_PROFILING_MATH(false, "");
	STOP_PROFILING_MATH("");
	return AlmostEqual(m_angle, angle.m_angle);
}

bool Math::Angle::operator!=(const Angle& angle) const
{
	START_PROFILING_MATH(false, "");
	STOP_PROFILING_MATH("");
	return (!((*this) == angle));
}

bool Math::Angle::operator>(const Angle& angle) const
{
	START_PROFILING_MATH(false, "");
	STOP_PROFILING_MATH("");
	return (m_angle > angle.m_angle);
}

bool Math::Angle::operator>=(const Angle& angle) const
{
	START_PROFILING_MATH(false, "");
	STOP_PROFILING_MATH("");
	return (! (m_angle < angle.m_angle));
}

bool Math::Angle::operator<(const Angle& angle) const
{
	START_PROFILING_MATH(false, "");
	STOP_PROFILING_MATH("");
	return (m_angle < angle.m_angle);
}

bool Math::Angle::operator<=(const Angle& angle) const
{
	START_PROFILING_MATH(false, "");
	STOP_PROFILING_MATH("");
	return (! (m_angle > angle.m_angle));
}

std::string Math::Angle::ToString() const
{
	START_PROFILING_MATH(false, "");
	STOP_PROFILING_MATH("");
	std::stringstream ss("");
	//ss << GetAngleInDegrees() << L'\u2103'; // TODO: Check whether the correct "degree" sign is displayed.
	//ss << GetAngleInDegrees() << '248'; // TODO: Check whether the correct "degree" sign is displayed.
	ss << ToDeg(m_angle) << " degrees";
	return ss.str();
}