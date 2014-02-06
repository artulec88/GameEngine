#pragma once

#include "Math.h"
#include "Utility\ISerializable.h"
#include <string>
#include <sstream>

namespace Math
{
class MATH_API Point : public Utility::ISerializable
{
public: // constructors and destructors
	Point() : m_x(0), m_y(0) { };
	Point(Real x, Real y) : m_x(x), m_y(y) { };

public: // public member functions
	Real GetX() const { return m_x; };
	Real GetY() const { return m_y; };
	void SetX(Real x) { m_x = x; };
	void SetY(Real y) { m_y = y; };

	Real Length() const;
	Real LengthSquared() const;
	
	void Negate() { m_x = -m_x; m_y = -m_y; }; // creates negation vector
	
	Point operator+(const Point& v) const { return Point(m_x + v.GetX(), m_y + v.GetY()); };
	Point operator-() const { return Point(-m_x, -m_y); };
	Point operator-(const Point& v) const { return Point(m_x - v.GetX(), m_y - v.GetY()); };
	Point operator*(Real s) const { return Point(s * m_x, s * m_y); };
	Point operator/(Real s) const { return Point(m_x / s, m_y / s); };

	Point Normalized() const;
	void Normalize();
	
	Real Dot(const Point& v) const;

public:
	virtual std::string ToString() const; // derived from Utility::ISerializable

protected: // member variables
	Real m_x;
	Real m_y;
}; /* end class Point */

} /* end namespace Math */