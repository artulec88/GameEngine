#include "stdafx.h"
#include "AABB.h"
#include "IntersectInfo.h"

math::AABB::AABB(const Vector3D& bottomLeftPos, const Vector3D& topRightPos) noexcept:
	m_bottomLeftPos(bottomLeftPos),
	m_topRightPos(topRightPos)
{
}

//math::AABB::AABB(const AABB& aabb) :
//	m_bottomLeftPos(aabb.m_bottomLeftPos),
//	m_topRightPos(aabb.m_topRightPos)
//{
//}
//
//math::AABB::AABB(AABB&& aabb) :
//	m_bottomLeftPos(std::move(aabb.m_bottomLeftPos)),
//	m_topRightPos(std::move(aabb.m_topRightPos))
//{
//}
//
//math::AABB& math::AABB::operator=(const AABB& aabb)
//{
//	m_bottomLeftPos = aabb.m_bottomLeftPos;
//	m_topRightPos = aabb.m_topRightPos;
//	return *this;
//}
//
//math::AABB& math::AABB::operator=(AABB&& aabb)
//{
//	m_bottomLeftPos = std::move(aabb.m_bottomLeftPos);
//	m_topRightPos = std::move(aabb.m_topRightPos);
//	// TODO: Reset the state of the move-from object (is it really neccessary?)
//	return *this;
//}

math::AABB::~AABB()
{
}

math::IntersectInfo math::AABB::DoesIntersectAABB(const AABB& aabb) const noexcept
{
	// TODO: Remove intermediate local variables.
	const Vector3D distances1 = aabb.GetBottomLeftPos() - m_topRightPos;
	const Vector3D distances2 = m_bottomLeftPos - aabb.GetTopRightPos();
	return IntersectInfo(distances1.Max(distances2).Max());
}