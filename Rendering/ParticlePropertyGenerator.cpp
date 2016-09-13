#include "stdafx.h"
#include "ParticlePropertyGenerator.h"

#include "Math\RandomGeneratorFactory.h"

/* ==================== class ParticlePropertyGenerator begin ==================== */
Rendering::Particles::ParticlePropertyGenerator::ParticlePropertyGenerator()
{
}


Rendering::Particles::ParticlePropertyGenerator::~ParticlePropertyGenerator()
{
}
/* ==================== class ParticlePropertyGenerator end ==================== */

/* ==================== class BoxPositionGenerator begin ==================== */
Rendering::Particles::BoxPositionGenerator::BoxPositionGenerator(const Math::AABB& aabb) :
	BoxPositionGenerator(aabb.GetBottomLeftPos().GetX(), aabb.GetTopRightPos().GetX(),
		aabb.GetBottomLeftPos().GetY(), aabb.GetTopRightPos().GetY(),
		aabb.GetBottomLeftPos().GetZ(), aabb.GetTopRightPos().GetZ())
{
}

Rendering::Particles::BoxPositionGenerator::BoxPositionGenerator(Math::Real minX, Math::Real maxX, Math::Real minY, Math::Real maxY, Math::Real minZ, Math::Real maxZ) :
	PositionGenerator(),
	m_minX(minX),
	m_maxX(maxX),
	m_minY(minY),
	m_maxY(maxY),
	m_minZ(minZ),
	m_maxZ(maxZ),
	m_randomGenerator(Math::Random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(Math::Random::Generators::SIMPLE))
{
}

Rendering::Particles::BoxPositionGenerator::~BoxPositionGenerator()
{
}

void Rendering::Particles::BoxPositionGenerator::Generate(Math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId)
{
	for (size_t i = startId; i < endId; ++i)
	{
		Set(particleContainer, i, Math::Vector3D{ m_randomGenerator.NextFloat(m_minX, m_maxX), m_randomGenerator.NextFloat(m_minY, m_maxY), m_randomGenerator.NextFloat(m_minZ, m_maxZ) });
	}
}
/* ==================== class BoxPositionGenerator end ==================== */

/* ==================== class EllipsoidPositionGenerator begin ==================== */
Rendering::Particles::EllipsoidPositionGenerator::EllipsoidPositionGenerator(const Math::Ellipsoid& ellipsoid) :
	EllipsoidPositionGenerator(ellipsoid.GetCenter(), ellipsoid.GetA(), ellipsoid.GetB(), ellipsoid.GetC())
{
}

Rendering::Particles::EllipsoidPositionGenerator::EllipsoidPositionGenerator(const Math::Vector3D& center, Math::Real a, Math::Real b, Math::Real c) :
	PositionGenerator(),
	m_center(center),
	m_a(a),
	m_b(b),
	m_c(c),
	m_randomGenerator(Math::Random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(Math::Random::Generators::SIMPLE))
{
}

Rendering::Particles::EllipsoidPositionGenerator::~EllipsoidPositionGenerator()
{
}

void Rendering::Particles::EllipsoidPositionGenerator::Generate(Math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId)
{
	// TODO: Picking random point inside the ellipsoid with uniform probability.
	// See http://stackoverflow.com/questions/5529148/algorithm-calculate-pseudo-random-point-inside-an-ellipse.
	// http://math.stackexchange.com/questions/87230/picking-random-points-in-the-volume-of-sphere-with-uniform-probability.
	for (size_t i = startId; i < endId; ++i)
	{
		const Math::Angle randomAngle{ m_randomGenerator.NextFloat(REAL_ZERO, 2.0f * Math::PI), Math::Unit::RADIAN };
		const Math::Real randomValueSquareRoot{ sqrt(m_randomGenerator.NextFloat(REAL_ZERO, REAL_ONE)) };
		Math::Real x = (randomValueSquareRoot * randomAngle.Cos()) * m_a;
		Math::Real y = (randomValueSquareRoot * randomAngle.Sin()) * m_b;
		Set(particleContainer, i, m_center + Math::Vector3D{ m_randomGenerator.NextFloat(-m_a, m_a), m_randomGenerator.NextFloat(-m_b, m_b), m_randomGenerator.NextFloat(-m_c, m_c) });
	}
}
/* ==================== class EllipsoidPositionGenerator end ==================== */

/* ==================== class BasicVelocityGenerator begin ==================== */
Rendering::Particles::BasicVelocityGenerator::BasicVelocityGenerator(Math::Real minSpeedX, Math::Real maxSpeedX, Math::Real minSpeedY, Math::Real maxSpeedY, Math::Real minSpeedZ, Math::Real maxSpeedZ) :
	VelocityGenerator(),
	m_minSpeedX(minSpeedX),
	m_maxSpeedX(maxSpeedX),
	m_minSpeedY(minSpeedY),
	m_maxSpeedY(maxSpeedY),
	m_minSpeedZ(minSpeedZ),
	m_maxSpeedZ(maxSpeedZ),
	m_randomGenerator(Math::Random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(Math::Random::Generators::SIMPLE))
{
}

Rendering::Particles::BasicVelocityGenerator::~BasicVelocityGenerator()
{
}

void Rendering::Particles::BasicVelocityGenerator::Generate(Math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId)
{
	for (size_t i = startId; i < endId; ++i)
	{
		Set(particleContainer, i, Math::Vector3D{ m_randomGenerator.NextFloat(m_minSpeedX, m_maxSpeedX), m_randomGenerator.NextFloat(m_minSpeedY, m_maxSpeedY), m_randomGenerator.NextFloat(m_minSpeedZ, m_maxSpeedZ) });
	}
}
/* ==================== class EllipsoidPositionGenerator end ==================== */