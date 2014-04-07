#include "StdAfx.h"
#include "Angle.h"
#include "FloatingPoint.h"

using namespace Math;

Angle::Angle(Real angle, bool isInDegrees /* = true */) :
  angle(angle),
  isInDegrees(isInDegrees)
{
}

Angle::Angle(const Angle& angle) :
  angle(angle.GetAngleInRadians()),
  isInDegrees(false)
{
}


// TODO: move this function to *.h file as an inline function
Real Angle::GetAngleInDegrees() const
{
  return (isInDegrees) ? angle : ToDeg(angle);
}


// TODO: move this function to *.h file as an inline function
Real Angle::GetAngleInRadians() const
{
  return (isInDegrees) ? ToRad(angle) : angle;
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
