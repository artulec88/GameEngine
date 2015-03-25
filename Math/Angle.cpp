#include "StdAfx.h"
#include "Angle.h"
#include "FloatingPoint.h"
#include "Utility\ILogger.h"

using namespace Math;

Angle::Angle(Real angle /* = REAL_ZERO */, Unit::UnitType unit /* = DEGREE */) :
	m_angle(angle),
	m_unit(unit)
{
}

Angle::Angle(const Angle& angle) :
	m_angle(angle.GetAngleInRadians()),
	m_unit(Unit::RADIAN)
{
}

// TODO: move this function to *.h file as an inline function
Real Angle::GetAngleInDegrees() const
{
	switch (m_unit)
	{
	case Unit::DEGREE:
		return m_angle;
	case Unit::RADIAN:
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
		return ToRad(m_angle);
	case Unit::RADIAN:
		return m_angle;
	default:
		LOG(Utility::Error, LOGPLACE, "Incorrect unit type for angle with amount=%.2f and unit=%d", m_angle, m_unit);
		return m_angle;
	}
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
