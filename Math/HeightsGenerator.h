#ifndef __MATH_HEIGHTS_GENERATOR_H__
#define __MATH_HEIGHTS_GENERATOR_H__

#include "Math.h"
#include "RandomGeneratorFactory.h"
#include "Interpolation.h"

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
		/// <param name="gridX"> The x-th grid. </param>
		/// <param name="gridZ"> The z-th grid. </param>
		/// <param name="vertexCount"> The number of vertices along one grid side </param>
		/// <param name="heightAmplitude">Defines the amplitude of the generated heights. The final heights cannot exceed the specified amplitude.</param>
		/// <param name="octaves"> The number of noise functions we want to use for the heights generator. </param>
		/// <param name="roughness"> The roughness of the heights generator. It indicates how much we decrease the amplitude by for each higher-frequency octave. </param>
		/// <remarks>
		/// </remarks>
		MATH_API HeightsGenerator(int gridX, int gridZ, int vertexCount, Real heightAmplitude, int octaves, Math::Real roughness);

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
		Real GetInterpolatedNoise(Real x, Real z) const;

		/// <summary>
		/// Generates random number for a pair of floating-point values. It returns a random floating-point value in range from <code>-1</code> to <code>1</code> (inclusively).
		/// For a given pair of floats the function will always return the same result during runtime, although this value will be random.
		/// Once the heights generator is created the noise function mustn't change.
		/// </summary>
		/// <returns> The random number between <code>-1</code> and <code>1</code>.
		Real GetSmoothNoise(int x, int z) const;

		/// <summary>
		/// Generates random number for a pair of floating-point values. It returns a random floating-point value in range from <code>-1</code> to <code>1</code> (inclusively).
		/// For a given pair of floats the function will always return the same result during runtime, although this value will be random.
		/// Once the heights generator is created the noise function mustn't change.
		/// </summary>
		/// <returns> The random number between <code>-1</code> and <code>1</code>.
		Real GetNoise(int x, int z) const;
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	private:
		const int m_offsetX;
		const int m_offsetZ;

		/// <summary>
		/// Defines the amplitude of the generated heights. The final heights cannot exceed the specified amplitude.
		/// </summary>
		Real m_heightAmplitude;
		int m_octaves;
		Math::Real m_roughness;

		Real m_smoothCornersFactor;
		Real m_smoothSidesFactor;
		Real m_smoothCenterFactor;

		Interpolation::Interpolator<Math::Real>* m_interpolator;

		const Random::RandomGenerator& m_randomGenerator;
	/* ==================== Non-static member variables end ==================== */
	}; /* end class HeightsGenerator */

} /* end namespace Math */

#endif // __MATH_HEIGHTS_GENERATOR_H__
