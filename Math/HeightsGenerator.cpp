#include "stdafx.h"
#include "HeightsGenerator.h"

Math::HeightsGenerator::HeightsGenerator(Math::Real heightAmplitude) :
	m_heightAmplitude(heightAmplitude)
{
}

Math::HeightsGenerator::~HeightsGenerator()
{
}

Math::Real Math::HeightsGenerator::GenerateHeight(Real x, Real z) const
{
	return REAL_ZERO;
}