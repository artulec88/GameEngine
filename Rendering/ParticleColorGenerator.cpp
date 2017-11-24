#include "stdafx.h"
#include "ParticleColorGenerator.h"

#include "Math/RandomGeneratorFactory.h"
#include "ParticleAccelerationGenerator.h"

/* ==================== class ConstantColorGenerator begin ==================== */
rendering::particles::generators::ConstantColorGenerator::ConstantColorGenerator(math::Real red /* = REAL_ONE */, math::Real green /* = REAL_ONE */, math::Real blue /* = REAL_ONE */, math::Real alpha /* = REAL_ONE */) :
	ColorGenerator(),
	m_color(red, green, blue, alpha)
{
}

rendering::particles::generators::ConstantColorGenerator::ConstantColorGenerator(const Color& color) :
	ColorGenerator(),
	m_color(color)
{
}

rendering::particles::generators::ConstantColorGenerator::~ConstantColorGenerator()
{
}

void rendering::particles::generators::ConstantColorGenerator::Generate(math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId)
{
	for (auto i = startId; i < endId; ++i)
	{
		Set(particleContainer, i, m_color);
	}
}
/* ==================== class ConstantColorGenerator end ==================== */

/* ==================== class FromSetColorGenerator begin ==================== */
rendering::particles::generators::FromSetColorGenerator::FromSetColorGenerator(std::initializer_list<Color> colors) :
	ColorGenerator(),
	m_colors(colors),
	m_randomGenerator(math::random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(math::random::generator_ids::SIMPLE))
{
}

rendering::particles::generators::FromSetColorGenerator::~FromSetColorGenerator()
{
}

void rendering::particles::generators::FromSetColorGenerator::Generate(math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId)
{
	for (auto i = startId; i < endId; ++i)
	{
		const size_t colorIndex = m_randomGenerator.NextInt(0, m_colors.size() - 1);
		Set(particleContainer, i, m_colors[colorIndex]);
	}
}
/* ==================== class FromSetColorGenerator end ==================== */

/* ==================== class RangeColorGenerator begin ==================== */
rendering::particles::generators::RangeColorGenerator::RangeColorGenerator(const Color& minColor, const Color& maxColor) :
	ColorGenerator(),
	m_minRed(minColor.GetRed() < maxColor.GetRed() ? minColor.GetRed() : maxColor.GetRed()),
	m_maxRed(minColor.GetRed() > maxColor.GetRed() ? minColor.GetRed() : maxColor.GetRed()),
	m_minGreen(minColor.GetGreen() < maxColor.GetGreen() ? minColor.GetGreen() : maxColor.GetGreen()),
	m_maxGreen(minColor.GetGreen() > maxColor.GetGreen() ? minColor.GetGreen() : maxColor.GetGreen()),
	m_minBlue(minColor.GetBlue() < maxColor.GetBlue() ? minColor.GetBlue() : maxColor.GetBlue()),
	m_maxBlue(minColor.GetBlue() > maxColor.GetBlue() ? minColor.GetBlue() : maxColor.GetBlue()),
	m_minAlpha(minColor.GetAlpha() < maxColor.GetAlpha() ? minColor.GetAlpha() : maxColor.GetAlpha()),
	m_maxAlpha(minColor.GetAlpha() > maxColor.GetAlpha() ? minColor.GetAlpha() : maxColor.GetAlpha()),
	m_randomGenerator(math::random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(math::random::generator_ids::SIMPLE))
{
}

rendering::particles::generators::RangeColorGenerator::~RangeColorGenerator()
{
}

void rendering::particles::generators::RangeColorGenerator::Generate(math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId)
{
	for (auto i = startId; i < endId; ++i)
	{
		Set(particleContainer, i, Color{ m_randomGenerator.NextFloat(m_minRed, m_maxRed), m_randomGenerator.NextFloat(m_minGreen, m_maxGreen),
			m_randomGenerator.NextFloat(m_minBlue, m_maxBlue), m_randomGenerator.NextFloat(m_minAlpha, m_maxAlpha) });
	}
}
/* ==================== class RangeColorGenerator end ==================== */