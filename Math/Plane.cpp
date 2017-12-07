#include "stdafx.h"
#include "Plane.h"
#include "Sphere.h"
#include "Angle.h"
#include "IntersectInfo.h"
#include "RandomGeneratorFactory.h"

#include "Utility/ILogger.h"

math::Plane::Plane(const Vector3D& normal, Real distance) :
	m_normal(normal),
	m_distance(distance)
{
}

math::Plane::~Plane()
{
}

math::Plane math::Plane::Normalized() const
{
	return Plane(m_normal.Normalized(), m_distance / m_normal.Length());
}

math::IntersectInfo math::Plane::DoesIntersectSphere(const Sphere& sphere) const
{
	// how far the sphere center is along the plane normal vector
	auto distanceFromSphereCenter = m_normal.Dot(sphere.GetCenter());
	distanceFromSphereCenter += m_distance; // now it represents the distance between the plane and the sphere
	distanceFromSphereCenter = Absolute(distanceFromSphereCenter);
	return IntersectInfo(distanceFromSphereCenter - sphere.GetRadius());
}

math::Vector3D math::Plane::CalculatePointClosestToOrigin() const
{
	const auto normalVectorLengthSquared = m_normal.LengthSquared();
	return Vector3D(m_normal.x * -m_distance / normalVectorLengthSquared,
		m_normal.y * -m_distance / normalVectorLengthSquared,
		m_normal.z * -m_distance / normalVectorLengthSquared);
}

math::Vector3D math::Plane::GenerateRandomPositionWithinRadius(Real radius, const Vector3D& translationPoint) const
{
	auto w(AlmostEqual(m_normal.x, REAL_ZERO) ?
		m_normal.Cross(Vector3D(REAL_ONE, REAL_ZERO, REAL_ZERO)) :
		m_normal.Cross(Vector3D(REAL_ZERO, REAL_ZERO, REAL_ONE)));
	const auto& randomGenerator = random::RandomGeneratorFactory::GetRandomGeneratorFactory().GetRandomGenerator(random::generator_ids::SIMPLE);
	//ERROR_LOG_MATH("Before rotation: ", w);
	w = w.Rotate(m_normal, Angle(randomGenerator.NextFloat(REAL_ZERO, 360.0f)));
	//ERROR_LOG_MATH("After rotation: ", w);
	if (AlmostEqual(radius, REAL_ZERO))
	{
		w.Normalize();
	}
	else
	{
		w *= randomGenerator.NextFloat(REAL_ZERO, radius) / w.Length();
	}
	if (!AlmostEqual(m_distance, REAL_ZERO))
	{
		w += translationPoint;
	}
	DEBUG_LOG_MATH("Random position on plane: ", w);
	return w;
}