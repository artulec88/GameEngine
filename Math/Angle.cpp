#include "StdAfx.h"
#include "Angle.h"
#include "FloatingPoint.h"
#include "Utility/ILogger.h"
#include <algorithm>

math::Angle::Angle(const Angle& angle) :
	m_angle(angle.m_angle)
#ifdef PROFILING_MATH_MODULE_ENABLED
	,m_classStats(STATS_STORAGE.GetClassStats("Angle"))
#endif
{
	START_PROFILING_MATH(false, "2");
	STOP_PROFILING_MATH("2");
}

math::Angle::Angle(Angle&& angle) noexcept:
	m_angle(std::move(angle.m_angle))
#ifdef PROFILING_MATH_MODULE_ENABLED
	, m_classStats(STATS_STORAGE.GetClassStats("Angle")) // TODO: Maybe use angle.m_classStats?
#endif
{
	START_PROFILING_MATH(false, "3");
	STOP_PROFILING_MATH("3");
}

math::Angle& math::Angle::operator=(const Angle& angle)
{
	START_PROFILING_MATH(false, "");
	m_angle = angle.m_angle;
	STOP_PROFILING_MATH("");
	return *this;
}

math::Angle& math::Angle::operator=(Angle&& angle) noexcept
{
	START_PROFILING_MATH(false, "");
	m_angle = std::move(angle.m_angle);
	STOP_PROFILING_MATH("");
	return *this;
}

math::Angle math::Angle::operator-() const
{
	START_PROFILING_MATH(false, "1");
	STOP_PROFILING_MATH("1");
	return Angle(-m_angle, units::RADIAN);
}

math::Angle math::Angle::operator+(const Angle& angle) const
{
	START_PROFILING_MATH(false, "");
	STOP_PROFILING_MATH("");
	return Angle(m_angle + angle.m_angle, units::RADIAN);
}

math::Angle math::Angle::operator-(const Angle& angle) const
{
	START_PROFILING_MATH(false, "2");
	STOP_PROFILING_MATH("2");
	return Angle(m_angle - angle.m_angle, units::RADIAN);
}

math::Angle math::Angle::operator*(const Real s) const
{
	START_PROFILING_MATH(false, "");
	STOP_PROFILING_MATH("");
	return Angle(m_angle * s, units::RADIAN);
}

math::Angle math::Angle::operator/(const Real s) const
{
	START_PROFILING_MATH(false, "");
	STOP_PROFILING_MATH("");
	return Angle(m_angle / s, units::RADIAN);
}

math::Angle& math::Angle::operator+=(const Angle& angle)
{
	START_PROFILING_MATH(false, "");
	m_angle += angle.m_angle;
	STOP_PROFILING_MATH("");
	return *this;
}

math::Angle& math::Angle::operator-=(const Angle& angle)
{
	START_PROFILING_MATH(false, "");
	m_angle -= angle.m_angle;
	STOP_PROFILING_MATH("");
	return *this;
}

math::Angle& math::Angle::operator*=(const Real s)
{
	START_PROFILING_MATH(false, "");
	m_angle *= s;
	STOP_PROFILING_MATH("");
	return *this;
}

math::Angle& math::Angle::operator/=(const Real s)
{
	START_PROFILING_MATH(false, "");
	CHECK_CONDITION_MATH(!AlmostEqual(s, REAL_ZERO), Utility::Logging::ERR, "Cannot divide the angle by 0. Returning the unmodified angle.");
	m_angle /= s;
	STOP_PROFILING_MATH("");
	return *this;
}

//math::Angle& math::Angle::operator=(Angle angle)
//{
//	swap(*this, angle);
//	return *this;
//}

bool math::Angle::operator==(const Angle& angle) const
{
	START_PROFILING_MATH(false, "");
	STOP_PROFILING_MATH("");
	return AlmostEqual(m_angle, angle.m_angle);
}

bool math::Angle::operator!=(const Angle& angle) const
{
	START_PROFILING_MATH(false, "");
	STOP_PROFILING_MATH("");
	return !(*this == angle);
}

bool math::Angle::operator>(const Angle& angle) const
{
	START_PROFILING_MATH(false, "");
	STOP_PROFILING_MATH("");
	return m_angle > angle.m_angle;
}

bool math::Angle::operator>=(const Angle& angle) const
{
	START_PROFILING_MATH(false, "");
	STOP_PROFILING_MATH("");
	return !(m_angle < angle.m_angle);
}

bool math::Angle::operator<(const Angle& angle) const
{
	START_PROFILING_MATH(false, "");
	STOP_PROFILING_MATH("");
	return m_angle < angle.m_angle;
}

bool math::Angle::operator<=(const Angle& angle) const
{
	START_PROFILING_MATH(false, "");
	STOP_PROFILING_MATH("");
	return !(m_angle > angle.m_angle);
}
