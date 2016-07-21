#include "stdafx.h"
#include "AABB.h"
#include "IntersectInfo.h"

Math::AABB::AABB(const Math::Vector3D& bottomLeftPos, const Vector3D& topRightPos) :
	m_bottomLeftPos(bottomLeftPos),
	m_topRightPos(topRightPos)
{
}

Math::AABB::AABB(const AABB& aabb) : // copy constructor
	m_bottomLeftPos(aabb.GetBottomLeftPos()),
	m_topRightPos(aabb.GetTopRightPos())
{
}

Math::AABB::AABB(AABB&& aabb) : // move constructor
	m_bottomLeftPos(std::move(aabb.m_bottomLeftPos)),
	m_topRightPos(std::move(aabb.m_topRightPos))
{
	// TODO: Reset the state of the move-from object (is it really neccessary?)
}

Math::AABB& Math::AABB::operator=(const AABB& aabb) // copy assignment operator
{
	m_bottomLeftPos = aabb.GetBottomLeftPos();
	m_topRightPos = aabb.GetTopRightPos();
	return *this;
}

Math::AABB& Math::AABB::operator=(AABB&& aabb) // move assignment operator
{
	m_bottomLeftPos = std::move(aabb.GetBottomLeftPos());
	m_topRightPos = std::move(aabb.GetTopRightPos());
	// TODO: Reset the state of the move-from object (is it really neccessary?)
	return *this;
}

Math::AABB::~AABB()
{
}

Math::IntersectInfo Math::AABB::DoesIntersectAABB(const Math::AABB& aabb) const
{
	// TODO: Remove intermediate local variables.
	Math::Vector3D distances1 = aabb.GetBottomLeftPos() - m_topRightPos;
	Math::Vector3D distances2 = m_bottomLeftPos - aabb.GetTopRightPos();
	Math::Vector3D distances = distances1.Max(distances2);

	Math::Real maxDistance = distances.Max();

	return Math::IntersectInfo(maxDistance < 0, maxDistance);
}