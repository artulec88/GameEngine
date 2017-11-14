#include "stdafx.h"
#include "HeightsGenerator.h"
#include "Interpolation_impl.h"
#include "FloatingPoint.h"

#include "Utility\ILogger.h"


Math::HeightsGenerator::HeightsGenerator(int gridX, int gridZ, int vertexCount, Math::Real heightAmplitude, int octaves, Math::Real roughness) :
	m_offsetX(gridX * (vertexCount - 1)),
	m_offsetZ(gridZ * (vertexCount - 1)),
	m_heightAmplitude(heightAmplitude),
	m_octaves(octaves),
	m_freqFactor(3.0f), /* TODO: Don't use hard-coded values */
	m_roughness(roughness),
	m_smoothCornersFactor(16.0f),
	m_smoothSidesFactor(8.0f),
	m_smoothCenterFactor(4.0f),
	m_interpolationType(Interpolation::InterpolationTypes::COSINE),
	m_randomGenerator(Random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(Math::Random::GeneratorIDs::SIMPLE, 1000000000))
{
	/* ==================== Small unit test to check whether GetNoise function always returns the same output for a given input begin ==================== */
	// TODO: In the future this test should be moved to MathTest project.
	int x = 5;
	int z = 14;
	Real noise1 = GetNoise(x, z);
	Real noise2 = GetNoise(x, z);
	CHECK_CONDITION_EXIT_ALWAYS_MATH(AlmostEqual(noise1, noise2), utility::logging::ERR,
		"The noise function does not return the same output for a given input (", noise1, " and ", noise2, ")");
	x += 1;
	z += -4;
	Real noise3 = GetNoise(x, z);
	Real noise4 = GetNoise(x, z);
	CHECK_CONDITION_EXIT_ALWAYS_MATH(AlmostEqual(noise3, noise4), utility::logging::ERR,
		"The noise function does not return the same output for a given input (", noise3, " and ", noise4, ")");
	CHECK_CONDITION_EXIT_ALWAYS_MATH(!AlmostEqual(noise1, noise3), utility::logging::ERR,
		"The noise function always returns the same output for any input (", noise1, ")");
	/* ==================== Small unit test to check whether GetNoise function always returns the same output for a given input end ==================== */
}

Math::HeightsGenerator::~HeightsGenerator()
{
}

Math::Real Math::HeightsGenerator::GenerateHeight(Real x, Real z) const
{
	Real totalHeight = REAL_ZERO;
	Real d = pow(m_freqFactor, m_octaves - 1);
	for (int i = 0; i < m_octaves; ++i)
	{
		const Real freq = pow(m_freqFactor, i) / d;
		const Real amplitude = pow(m_roughness, i) * m_heightAmplitude;
		totalHeight += GetInterpolatedNoise((x + m_offsetX) * freq, (z + m_offsetZ) * freq) * amplitude;
	}
	return totalHeight;
}

Math::Real Math::HeightsGenerator::GetInterpolatedNoise(Real x, Real z) const
{
	// Watch: https://www.youtube.com/watch?v=qChQrNWU9Xw
	Real intPartX, intPartZ;
	const Real fractX = modf(x, &intPartX);
	const Real fractZ = modf(z, &intPartZ);

	const int intX = static_cast<int>(intPartX);
	const int intZ = static_cast<int>(intPartZ);
	
	const Real height1 = GetSmoothNoise(intX, intZ);
	const Real height2 = GetSmoothNoise(intX + 1, intZ);
	const Real height3 = GetSmoothNoise(intX, intZ + 1);
	const Real height4 = GetSmoothNoise(intX + 1, intZ + 1);

	const Math::Real i1 = Interpolation::Interpolate<Real>(m_interpolationType, height1, height2, fractX);
	const Math::Real i2 = Interpolation::Interpolate<Real>(m_interpolationType, height3, height4, fractX);
	return Interpolation::Interpolate<Real>(m_interpolationType, i1, i2, fractZ);
}

Math::Real Math::HeightsGenerator::GetSmoothNoise(int x, int z) const
{
	// TODO: Place for an improvement. Create a GaussianBlur class which would be able to smooth noise values a little bit more efficiently.
	const Real corners = (GetNoise(x - 1, z - 1) + GetNoise(x + 1, z - 1) + GetNoise(x - 1, z + 1) + GetNoise(x + 1, z + 1)) / m_smoothCornersFactor;
	const Real sides = (GetNoise(x - 1, z) + GetNoise(x + 1, z) + GetNoise(x, z - 1) + GetNoise(x, z + 1)) / m_smoothSidesFactor;
	const Real center = GetNoise(x, z) / m_smoothCenterFactor;
	return corners + sides + center;
}

Math::Real Math::HeightsGenerator::GetNoise(int x, int z) const
{
	m_randomGenerator.SetSeed(x * 49632 + z * 325176 + m_randomGenerator.GetSeed());
	return m_randomGenerator.NextFloat(-1.0f, 1.0f);
}