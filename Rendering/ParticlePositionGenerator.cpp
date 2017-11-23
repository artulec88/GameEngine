#include "stdafx.h"
#include "ParticlePositionGenerator.h"

#include "Math/RandomGeneratorFactory.h"

/* ==================== class ConstantPositionGenerator begin ==================== */
rendering::particles::generators::ConstantPositionGenerator::ConstantPositionGenerator(math::Real xPos, math::Real yPos, math::Real zPos) :
	PositionGenerator(),
	m_position(xPos, yPos, zPos)
{
}

rendering::particles::generators::ConstantPositionGenerator::ConstantPositionGenerator(const math::Vector3D& position) :
	PositionGenerator(),
	m_position(position)
{
}

rendering::particles::generators::ConstantPositionGenerator::~ConstantPositionGenerator()
{
}

void rendering::particles::generators::ConstantPositionGenerator::Generate(math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId)
{
	for (size_t i = startId; i < endId; ++i)
	{
		Set(particleContainer, i, m_position);
	}
}
/* ==================== class ConstantPositionGenerator end ==================== */

/* ==================== class PlanePositionGenerator begin ==================== */
rendering::particles::generators::PlanePositionGenerator::PlanePositionGenerator(math::Real normalX, math::Real normalY, math::Real normalZ, math::Real distanceToOrigin, math::Real radius) :
	PlanePositionGenerator(math::Plane(math::Vector3D(normalX, normalY, normalZ), distanceToOrigin), radius)
{
}

rendering::particles::generators::PlanePositionGenerator::PlanePositionGenerator(const math::Plane& plane, math::Real radius) :
	PositionGenerator(),
	m_plane(plane),
	m_radius(radius),
	m_translationPoint(50.0f, 0.0f, 50.0f),
	m_randomGenerator(math::random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(math::random::generator_ids::SIMPLE))
{
}

rendering::particles::generators::PlanePositionGenerator::~PlanePositionGenerator()
{
}

void rendering::particles::generators::PlanePositionGenerator::Generate(math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId)
{
	// See http://stackoverflow.com/questions/29350965/generate-a-random-point-in-a-specific-plane-in-c
	for (size_t i = startId; i < endId; ++i)
	{
		//math::Vector3D randomPos(m_plane.GenerateRandomPositionWithinRadius(m_radius));
		//CRITICAL_LOG_MATH("Random position on plane: ", randomPos);
		//Set(particleContainer, i, randomPos);
		Set(particleContainer, i, m_plane.GenerateRandomPositionWithinRadius(m_radius, m_translationPoint));
	}
}
/* ==================== class PlanePositionGenerator end ==================== */

/* ==================== class BoxPositionGenerator begin ==================== */
rendering::particles::generators::BoxPositionGenerator::BoxPositionGenerator(const math::Aabb& aabb) :
	BoxPositionGenerator(aabb.GetBottomLeftPos().x, aabb.GetTopRightPos().x,
		aabb.GetBottomLeftPos().y, aabb.GetTopRightPos().y,
		aabb.GetBottomLeftPos().z, aabb.GetTopRightPos().z)
{
}

rendering::particles::generators::BoxPositionGenerator::BoxPositionGenerator(const math::Vector3D& centerPosition, math::Real xOffset, math::Real yOffset, math::Real zOffset) :
	BoxPositionGenerator(centerPosition.x - xOffset, centerPosition.x + xOffset,
		centerPosition.y - yOffset, centerPosition.y + yOffset,
		centerPosition.z - zOffset, centerPosition.z + zOffset)
{
}

rendering::particles::generators::BoxPositionGenerator::BoxPositionGenerator(math::Real minX, math::Real maxX, math::Real minY, math::Real maxY, math::Real minZ, math::Real maxZ) :
	PositionGenerator(),
	m_minX(minX),
	m_maxX(maxX),
	m_minY(minY),
	m_maxY(maxY),
	m_minZ(minZ),
	m_maxZ(maxZ),
	m_randomGenerator(math::random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(math::random::generator_ids::SIMPLE))
{
}

rendering::particles::generators::BoxPositionGenerator::~BoxPositionGenerator()
{
}

void rendering::particles::generators::BoxPositionGenerator::Generate(math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId)
{
	for (size_t i = startId; i < endId; ++i)
	{
		Set(particleContainer, i, math::Vector3D{ m_randomGenerator.NextFloat(m_minX, m_maxX), m_randomGenerator.NextFloat(m_minY, m_maxY), m_randomGenerator.NextFloat(m_minZ, m_maxZ) });
	}
}
/* ==================== class BoxPositionGenerator end ==================== */

/* ==================== class EllipsoidPositionGenerator begin ==================== */
rendering::particles::generators::EllipsoidPositionGenerator::EllipsoidPositionGenerator(const math::Ellipsoid& ellipsoid) :
	EllipsoidPositionGenerator(ellipsoid.GetCenter(), ellipsoid.GetA(), ellipsoid.GetB(), ellipsoid.GetC())
{
}

rendering::particles::generators::EllipsoidPositionGenerator::EllipsoidPositionGenerator(const math::Vector3D& center, math::Real a, math::Real b, math::Real c) :
	PositionGenerator(),
	m_center(center),
	m_a(a),
	m_b(b),
	m_c(c),
	m_randomGenerator(math::random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(math::random::generator_ids::SIMPLE))
{
}

rendering::particles::generators::EllipsoidPositionGenerator::~EllipsoidPositionGenerator()
{
}

void rendering::particles::generators::EllipsoidPositionGenerator::Generate(math::Real deltaTime, ParticlesContainer* particleContainer, size_t startId, size_t endId)
{
	// TODO: Picking random point inside the ellipsoid with uniform probability.
	// See http://stackoverflow.com/questions/5529148/algorithm-calculate-pseudo-random-point-inside-an-ellipse.
	// http://math.stackexchange.com/questions/87230/picking-random-points-in-the-volume-of-sphere-with-uniform-probability.
	for (size_t i = startId; i < endId; ++i)
	{
		const math::Angle randomAngle{ m_randomGenerator.NextFloat(REAL_ZERO, 2.0f * math::PI), math::units::RADIAN };
		const math::Real randomValueSquareRoot{ sqrt(m_randomGenerator.NextFloat(REAL_ZERO, REAL_ONE)) };
		//math::Real x = (randomValueSquareRoot * randomAngle.Cos()) * m_a;
		//math::Real y = (randomValueSquareRoot * randomAngle.Sin()) * m_b;
		Set(particleContainer, i, m_center + math::Vector3D{ m_randomGenerator.NextFloat(-m_a, m_a), m_randomGenerator.NextFloat(-m_b, m_b), m_randomGenerator.NextFloat(-m_c, m_c) });
	}
}
/* ==================== class EllipsoidPositionGenerator end ==================== */