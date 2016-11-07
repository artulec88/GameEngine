#include "stdafx.h"
#include "Plane.h"
#include "Sphere.h"
#include "IntersectInfo.h"
#include "RandomGeneratorFactory.h"

Math::Plane::Plane(const Vector3D& normal, Real distance) :
	m_normal(normal),
	m_distance(distance)
{
}

Math::Plane::~Plane(void)
{
}

Math::Plane Math::Plane::Normalized() const
{
	Math::Real length = m_normal.Length();
	return Plane(m_normal.Normalized(), m_distance / length);
}

Math::IntersectInfo Math::Plane::DoesIntersectSphere(const Sphere& sphere) const
{
	// how far the sphere center is along the plane normal vector
	Math::Real distanceFromSphereCenter = m_normal.Dot(sphere.GetCenter());
	distanceFromSphereCenter += m_distance; // now it represents the distance between the plane and the sphere
	distanceFromSphereCenter = Math::Absolute(distanceFromSphereCenter);

	Math::Real distanceFromSphere = distanceFromSphereCenter - sphere.GetRadius();

	return Math::IntersectInfo(distanceFromSphere < REAL_ZERO, distanceFromSphere);
}

Math::Vector3D Math::Plane::CalculatePointClosestToOrigin() const
{
	Math::Real normalVectorLengthSquared = m_normal.LengthSquared();
	return Math::Vector3D(m_normal.GetX() * -m_distance / normalVectorLengthSquared,
		m_normal.GetY() * -m_distance / normalVectorLengthSquared,
		m_normal.GetZ() * -m_distance / normalVectorLengthSquared);
}

Math::Vector3D Math::Plane::GenerateRandomPositionWithinRadius(Math::Real radius, const Math::Vector3D& translationPoint) const
{
	Math::Vector3D w(Math::AlmostEqual(m_normal.GetX(), REAL_ZERO) ?
		m_normal.Cross(Math::Vector3D(REAL_ONE, REAL_ZERO, REAL_ZERO)) :
		m_normal.Cross(Math::Vector3D(REAL_ZERO, REAL_ZERO, REAL_ONE)));
	const Math::Random::RandomGenerator& randomGenerator = Math::Random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(Math::Random::Generators::SIMPLE);
	//ERROR_LOG_MATH("Before rotation: ", w);
	w = w.Rotate(m_normal, Math::Angle(randomGenerator.NextFloat(REAL_ZERO, 360.0f)));
	//ERROR_LOG_MATH("After rotation: ", w);
	if (Math::AlmostEqual(radius, REAL_ZERO))
	{
		w.Normalize();
	}
	else
	{
		w *= (randomGenerator.NextFloat(REAL_ZERO, radius) / w.Length());
	}
	if (!Math::AlmostEqual(m_distance, REAL_ZERO))
	{
		w += translationPoint;
	}
	DEBUG_LOG_MATH("Random position on plane: ", w);
	return w;
}