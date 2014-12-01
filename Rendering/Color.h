#pragma once

#include "Math\Math.h"

namespace Rendering
{

class Color
{
/* ==================== Constructors and destructors begin ==================== */
public:
	Color(void) : red(0.0f), green(0.0f), blue(0.0f), alpha(1.0f) { }
	Color(Math::Real red, Math::Real green, Math::Real blue, Math::Real alpha = 1.0f) : red(red), green(green), blue(blue), alpha(alpha) { };
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static, non-virtual member functions begin ==================== */
public:
	Math::Real GetRed() const { return red; }
	Math::Real GetGreen() const { return green; }
	Math::Real GetBlue() const { return blue; }
	Math::Real GetAlpha() const { return alpha; }

	void SetRed(Math::Real red) { this->red = red; }
	void SetGreen(Math::Real green) { this->green = green; }
	void SetBlue(Math::Real blue) { this->blue = blue; }
	void SetAlpha(Math::Real alpha) { this->alpha = alpha; }
/* ==================== Non-static, non-virtual member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	Math::Real red, green, blue;
	Math::Real alpha;
/* ==================== Non-static member variables end ==================== */
}; /* end class Color */

} /* end namespace Rendering */