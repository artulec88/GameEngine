#include "StdAfx.h"
#include "Angle.h"
#include "FloatingPoint.h"
#include "Utility\ILogger.h"
#include <sstream>

using namespace Math;

Angle::Angle(Real angle /* = REAL_ZERO */, Unit::UnitType unit /* = DEGREE */) :
	m_angle(angle),
	m_unit(unit)
#ifdef COUNT_STATS
	,m_degToRadConversionCount(0),
	m_radToDegConversionCount(0)
#endif
{
}

Angle::Angle(const Angle& angle) :
	m_angle(angle.GetAngleInRadians()),
	m_unit(Unit::RADIAN)
#ifdef COUNT_STATS
	,m_degToRadConversionCount(0),
	m_radToDegConversionCount(0)
#endif
{
}

Angle::~Angle()
{
#ifdef COUNT_STATS
	LOG(Utility::Debug, LOGPLACE, "The conversions between radians and degrees units report: ToRad: %d\t ToDeg: %d", m_degToRadConversionCount, m_radToDegConversionCount);
#endif
}

// TODO: move this function to *.h file as an inline function
Real Angle::GetAngleInDegrees() const
{
	switch (m_unit)
	{
	case Unit::DEGREE:
		return m_angle;
	case Unit::RADIAN:
#ifdef COUNT_STATS
		++m_radToDegConversionCount;
#endif
		return ToDeg(m_angle);
	default:
		LOG(Utility::Error, LOGPLACE, "Incorrect unit type for angle with amount=%.2f and unit=%d", m_angle, m_unit);
		return m_angle;
	}
}


// TODO: move this function to *.h file as an inline function
Real Angle::GetAngleInRadians() const
{
	switch (m_unit)
	{
	case Unit::DEGREE:
#ifdef COUNT_STATS
		++m_degToRadConversionCount;
#endif
		return ToRad(m_angle);
	case Unit::RADIAN:
		return m_angle;
	default:
		LOG(Utility::Error, LOGPLACE, "Incorrect unit type for angle with amount=%.2f and unit=%d", m_angle, m_unit);
		return m_angle;
	}
}

Angle Angle::operator-() const
{
	return Angle(-m_angle, m_unit);
}

Angle Angle::operator+(const Angle& angle) const
{
	switch (m_unit)
	{
	case Unit::DEGREE:
		return Angle(m_angle + angle.GetAngleInDegrees(), m_unit);
		break;
	case Unit::RADIAN:
		return Angle(m_angle + angle.GetAngleInRadians(), m_unit);
		break;
	default:
		LOG(Utility::Error, LOGPLACE, "Cannot add two angles. The angle is specified in unknown unit type (%d)", m_unit);
		return Angle(*this);
	}
}

Angle Angle::operator-(const Angle& angle) const
{
	switch (m_unit)
	{
	case Unit::DEGREE:
		return Angle(m_angle - angle.GetAngleInDegrees(), m_unit);
		break;
	case Unit::RADIAN:
		return Angle(m_angle - angle.GetAngleInRadians(), m_unit);
		break;
	default:
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

Angle& Angle::operator+=(const Angle& angle) const
{
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
	return *this;
}

Angle& Angle::operator-=(const Angle& angle) const
{
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
	return *this;
}

Angle& Angle::operator*=(Real s) const
{
	m_angle *= s;
	return *this;
}

Angle& Angle::operator/=(Real s) const
{
	m_angle /= s;
	return *this;
}

bool Angle::operator==(const Angle& angle) const
{
	const Real angleRad = angle.GetAngleInRadians();
	const Real rad = this->GetAngleInRadians();
	return (AlmostEqual(angleRad, rad));
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
	ss << GetAngleInDegrees() << "\u2103";
	return ss.str();
}