#ifndef __MATH_HEIGHTS_GENERATOR_H__
#define __MATH_HEIGHTS_GENERATOR_H__

#include "Math.h"
#include "RandomGeneratorFactory.h"
#include "Interpolation.h"

namespace math
{
	/// <summary> The random floating-point values generator for a given input positions. </summary>
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
		/// <param name="vertexCount"> The number of vertices along one grid side. </param>
		/// <param name="heightAmplitude">Defines the amplitude of the generated heights. The final heights cannot exceed the specified amplitude.</param>
		/// <param name="octaves"> The number of noise functions we want to use for the heights generator. </param>
		/// <param name="roughness"> The roughness of the heights generator. It indicates how much we decrease the amplitude by for each higher-frequency octave. </param>
		/// <remarks>
		/// </remarks>
		MATH_API HeightsGenerator(int gridX, int gridZ, int vertexCount, Real heightAmplitude, int octaves, Real roughness);

		/// <summary>
		/// Destroys the heights generator.
		/// </summary>
		MATH_API ~HeightsGenerator();

		/// <summary> Heights generator copy constructor. </summary>
		/// <param name="heightsGenerator"> The heights generator to copy construct from. </param>
		MATH_API HeightsGenerator(const HeightsGenerator& heightsGenerator) = delete;

		/// <summary> Heights generator move constructor. </summary>
		/// <param name="heightsGenerator"> The heights generator to move construct from. </param>
		MATH_API HeightsGenerator(HeightsGenerator&& heightsGenerator) = delete;

		/// <summary> Heights generator copy assignment operator. </summary>
		/// <param name="heightsGenerator"> The heights generator to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned heights generator. </returns>
		MATH_API HeightsGenerator& operator=(const HeightsGenerator& heightsGenerator) = delete;

		/// <summary> Heights generator move assignment operator. </summary>
		/// <param name="heightsGenerator"> The heights generator to move assign from. </param>
		/// <returns> The reference to the newly move-assigned heights generator. </returns>
		MATH_API HeightsGenerator& operator=(HeightsGenerator&& heightsGenerator) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		/// <summary> Generates a random (but constant for the given input parameters) value (height) for the specified input position. </summary>
		/// <param name="x"> The X component of the position we want to generate height for. </param>
		/// <param name="z"> The Z component of the position we want to generate height for. </param>
		/// <returns> The value (height) generated for the given input position [<paramref name="x"/>, <paramref name="z"/>]. </returns>
		MATH_API Real GenerateHeight(Real x, Real z) const;
	private:
		/// <summary>
		/// Generates random number for a pair of floating-point values. It returns a random floating-point value in range from <code>-1</code> to <code>1</code> (inclusively).
		/// For a given pair of floats the function will always return the same result during runtime, although this value will be random.
		/// Once the heights generator is created the noise function mustn't change.
		/// </summary>
		/// <returns> The random number between <code>-1</code> and <code>1</code>.</returns>
		Real GetInterpolatedNoise(Real x, Real z) const;

		/// <summary>
		/// Generates random number for a pair of floating-point values. It returns a random floating-point value in range from <code>-1</code> to <code>1</code> (inclusively).
		/// For a given pair of floats the function will always return the same result during runtime, although this value will be random.
		/// Once the heights generator is created the noise function mustn't change.
		/// </summary>
		/// <returns> The random number between <code>-1</code> and <code>1</code>.</returns>
		Real GetSmoothNoise(int x, int z) const;

		/// <summary>
		/// Generates random number for a pair of floating-point values. It returns a random floating-point value in range from <code>-1</code> to <code>1</code> (inclusively).
		/// For a given pair of floats the function will always return the same result during runtime, although this value will be random.
		/// Once the heights generator is created the noise function mustn't change.
		/// </summary>
		/// <param name="x"> The X component of the position we want to calculate noise for. </param>
		/// <param name="y"> The Y component of the position we want to calculate noise for. </param>
		/// <returns> The random number between <code>-1</code> and <code>1</code>.</returns>
		Real GetNoise(int x, int z) const;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		const int m_offsetX;
		const int m_offsetZ;

		/// <summary>
		/// Defines the amplitude of the generated heights. The final heights cannot exceed the specified amplitude.
		/// </summary>
		Real m_heightAmplitude;

		/// <summary>
		/// The number of noise functions that we are going to use during heights generation process.
		/// </summary>
		int m_octaves;

		/// <summary>
		/// The frequency factor. The higher the value the more...
		/// </summary>
		Real m_freqFactor;

		/// <summary>
		/// The amount that we decrease the amplitude by for each higher-frequency octave.
		/// In other words, this value will determine how rough the final output of the generation process (e.g. terrain) will be.
		/// </summary>
		Real m_roughness;

		Real m_smoothCornersFactor;
		Real m_smoothSidesFactor;
		Real m_smoothCenterFactor;

		/// <summary>
		/// The interpolator type we use in heights generation process.
		/// </summary>
		interpolation::interpolation_types::InterpolationType m_interpolationType;

		const random::RandomGenerator& m_randomGenerator;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class HeightsGenerator */

} /* end namespace math */

#endif // __MATH_HEIGHTS_GENERATOR_H__
