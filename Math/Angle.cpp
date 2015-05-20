#include "StdAfx.h"
#include "Angle.h"
#include "FloatingPoint.h"
#include "Utility\ILogger.h"
#include <sstream>

using namespace Math;

Angle::Angle(const Angle& angle) :
	m_angle(angle.GetAngleInRadians()),
	m_unit(Unit::RADIAN)
#ifdef CALCULATE_MATH_STATS
	,m_degToRadConversionCount(0),
	m_radToDegConversionCount(0),
	m_classStats(STATS_STORAGE.GetClassStats("Angle"))
#endif
{
}

// TODO: move this function to *.h file as an inline function
Real Angle::GetAngleInDegrees() const
{
	START_PROFILING;
	switch (m_unit)
	{
	case Unit::DEGREE:
		STOP_PROFILING;
		return m_angle;
	case Unit::RADIAN:
#ifdef CALCULATE_MATH_STATS
		++m_radToDegConversionCount;
#endif
		STOP_PROFILING;
		return ToDeg(m_angle);
	default:
		LOG(Utility::Error, LOGPLACE, "Incorrect unit type for angle with amount=%.2f and unit=%d", m_angle, m_unit);
		STOP_PROFILING;
		return m_angle;
	}
}


// TODO: move this function to *.h file as an inline function
Real Angle::GetAngleInRadians() const
{
	START_PROFILING;
	switch (m_unit)
	{
	case Unit::DEGREE:
#ifdef CALCULATE_MATH_STATS
		++m_degToRadConversionCount;
#endif
		STOP_PROFILING;
		return ToRad(m_angle);
	case Unit::RADIAN:
		STOP_PROFILING;
		return m_angle;
	default:
		LOG(Utility::Error, LOGPLACE, "Incorrect unit type for angle with amount=%.2f and unit=%d", m_angle, m_unit);
		STOP_PROFILING;
		return m_angle;
	}
}

Angle Angle::operator-() const
{
	return Angle(-m_angle, m_unit);
}

Angle Angle::operator+(const Angle& angle) const
{
	START_PROFILING;
	switch (m_unit)
	{
	case Unit::DEGREE:
		STOP_PROFILING;
		return Angle(m_angle + angle.GetAngleInDegrees(), m_unit);
		break;
	case Unit::RADIAN:
		STOP_PROFILING;
		return Angle(m_angle + angle.GetAngleInRadians(), m_unit);
		break;
	default:
		STOP_PROFILING;
		LOG(Utility::Error, LOGPLACE, "Cannot add two angles. The angle is specified in unknown unit type (%d)", m_unit);
		return Angle(*this);
	}
}

Angle Angle::operator-(const Angle& angle) const
{
	START_PROFILING;
	switch (m_unit)
	{
	case Unit::DEGREE:
		STOP_PROFILING;
		return Angle(m_angle - angle.GetAngleInDegrees(), m_unit);
		break;
	case Unit::RADIAN:
		STOP_PROFILING;
		return Angle(m_angle - angle.GetAngleInRadians(), m_unit);
		break;
	default:
		STOP_PROFILING;
		LOG(Utility::Error, LOGPLACE, "Cannot add two angles. The angle is specified in unknown unit type (%d)", m_unit);
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
	START_PROFILING;
	switch (m_unit)
	{
	case Unit::DEGREE:
		m_angle += angle.GetAngleInDegrees();
		break;
	case Unit::RADIAN:
		m_angle += angle.GetAngleInRadians();
		break;
	default:
		LOG(Utility::Error, LOGPLACE, "Cannot add two angles. The angle is specified in unknown unit type (%d)", m_unit);
	}
	STOP_PROFILING;
	return *this;
}

Angle& Angle::operator-=(const Angle& angle)
{
	START_PROFILING;
	switch (m_unit)
	{
	case Unit::DEGREE:
		m_angle -= angle.GetAngleInDegrees();
		break;
	case Unit::RADIAN:
		m_angle -= angle.GetAngleInRadians();
		break;
	default:
		LOG(Utility::Error, LOGPLACE, "Cannot add two angles. The angle is specified in unknown unit type (%d)", m_unit);
	}
	STOP_PROFILING;
	return *this;
}

Angle& Angle::operator*=(Real s)
{
	START_PROFILING;
	m_angle *= s;
	STOP_PROFILING;
	return *this;
}

Angle& Angle::operator/=(Real s)
{
	START_PROFILING;
	m_angle /= s;
	STOP_PROFILING;
	return *this;
}

Angle& Angle::operator=(const Angle& angle)
{
	switch (angle.GetUnit())
	{
	case Unit::DEGREE:
		m_angle = angle.GetAngleInDegrees();
		break;
	case Unit::RADIAN:
		m_angle = angle.GetAngleInRadians();
		break;
	default:
		LOG(Utility::Error, LOGPLACE, "The angle is specified in unknown unit type (%d)", angle.GetUnit());
	}
	m_unit = angle.GetUnit();
	return (*this);
}

bool Angle::operator==(const Angle& angle) const
{
	START_PROFILING;
	const Real angleRad = angle.GetAngleInRadians();
	const Real rad = this->GetAngleInRadians();
	bool areAnglesEqual = AlmostEqual(angleRad, rad);
	STOP_PROFILING;
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
	ss << GetAngleInDegrees() << L'\u2103';
	return ss.str();
}