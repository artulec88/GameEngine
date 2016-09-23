#ifndef __RENDERING_ATTENUATION_H__
#define __RENDERING_ATTENUATION_H__

#include "Rendering.h"
#include "Math\Math.h"

namespace Rendering
{

	/// <summary> The class representing the gradual loss in intensity of light. </summary>
	class Attenuation
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> The constructor. </summary>
		/// <param name="constant"> Constant attribute of attenuation </param>
		/// <param name="linear"> Linear attribute of attenuation </param>
		/// <param name="exponent"> Exponent attribute of attenuation </param>
		RENDERING_API explicit Attenuation(Math::Real constant = REAL_ZERO, Math::Real linear = REAL_ZERO, Math::Real exponent = REAL_ONE) :
			m_constant(constant),
			m_linear(linear),
			m_exponent(exponent)
		{
		}
		/// <summary> The destructor </summary>
		RENDERING_API ~Attenuation(void)
		{
		}

		/// <summary> Attenuation copy constructor. </summary>
		RENDERING_API Attenuation(const Attenuation& attenuation) = default;
		/// <summary> Attenuation move constructor. </summary>
		RENDERING_API Attenuation(Attenuation&& attenuation) = default;
		/// <summary> Attenuation copy assignment operator. </summary>
		RENDERING_API Attenuation& operator=(const Attenuation& attenuation) = default;
		/// <summary> Attenuation move assignment operator. </summary>
		RENDERING_API Attenuation& operator=(Attenuation&& attenuation) = default;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	public:
		RENDERING_API inline Math::Real GetConstant() const { return m_constant; };
		RENDERING_API inline Math::Real GetLinear() const { return m_linear; };
		RENDERING_API inline Math::Real GetExponent() const { return m_exponent; };
		/* ==================== Non-static member variables end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		/// <summary>
		/// The constant attribute of attenuation. Determines the constant gradual loss in intensity.
		/// </summary>
		Math::Real m_constant;

		/// <summary>
		/// The linear attribute of attenuation. Determines the gradual loss (growing linearly) in intensity.
		/// </summary>
		Math::Real m_linear;

		/// <summary>
		/// The exponent attribute of attenuation. Determines the gradual loss (growing exponentially) in intensity.
		/// </summary>
		Math::Real m_exponent;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class Attenuation */

} /* end namespace Rendering */

#endif /* __RENDERING_ATTENUATION_H__ */