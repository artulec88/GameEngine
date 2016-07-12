#include "stdafx.h"
#include "HeightsGenerator.h"
#include "Interpolation_impl.h"
#include "Utility\ILogger.h"
#include "FloatingPoint.h"
#include <math.h>

Math::HeightsGenerator::HeightsGenerator(int gridX, int gridZ, int vertexCount, Math::Real heightAmplitude, int octaves, Math::Real roughness) :
	m_offsetX(gridX * (vertexCount - 1)),
	m_offsetZ(gridZ * (vertexCount - 1)),
	m_heightAmplitude(heightAmplitude),
	m_octaves(octaves),
	m_roughness(roughness),
	m_smoothCornersFactor(16.0f),
	m_smoothSidesFactor(8.0f),
	m_smoothCenterFactor(4.0f),
	m_interpolator(std::make_unique<Interpolation::CosineInterpolator<Math::Real>>()),
	m_randomGenerator(Random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(Math::Random::Generators::SIMPLE, 1000000000))
{
	/* ==================== Small unit test to check whether GetNoise function always returns the same output for a given input begin ==================== */
	int x = 5;
	int z = 14;
	Real noise1 = GetNoise(x, z);
	Real noise2 = GetNoise(x, z);
	CHECK_CONDITION_EXIT_ALWAYS_MATH(AlmostEqual(noise1, noise2), Utility::Logging::ERR, "The noise function does not return the same output for a given input (%.4f and %.4f)", noise1, noise2);
	x += 1;
	z += -4;
	Real noise3 = GetNoise(x, z);
	Real noise4 = GetNoise(x, z);
	CHECK_CONDITION_EXIT_ALWAYS_MATH(AlmostEqual(noise3, noise4), Utility::Logging::ERR, "The noise function does not return the same output for a given input (%.4f and %.4f)", noise3, noise4);
	CHECK_CONDITION_EXIT_ALWAYS_MATH(!AlmostEqual(noise1, noise3), Utility::Logging::ERR, "The noise function always returns the same output for any input (%.4f)", noise1);
	/* ==================== Small unit test to check whether GetNoise function always returns the same output for a given input begin ==================== */
}

Math::HeightsGenerator::~HeightsGenerator()
{
}

Math::Real Math::HeightsGenerator::GenerateHeight(Real x, Real z) const
{
	Real totalHeight = REAL_ZERO;
	Real d = pow(3.0f, m_octaves - 1);
	for (int i = 0; i < m_octaves; ++i)
	{
		Real freq = pow(3.0f, i) / d;
		Real amplitude = pow(m_roughness, i) * m_heightAmplitude;
		totalHeight += GetInterpolatedNoise((x + m_offsetX) * freq, (z + m_offsetZ) * freq) * amplitude;
	}
	return totalHeight;
}

Math::Real Math::HeightsGenerator::GetInterpolatedNoise(Real x, Real z) const
{
	// Watch: https://www.youtube.com/watch?v=qChQrNWU9Xw
	Real intPartX, intPartZ;
	Real fractX = modf(x, &intPartX);
	Real fractZ = modf(z, &intPartZ);

	int intX = static_cast<int>(intPartX);
	int intZ = static_cast<int>(intPartZ);
	
	Real height1 = GetSmoothNoise(intX, intZ);
	Real height2 = GetSmoothNoise(intX + 1, intZ);
	Real height3 = GetSmoothNoise(intX, intZ + 1);
	Real height4 = GetSmoothNoise(intX + 1, intZ + 1);

	Math::Real i1 = m_interpolator->Interpolate(height1, height2, fractX);
	Math::Real i2 = m_interpolator->Interpolate(height3, height4, fractX);
	return m_interpolator->Interpolate(i1, i2, fractZ);
}

Math::Real Math::HeightsGenerator::GetSmoothNoise(int x, int z) const
{
	//m_randomGenerator.SetSeed(x * 49632 + z * 325176 + m_randomGenerator.GetSeed());
	Real corners = (GetNoise(x - 1, z - 1) + GetNoise(x + 1, z - 1) + GetNoise(x - 1, z + 1) + GetNoise(x + 1, z + 1)) / m_smoothCornersFactor;
	Real sides = (GetNoise(x - 1, z) + GetNoise(x + 1, z) + GetNoise(x, z - 1) + GetNoise(x, z + 1)) / m_smoothSidesFactor;
	Real center = GetNoise(x, z) / m_smoothCenterFactor;
	return corners + sides + center;
}

Math::Real Math::HeightsGenerator::GetNoise(int x, int z) const
{
	m_randomGenerator.SetSeed(x * 49632 + z * 325176 + m_randomGenerator.GetSeed());
	return m_randomGenerator.NextFloat(-1.0f, 1.0f);
}