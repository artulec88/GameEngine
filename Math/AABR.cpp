#include "stdafx.h"
#include "AABR.h"
#include "IntersectInfo.h"

Math::AABR::AABR(const Math::Vector2D& bottomLeftPos, const Vector2D& topRightPos) :
	m_bottomLeftPos(bottomLeftPos),
	m_topRightPos(topRightPos)
{
}

Math::AABR::~AABR()
{
}

Math::IntersectInfo Math::AABR::DoesContainPoint(const Math::Vector2D& point) const
{
	return DoesContainPoint(point.GetX(), point.GetY());
}

Math::IntersectInfo Math::AABR::DoesContainPoint(Math::Real x, Math::Real y) const
{
	if ((x < m_bottomLeftPos.GetX()) || (x > m_topRightPos.GetX()) ||
		(y < m_topRightPos.GetY()) || (y > m_bottomLeftPos.GetY()))
	{
		return Math::IntersectInfo(false, 0 /* TODO: Calculate distance to the nearest face of the AABR */);
	}
	return Math::IntersectInfo(true, 0 /* TODO: Calculate distance to the nearest face of the AABR */);
}

Math::IntersectInfo Math::AABR::DoesIntersectAABR(const Math::AABR& aabr) const
{
	// TODO: Remove intermediate local variables.
	Math::Vector2D distances1 = aabr.GetBottomLeftPos() - m_topRightPos;
	Math::Vector2D distances2 = m_bottomLeftPos - aabr.GetTopRightPos();
	Math::Vector2D distances = distances1.Max(distances2);

	Math::Real maxDistance = distances.Max();

	return Math::IntersectInfo(maxDistance < 0, maxDistance);
}