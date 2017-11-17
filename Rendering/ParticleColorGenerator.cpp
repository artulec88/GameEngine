#include "stdafx.h"
#include "ParticleColorGenerator.h"

#include "Math/RandomGeneratorFactory.h"
#include "ParticleAccelerationGenerator.h"

/* ==================== class ConstantColorGenerator begin ==================== */
Rendering::Particles::Generators::ConstantColorGenerator::ConstantColorGenerator(math::Real red /* = REAL_ONE */, math::Real green /* = REAL_ONE */, math::Real blue /* = REAL_ONE */, math::Real alpha /* = REAL_ONE */) :
	ColorGenerator(),
	m_color(red, green, blue, alpha)
{
}

Rendering::Particles::Generators::ConstantColorGenerator::ConstantColorGenerator(const Color& color) :
	ColorGenerator(),
	m_color(color)
{
}

Rendering::Particles::Generators::ConstantColorGenerator::~ConstantColorGenerator()
{
}

void Rendering::Particles::Generators::ConstantColorGenerator::Generate(math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId)
{
	for (size_t i = startId; i < endId; ++i)
	{
		Set(particleContainer, i, m_color);
	}
}
/* ==================== class ConstantColorGenerator end ==================== */

/* ==================== class FromSetColorGenerator begin ==================== */
Rendering::Particles::Generators::FromSetColorGenerator::FromSetColorGenerator(std::initializer_list<Color> colors) :
	ColorGenerator(),
	m_colors(colors),
	m_randomGenerator(math::random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(math::random::generator_ids::SIMPLE))
{
}

Rendering::Particles::Generators::FromSetColorGenerator::~FromSetColorGenerator()
{
}

void Rendering::Particles::Generators::FromSetColorGenerator::Generate(math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId)
{
	for (size_t i = startId; i < endId; ++i)
	{
		const size_t colorIndex = m_randomGenerator.NextInt(0, m_colors.size() - 1);
		Set(particleContainer, i, m_colors[colorIndex]);
	}
}
/* ==================== class FromSetColorGenerator end ==================== */

/* ==================== class RangeColorGenerator begin ==================== */
Rendering::Particles::Generators::RangeColorGenerator::RangeColorGenerator(const Color& minColor, const Color& maxColor) :
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

Rendering::Particles::Generators::RangeColorGenerator::~RangeColorGenerator()
{
}

void Rendering::Particles::Generators::RangeColorGenerator::Generate(math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId)
{
	for (size_t i = startId; i < endId; ++i)
	{
		Set(particleContainer, i, Color{ m_randomGenerator.NextFloat(m_minRed, m_maxRed), m_randomGenerator.NextFloat(m_minGreen, m_maxGreen),
			m_randomGenerator.NextFloat(m_minBlue, m_maxBlue), m_randomGenerator.NextFloat(m_minAlpha, m_maxAlpha) });
	}
}
/* ==================== class RangeColorGenerator end ==================== */