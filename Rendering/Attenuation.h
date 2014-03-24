#pragma once

#include "Rendering.h"
#include "Math\Math.h"

namespace Rendering
{

class RENDERING_API Attenuation
{
/* ==================== Non-static member variables begin ==================== */
private:
	Math::Real constant;
	Math::Real linear;
	Math::Real exponent;
/* ==================== Non-static member variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	Attenuation(Math::Real constant = 0.0, Math::Real linear = 0.0, Math::Real exponent = 0.0);
	~Attenuation(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member variables begin ==================== */
public:
	Math::Real GetConstant() const { return this->constant; };
	Math::Real GetLinear() const { return this->linear; };
	Math::Real GetExponent() const { return this->exponent; };
/* ==================== Non-static member variables end ==================== */
}; /* end class Attenuation */

} /* end namespace Rendering */