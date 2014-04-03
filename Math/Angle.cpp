#include "StdAfx.h"
#include "Angle.h"

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
  /**
	 * TODO: This function is rather slow. Maybe before creating FloatingPoint objects compare
	 * the numbers using simple tools, like epsilon?
	 * Additionaly, maybe consider creating a static function in the Math library for comparing numbers?
	 */
	const Real epsilon = static_cast<Real>(0.1);
	const Real angleRad = angle.GetAngleInRadians();
	const Real rad = this->GetAngleInRadians();
	if (abs(angleRad - rad) > epsilon)
	{
	  return false;
  }
	const FloatingPoint<Real> fpAngleRad(angleRad);
	const FloatingPoint<Real> fpRad(rad);
	return (fpAngleRad.AlmostEqual(fpRad));
}
