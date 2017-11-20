#include "stdafx.h"
#include "HeightsGenerator.h"
#include "Interpolation_impl.h"
#include "FloatingPoint.h"

#include "Utility/ILogger.h"


math::HeightsGenerator::HeightsGenerator(int gridX, int gridZ, int vertexCount, Real heightAmplitude, int octaves, Real roughness) :
	m_offsetX(gridX * (vertexCount - 1)),
	m_offsetZ(gridZ * (vertexCount - 1)),
	m_heightAmplitude(heightAmplitude),
	m_octaves(octaves),
	m_freqFactor(3.0f), /* TODO: Don't use hard-coded values */
	m_roughness(roughness),
	m_smoothCornersFactor(16.0f),
	m_smoothSidesFactor(8.0f),
	m_smoothCenterFactor(4.0f),
	m_interpolationType(interpolation::interpolation_types::COSINE),
	m_randomGenerator(random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(random::generator_ids::SIMPLE, 1000000000))
{
	/* ==================== Small unit test to check whether GetNoise function always returns the same output for a given input begin ==================== */
	// TODO: In the future this test should be moved to MathTest project.
	auto x = 5;
	auto z = 14;
	auto noise1 = GetNoise(x, z);
	auto noise2 = GetNoise(x, z);
	CHECK_CONDITION_EXIT_ALWAYS_MATH(AlmostEqual(noise1, noise2), utility::logging::ERR,
		"The noise function does not return the same output for a given input (", noise1, " and ", noise2, ")");
	x += 1;
	z += -4;
	auto noise3 = GetNoise(x, z);
	auto noise4 = GetNoise(x, z);
	CHECK_CONDITION_EXIT_ALWAYS_MATH(AlmostEqual(noise3, noise4), utility::logging::ERR,
		"The noise function does not return the same output for a given input (", noise3, " and ", noise4, ")");
	CHECK_CONDITION_EXIT_ALWAYS_MATH(!AlmostEqual(noise1, noise3), utility::logging::ERR,
		"The noise function always returns the same output for any input (", noise1, ")");
	/* ==================== Small unit test to check whether GetNoise function always returns the same output for a given input end ==================== */
}

math::HeightsGenerator::~HeightsGenerator()
{
}

math::Real math::HeightsGenerator::GenerateHeight(Real x, Real z) const
{
	auto totalHeight = REAL_ZERO;
	const auto d = pow(m_freqFactor, m_octaves - 1);
	for (auto i = 0; i < m_octaves; ++i)
	{
		const auto freq = pow(m_freqFactor, i) / d;
		const auto amplitude = pow(m_roughness, i) * m_heightAmplitude;
		totalHeight += GetInterpolatedNoise((x + m_offsetX) * freq, (z + m_offsetZ) * freq) * amplitude;
	}
	return totalHeight;
}

math::Real math::HeightsGenerator::GetInterpolatedNoise(Real x, Real z) const
{
	// Watch: https://www.youtube.com/watch?v=qChQrNWU9Xw
	Real intPartX, intPartZ;
	const auto fractX = modf(x, &intPartX);
	const auto fractZ = modf(z, &intPartZ);

	const auto intX = static_cast<int>(intPartX);
	const auto intZ = static_cast<int>(intPartZ);
	
	const auto height1 = GetSmoothNoise(intX, intZ);
	const auto height2 = GetSmoothNoise(intX + 1, intZ);
	const auto height3 = GetSmoothNoise(intX, intZ + 1);
	const auto height4 = GetSmoothNoise(intX + 1, intZ + 1);

	const auto i1 = interpolation::Interpolate<Real>(m_interpolationType, height1, height2, fractX);
	const auto i2 = interpolation::Interpolate<Real>(m_interpolationType, height3, height4, fractX);
	return interpolation::Interpolate<Real>(m_interpolationType, i1, i2, fractZ);
}

math::Real math::HeightsGenerator::GetSmoothNoise(int x, int z) const
{
	// TODO: Place for an improvement. Create a GaussianBlur class which would be able to smooth noise values a little bit more efficiently.
	const auto corners = (GetNoise(x - 1, z - 1) + GetNoise(x + 1, z - 1) + GetNoise(x - 1, z + 1) + GetNoise(x + 1, z + 1)) / m_smoothCornersFactor;
	const auto sides = (GetNoise(x - 1, z) + GetNoise(x + 1, z) + GetNoise(x, z - 1) + GetNoise(x, z + 1)) / m_smoothSidesFactor;
	const auto center = GetNoise(x, z) / m_smoothCenterFactor;
	return corners + sides + center;
}

math::Real math::HeightsGenerator::GetNoise(int x, int z) const
{
	m_randomGenerator.SetSeed(x * 49632 + z * 325176 + m_randomGenerator.GetSeed());
	return m_randomGenerator.NextFloat(-1.0f, 1.0f);
}