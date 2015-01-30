#include "StdAfx.h"
#include "Angle.h"
#include "FloatingPoint.h"
#include "Utility\ILogger.h"

using namespace Math;

Angle::Angle(Real angle /* = REAL_ZERO */, Unit unit /* = DEGREE */) :
	angle(angle),
	unit(unit)
{
}

Angle::Angle(const Angle& angle) :
	angle(angle.GetAngleInRadians()),
	unit(RADIAN)
{
}

// TODO: move this function to *.h file as an inline function
Real Angle::GetAngleInDegrees() const
{
	switch (unit)
	{
	case DEGREE:
		return angle;
	case RADIAN:
		return ToDeg(angle);
	default:
		LOG(Utility::Error, LOGPLACE, "Incorrect unit type for angle with amount=%.2f and unit=%d", angle, unit);
		return angle;
	}
}


// TODO: move this function to *.h file as an inline function
Real Angle::GetAngleInRadians() const
{
	switch (unit)
	{
	case DEGREE:
		return ToRad(angle);
	case RADIAN:
		return angle;
	default:
		LOG(Utility::Error, LOGPLACE, "Incorrect unit type for angle with amount=%.2f and unit=%d", angle, unit);
		return angle;
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
