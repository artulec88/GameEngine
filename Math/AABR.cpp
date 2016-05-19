#include "stdafx.h"
#include "AABR.h"
#include "IntersectInfo.h"
#include "Utility\ILogger.h"

Math::AABR::AABR(const Math::Vector2D& bottomLeftPos, const Vector2D& topRightPos) :
	m_bottomLeftPos(bottomLeftPos),
	m_topRightPos(topRightPos)
{
	CHECK_CONDITION_MATH(!(m_bottomLeftPos.GetX() > m_topRightPos.GetX()), Utility::Error,
		"The AABR cannot be properly created. The X component of bottom left vertex (%.3f) cannot be greater than X component of the top right vertex (%.3f)",
		m_bottomLeftPos.GetX(), m_topRightPos.GetX());
	CHECK_CONDITION_MATH(!(m_bottomLeftPos.GetY() < m_topRightPos.GetY()), Utility::Error,
		"The AABR cannot be properly created. The Y component of bottom left vertex (%.3f) cannot be less than Y component of the top right vertex (%.3f)",
		m_bottomLeftPos.GetY(), m_topRightPos.GetY());
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
	Math::Real distanceX = CalcDistanceToNearestFaceX(x);
	Math::Real distanceY = CalcDistanceToNearestFaceY(y);
	//DELOCUST_LOG_MATH("DistanceX = %.3f, distanceY = %.3f", distanceX, distanceY);
	bool isIntersecting = !((distanceX > 0) || (distanceY > 0));
	return Math::IntersectInfo(isIntersecting, (distanceX > distanceY) ? distanceX : distanceY);
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

Math::Real Math::AABR::CalcDistanceToNearestFaceX(Math::Real x) const
{
	Math::Real distanceX;
	if (x < m_bottomLeftPos.GetX())
	{
		// positive value, because X is outside the AABR
		distanceX = m_bottomLeftPos.GetX() - x;
	}
	else if (x > m_topRightPos.GetX())
	{
		// positive value, because X is outside the AABR
		distanceX = x - m_topRightPos.GetX();
	}
	else
	{
		distanceX = x - m_bottomLeftPos.GetX();
		if ((m_topRightPos.GetX() - x) < distanceX)
		{
			distanceX = m_topRightPos.GetX() - x;
		}
		distanceX = -distanceX;
		// negative value, because X is inside the AABR
	}
	return distanceX;
}

Math::Real Math::AABR::CalcDistanceToNearestFaceY(Math::Real y) const
{
	Math::Real distanceY;
	if (y < m_topRightPos.GetY())
	{
		// positive value, because Y is outside the AABR
		distanceY = m_topRightPos.GetY() - y;
	}
	else if (y > m_bottomLeftPos.GetY())
	{
		// positive value, because Y is outside the AABR
		distanceY = y - m_bottomLeftPos.GetY();
	}
	else
	{
		distanceY = y - m_topRightPos.GetY();
		if ((m_bottomLeftPos.GetY() - y) < distanceY)
		{
			distanceY = m_bottomLeftPos.GetY() - y;
		}
		distanceY = -distanceY;
		// negative value, because Y is inside the AABR
	}
	return distanceY;
}