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
	Attenuation(Math::Real constant = REAL_ZERO, Math::Real linear = REAL_ZERO, Math::Real exponent = REAL_ONE) :
		m_constant(constant),
		m_linear(linear),
		m_exponent(exponent)
	{
	}
	~Attenuation(void)
	{
	}
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member variables begin ==================== */
public:
	Math::Real GetConstant() const { return m_constant; };
	Math::Real GetLinear() const { return m_linear; };
	Math::Real GetExponent() const { return m_exponent; };
/* ==================== Non-static member variables end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	Math::Real m_constant;
	Math::Real m_linear;
	Math::Real m_exponent;
/* ==================== Non-static member variables end ==================== */
}; /* end class Attenuation */

} /* end namespace Rendering */