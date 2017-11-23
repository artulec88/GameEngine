#ifndef __RENDERING_ATTENUATION_H__
#define __RENDERING_ATTENUATION_H__

#include "Rendering.h"
#include "Math/Math.h"

namespace rendering
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
		RENDERING_API explicit constexpr Attenuation(const math::Real constant = REAL_ZERO,
			const math::Real linear = REAL_ZERO, const math::Real exponent = REAL_ONE) noexcept :
			m_constant(constant),
			m_linear(linear),
			m_exponent(exponent)
		{
		}

		/// <summary> The attenuation destructor </summary>
		RENDERING_API ~Attenuation() noexcept = default;

		/// <summary> Attenuation copy constructor. </summary>
		/// <param name="attenuation"> The attenuation to copy construct from. </param>
		RENDERING_API constexpr Attenuation(const Attenuation& attenuation) noexcept = default;

		/// <summary> Attenuation move constructor. </summary>
		/// <param name="attenuation"> The attenuation to move construct from. </param>
		RENDERING_API constexpr Attenuation(Attenuation&& attenuation) noexcept = default;

		/// <summary> Attenuation copy assignment operator. </summary>
		/// <param name="attenuation"> The attenuation to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned attenuation. </returns>
		RENDERING_API Attenuation& operator=(const Attenuation& attenuation) noexcept = default;

		/// <summary> Attenuation move assignment operator. </summary>
		/// <param name="attenuation"> The r-value reference of the attenuation to move assign from. </param>
		/// <returns> The reference to the newly move-assigned attenuation. </returns>
		RENDERING_API Attenuation& operator=(Attenuation&& attenuation) noexcept = default;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		/// <summary> Gets the constant factor of the attenuation. </summary>
		/// <returns> The constant factor in gradual loss of attenuation. </returns>
		RENDERING_API constexpr math::Real GetConstant() const noexcept { return m_constant; }

		/// <summary> Gets the linear factor of the attenuation. </summary>
		/// <returns> The linear factor in gradual loss of attenuation. </returns>
		RENDERING_API constexpr math::Real GetLinear() const noexcept { return m_linear; }

		/// <summary> Gets the exponent factor of the attenuation. </summary>
		/// <returns> The exponent factor in gradual loss of attenuation. </returns>
		RENDERING_API constexpr math::Real GetExponent() const noexcept { return m_exponent; }
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		/// <summary>
		/// The constant attribute of attenuation. Determines the constant gradual loss in intensity.
		/// </summary>
		math::Real m_constant;

		/// <summary>
		/// The linear attribute of attenuation. Determines the gradual loss (growing linearly) in intensity.
		/// </summary>
		math::Real m_linear;

		/// <summary>
		/// The exponent attribute of attenuation. Determines the gradual loss (growing exponentially) in intensity.
		/// </summary>
		math::Real m_exponent;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class Attenuation */
} /* end namespace rendering */

#endif /* __RENDERING_ATTENUATION_H__ */