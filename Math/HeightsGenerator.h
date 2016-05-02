#ifndef __MATH_HEIGHTS_GENERATOR_H__
#define __MATH_HEIGHTS_GENERATOR_H__

#include "Math.h"

namespace Math
{
	class HeightsGenerator
	{
	/* ==================== Static variables and functions begin ==================== */
	/* ==================== Static variables and functions end ==================== */

	/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary>
		/// Creates a heights generator.
		/// </summary>
		/// <param name="heightAmplitude">Defines the amplitude of the generated heights. The final heights cannot exceed the specified amplitude.</param>
		/// <remarks>
		/// </remarks>
		MATH_API HeightsGenerator(Real heightAmplitude);

		/// <summary>
		/// Destroys the heights generator.
		/// </summary>
		MATH_API ~HeightsGenerator(void);
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		MATH_API Real GenerateHeight(Real x, Real z) const;
	private:
		/// <summary>
		/// Generates random number for a pair of floating-point values. It returns a random floating-point value in range from <code>-1</code> to <code>1</code> (inclusively).
		/// For a given pair of floats the function will always return the same result during runtime, although this value will be random.
		/// Once the heights generator is created the noise function mustn't change.
		/// </summary>
		/// <returns> The random number between <code>-1</code> and <code>1</code>.
		MATH_API Real GetNoise(Real x, Real z) const;
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
		/// <summary>
		/// Defines the amplitude of the generated heights. The final heights cannot exceed the specified amplitude.
		/// </summary>
		Real m_heightAmplitude;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class HeightsGenerator */

} /* end namespace Math */

#endif // __MATH_HEIGHTS_GENERATOR_H__
