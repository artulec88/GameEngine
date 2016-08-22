#include "StdAfx.h"
#include "Angle.h"
#include "FloatingPoint.h"
#include "Utility\ILogger.h"
#include <sstream>
#include <algorithm>

using namespace Math;

Angle::Angle(const Angle& angle) :
	m_angle(angle.m_angle),
	m_unit(angle.m_unit)
#ifdef CALCULATE_MATH_STATS
	,m_classStats(STATS_STORAGE.GetClassStats("Angle"))
#endif
{
}

Angle::Angle(Angle&& angle) :
	m_angle(angle.m_angle),
	m_unit(angle.m_unit)
#ifdef CALCULATE_MATH_STATS
	, m_classStats(STATS_STORAGE.GetClassStats("Angle")) // TODO: Maybe use angle.m_classStats?
#endif
{
}


Angle Angle::operator-() const
{
	return Angle(-m_angle, m_unit);
}

Angle Angle::operator+(const Angle& angle) const
{
	START_PROFILING("");
	switch (m_unit)
	{
	case Unit::DEGREE:
		STOP_PROFILING("");
		return Angle(m_angle + angle.GetAngleInDegrees(), m_unit);
		break;
	case Unit::RADIAN:
		STOP_PROFILING("");
		return Angle(m_angle + angle.GetAngleInRadians(), m_unit);
		break;
	default:
		STOP_PROFILING("");
		ERROR_LOG_MATH("Cannot add two angles. The angle is specified in unknown unit type (", m_unit, ")");
		return Angle(*this);
	}
}

Angle Angle::operator-(const Angle& angle) const
{
	START_PROFILING("");
	switch (m_unit)
	{
	case Unit::DEGREE:
		STOP_PROFILING("");
		return Angle(m_angle - angle.GetAngleInDegrees(), m_unit);
		break;
	case Unit::RADIAN:
		STOP_PROFILING("");
		return Angle(m_angle - angle.GetAngleInRadians(), m_unit);
		break;
	default:
		STOP_PROFILING("");
		ERROR_LOG_MATH("Cannot subtract two angles. The angle is specified in unknown unit type (", m_unit, ")");
		return Angle(*this);
	}
}

Angle Angle::operator*(Real s) const
{
	return Angle(m_angle * s, m_unit);
}

Angle Angle::operator/(Real s) const
{
	return Angle(m_angle / s, m_unit);
}

Angle& Angle::operator+=(const Angle& angle)
{
	START_PROFILING("");
	switch (m_unit)
	{
	case Unit::DEGREE:
		m_angle += angle.GetAngleInDegrees();
		break;
	case Unit::RADIAN:
		m_angle += angle.GetAngleInRadians();
		break;
	default:
		ERROR_LOG_MATH("Cannot add two angles. The angle is specified in unknown unit type (", m_unit, ")");
	}
	STOP_PROFILING("");
	return *this;
}

Angle& Angle::operator-=(const Angle& angle)
{
	START_PROFILING("");
	switch (m_unit)
	{
	case Unit::DEGREE:
		m_angle -= angle.GetAngleInDegrees();
		break;
	case Unit::RADIAN:
		m_angle -= angle.GetAngleInRadians();
		break;
	default:
		ERROR_LOG_MATH("Cannot subtract two angles. The angle is specified in unknown unit type (", m_unit, ")");
	}
	STOP_PROFILING("");
	return *this;
}

Angle& Angle::operator*=(Real s)
{
	START_PROFILING("");
	m_angle *= s;
	STOP_PROFILING("");
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
	//std::swap(*this, angle);
	//std::swap(m_angle, angle.m_angle);
	//std::swap(m_unit, angle.m_unit);
	//std::swap(m_classStats, angle.m_classStats);
	m_angle = angle.m_angle;
	m_unit = angle.m_unit;
	return (*this);
}

bool Angle::operator==(const Angle& angle) const
{
	START_PROFILING("");
	const Real angleRad = angle.GetAngleInRadians();
	const Real rad = this->GetAngleInRadians();
	bool areAnglesEqual = AlmostEqual(angleRad, rad);
	STOP_PROFILING("");
	return areAnglesEqual;
}

bool Angle::operator!=(const Angle& angle) const
{
	return (!((*this) == angle));
}

bool Angle::operator>(const Angle& angle) const
{
	return (GetAngleInDegrees() > angle.GetAngleInDegrees());
}

bool Angle::operator>=(const Angle& angle) const
{
	return (! (GetAngleInDegrees() < angle.GetAngleInDegrees()));
}

bool Angle::operator<(const Angle& angle) const
{
	return (GetAngleInDegrees() < angle.GetAngleInDegrees());
}

bool Angle::operator<=(const Angle& angle) const
{
	return (! (GetAngleInDegrees() > angle.GetAngleInDegrees()));
}

std::string Angle::ToString() const
{
	std::stringstream ss("");
	//ss << GetAngleInDegrees() << L'\u2103'; // TODO: Check whether the correct "degree" sign is displayed.
	//ss << GetAngleInDegrees() << '248'; // TODO: Check whether the correct "degree" sign is displayed.
	ss << GetAngleInDegrees() << " degrees";
	return ss.str();
}