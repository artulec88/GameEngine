#pragma once

#include "Math.h"
//#include "Utility\ISerializable.h"
#include <string>
#include <sstream>

namespace Math
{

  class Angle
  {
  public: // constructors and destructors
    Angle(Real angle, bool isInDegrees = true);

  public: // non-static member functions
    Real GetAngleInDegrees() const;
    Real GetAngleInRadians() const;
    bool IsInDegrees() const { return isInDegrees; }
    
  private: // non-static member variables
    Real angle;
    const bool isInDegrees;
  }; /* end class Angle */
  
} /* end namespace Math */
