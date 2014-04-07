#pragma once

#include "Math.h"
//#include "Utility\ISerializable.h"
#include <string>
#include <sstream>

namespace Math
{

  class MATH_API Angle
  {
  public: // constructors and destructors
    Angle(Real angle, bool isInDegrees = true);
    Angle(const Angle& angle); // copy constructor

  public: // non-static member functions
    Real GetAngleInDegrees() const;
    Real GetAngleInRadians() const;
    bool IsInDegrees() const { return isInDegrees; }
    bool operator==(const Angle& angle) const;
    bool operator!=(const Angle& angle) const;
    bool operator>(const Angle& angle) const;
    bool operator>=(const Angle& angle) const;
    bool operator<(const Angle& angle) const;
    bool operator<=(const Angle& angle) const;
    
  private: // non-static member variables
    Real angle;
    const bool isInDegrees;
  }; /* end class Angle */
  
} /* end namespace Math */
