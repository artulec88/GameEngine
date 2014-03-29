#include "StdAfx.h"
#include "Angle.h"

using namespace Math;

Angle::Angle(Real angle, bool isInDegrees /* = true */) :
  angle(angle),
  isInDegrees(isInDegrees)
{
}

Math::Real Angle::GetAngleInDegrees() const
{
  return (isInDegrees) ? angle : ToDeg(angle);
}

Math::Real Angle::GetAngleInRadians() const
{
  return (isInDegrees) ? ToRad(angle) : angle;
}
