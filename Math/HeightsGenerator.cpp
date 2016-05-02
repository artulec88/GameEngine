#include "stdafx.h"
#include "HeightsGenerator.h"
#include "Utility\ILogger.h"
#include "FloatingPoint.h"

Math::HeightsGenerator::HeightsGenerator(Math::Real heightAmplitude) :
	m_heightAmplitude(heightAmplitude),
	m_smoothCornersFactor(16.0f),
	m_smoothSidesFactor(8.0f),
	m_smoothCenterFactor(4.0f),
	m_randomGenerator(Random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(Math::Random::Generators::SIMPLE, 1000000000))
{
	/* ==================== Small unit test to check whether GetNoise function always returns the same output for a given input begin ==================== */
	Real x = 5.0f;
	Real z = 14.0f;
	Real noise1 = GetNoise(x, z);
	Real noise2 = GetNoise(x, z);
	CHECK_CONDITION_EXIT_ALWAYS(AlmostEqual(noise1, noise2), Utility::Error, "The noise function does not return the same output for a given input (%.4f and %.4f)", noise1, noise2);
	x += 1.0f;
	z += -4.0f;
	Real noise3 = GetNoise(x, z);
	Real noise4 = GetNoise(x, z);
	CHECK_CONDITION_EXIT_ALWAYS(AlmostEqual(noise3, noise4), Utility::Error, "The noise function does not return the same output for a given input (%.4f and %.4f)", noise3, noise4);
	CHECK_CONDITION_EXIT_ALWAYS(!AlmostEqual(noise1, noise3), Utility::Error, "The noise function always returns the same output for any input (%.4f)", noise1);
	/* ==================== Small unit test to check whether GetNoise function always returns the same output for a given input begin ==================== */
}

Math::HeightsGenerator::~HeightsGenerator()
{
}

Math::Real Math::HeightsGenerator::GenerateHeight(Real x, Real z) const
{
	return GetSmoothNoise(x, z) * m_heightAmplitude;
}

Math::Real Math::HeightsGenerator::GetSmoothNoise(Real x, Real z) const
{
	//m_randomGenerator.SetSeed(x * 49632 + z * 325176 + m_randomGenerator.GetSeed());
	Real corners = (GetNoise(x - 1, z - 1) + GetNoise(x + 1, z - 1) + GetNoise(x - 1, z + 1) + GetNoise(x + 1, z + 1)) / m_smoothCornersFactor;
	Real sides = (GetNoise(x - 1, z) + GetNoise(x + 1, z) + GetNoise(x, z - 1) + GetNoise(x, z + 1)) / m_smoothSidesFactor;
	Real center = GetNoise(x, z) / m_smoothCenterFactor;
	return corners + sides + center;
}

Math::Real Math::HeightsGenerator::GetNoise(Real x, Real z) const
{
	m_randomGenerator.SetSeed(x * 49632 + z * 325176 + m_randomGenerator.GetSeed());
	return m_randomGenerator.NextFloat(-1.0f, 1.0f);
}