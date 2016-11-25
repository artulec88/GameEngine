#include "stdafx.h"
#include "AABB.h"
#include "IntersectInfo.h"

Math::AABB::AABB(const Vector3D& bottomLeftPos, const Vector3D& topRightPos) noexcept:
	m_bottomLeftPos(bottomLeftPos),
	m_topRightPos(topRightPos)
{
}

//Math::AABB::AABB(const AABB& aabb) :
//	m_bottomLeftPos(aabb.m_bottomLeftPos),
//	m_topRightPos(aabb.m_topRightPos)
//{
//}
//
//Math::AABB::AABB(AABB&& aabb) :
//	m_bottomLeftPos(std::move(aabb.m_bottomLeftPos)),
//	m_topRightPos(std::move(aabb.m_topRightPos))
//{
//}
//
//Math::AABB& Math::AABB::operator=(const AABB& aabb)
//{
//	m_bottomLeftPos = aabb.m_bottomLeftPos;
//	m_topRightPos = aabb.m_topRightPos;
//	return *this;
//}
//
//Math::AABB& Math::AABB::operator=(AABB&& aabb)
//{
//	m_bottomLeftPos = std::move(aabb.m_bottomLeftPos);
//	m_topRightPos = std::move(aabb.m_topRightPos);
//	// TODO: Reset the state of the move-from object (is it really neccessary?)
//	return *this;
//}

Math::AABB::~AABB()
{
}

Math::IntersectInfo Math::AABB::DoesIntersectAABB(const AABB& aabb) const noexcept
{
	// TODO: Remove intermediate local variables.
	const Vector3D distances1 = aabb.GetBottomLeftPos() - m_topRightPos;
	const Vector3D distances2 = m_bottomLeftPos - aabb.GetTopRightPos();
	return IntersectInfo(distances1.Max(distances2).Max());
}