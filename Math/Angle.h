#pragma once

#include "Math.h"
//#include "Utility\ISerializable.h"
#include <string>
#include <sstream>

namespace Math
{

class MATH_API Angle
{
public:
	enum Unit { DEGREE = 0, RADIAN };
public: // constructors and destructors
	explicit Angle(Real angle = REAL_ZERO, Unit unit = DEGREE); // Explicit constructor, so that Real cannot be easily cast to Angle
	Angle(const Angle& angle); // copy constructor

public: // non-static member functions
	Real GetAngleInDegrees() const;
	Real GetAngleInRadians() const;
	bool IsInDegrees() const { return (unit == DEGREE); }
	bool operator==(const Angle& angle) const;
	bool operator!=(const Angle& angle) const;
	bool operator>(const Angle& angle) const;
	bool operator>=(const Angle& angle) const;
	bool operator<(const Angle& angle) const;
	bool operator<=(const Angle& angle) const;
    
private: // non-static member variables
	Real angle;
	Unit unit;
}; /* end class Angle */
  
} /* end namespace Math */
