#include "stdafx.h"
#include "ParticlePositionGenerator.h"

#include "Math\RandomGeneratorFactory.h"

/* ==================== class ConstantPositionGenerator begin ==================== */
Rendering::Particles::ConstantPositionGenerator::ConstantPositionGenerator(Math::Real xPos, Math::Real yPos, Math::Real zPos) :
	PositionGenerator(),
	m_position(xPos, yPos, zPos)
{
}

Rendering::Particles::ConstantPositionGenerator::ConstantPositionGenerator(const Math::Vector3D& position) :
	PositionGenerator(),
	m_position(position)
{
}

Rendering::Particles::ConstantPositionGenerator::~ConstantPositionGenerator()
{
}

void Rendering::Particles::ConstantPositionGenerator::Generate(Math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId)
{
	for (size_t i = startId; i < endId; ++i)
	{
		Set(particleContainer, i, m_position);
	}
}
/* ==================== class ConstantPositionGenerator end ==================== */

/* ==================== class PlanePositionGenerator begin ==================== */
Rendering::Particles::PlanePositionGenerator::PlanePositionGenerator(const Math::Plane& plane) :
	PositionGenerator(),
	m_plane(plane),
	m_randomGenerator(Math::Random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(Math::Random::Generators::SIMPLE))
{
}

Rendering::Particles::PlanePositionGenerator::~PlanePositionGenerator()
{
}

void Rendering::Particles::PlanePositionGenerator::Generate(Math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId)
{

	//for (size_t i = startId; i < endId; ++i)
	//{
	//	Set(particleContainer, i, GenerateRandomPosition());
	//}
}
/* ==================== class PlanePositionGenerator end ==================== */

/* ==================== class BoxPositionGenerator begin ==================== */
Rendering::Particles::BoxPositionGenerator::BoxPositionGenerator(const Math::AABB& aabb) :
	BoxPositionGenerator(aabb.GetBottomLeftPos().GetX(), aabb.GetTopRightPos().GetX(),
		aabb.GetBottomLeftPos().GetY(), aabb.GetTopRightPos().GetY(),
		aabb.GetBottomLeftPos().GetZ(), aabb.GetTopRightPos().GetZ())
{
}

Rendering::Particles::BoxPositionGenerator::BoxPositionGenerator(const Math::Vector3D& centerPosition, Math::Real xOffset, Math::Real yOffset, Math::Real zOffset) :
	BoxPositionGenerator(centerPosition.GetX() - xOffset, centerPosition.GetX() + xOffset,
		centerPosition.GetY() - yOffset, centerPosition.GetY() + yOffset,
		centerPosition.GetZ() - zOffset, centerPosition.GetZ() + zOffset)
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