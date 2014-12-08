#pragma once

#include "Rendering.h"
#include "Math\Math.h"

namespace Rendering
{

class RENDERING_API Attenuation
{
/* ==================== Static variables and functions begin ==================== */
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	Attenuation(Math::Real constant = REAL_ZERO, Math::Real linear = REAL_ZERO, Math::Real exponent = REAL_ONE);
	~Attenuation(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member variables begin ==================== */
public:
	Math::Real GetConstant() const { return this->constant; };
	Math::Real GetLinear() const { return this->linear; };
	Math::Real GetExponent() const { return this->exponent; };
/* ==================== Non-static member variables end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	Math::Real constant;
	Math::Real linear;
	Math::Real exponent;
/* ==================== Non-static member variables end ==================== */
}; /* end class Attenuation */

} /* end namespace Rendering */