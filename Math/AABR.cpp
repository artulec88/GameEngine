#include "stdafx.h"
#include "AABR.h"
#include "IntersectInfo.h"
#include "Utility\ILogger.h"

Math::AABR::AABR(const Vector2D& bottomLeftPos, const Vector2D& topRightPos) noexcept:
	m_bottomLeftPos(bottomLeftPos),
	m_topRightPos(topRightPos)
{
	CHECK_CONDITION_MATH(!(m_bottomLeftPos.GetX() > m_topRightPos.GetX()), Utility::Logging::ERR,
		"The AABR cannot be properly created. The X component of bottom left vertex (", m_bottomLeftPos.GetX(),
		") cannot be greater than X component of the top right vertex (", m_topRightPos.GetX(), ")");
	CHECK_CONDITION_MATH(!(m_bottomLeftPos.GetY() < m_topRightPos.GetY()), Utility::Logging::ERR,
		"The AABR cannot be properly created. The Y component of bottom left vertex (", m_bottomLeftPos.GetY(),
		") cannot be less than Y component of the top right vertex (", m_topRightPos.GetY(), ")");
}

Math::AABR::~AABR()
{
}

Math::IntersectInfo Math::AABR::DoesContainPoint(Real x, Real y) const noexcept
{
	Real distanceX = CalcDistanceToNearestFaceX(x);
	Real distanceY = CalcDistanceToNearestFaceY(y);
	//DELOCUST_LOG_MATH("DistanceX = ", distanceX, ", distanceY = ", distanceY);
	return IntersectInfo((distanceX < distanceY) ? distanceX : distanceY);
}

Math::IntersectInfo Math::AABR::DoesIntersectAABR(const AABR& aabr) const noexcept
{
	// TODO: Remove intermediate local variables.
	Vector2D distances1 = aabr.GetBottomLeftPos() - m_topRightPos;
	Vector2D distances2 = m_bottomLeftPos - aabr.GetTopRightPos();
	return IntersectInfo(distances1.Max(distances2).Max());
}

Math::Real Math::AABR::CalcDistanceToNearestFaceX(Real x) const noexcept
{
	Real distanceX;
	if (x < m_bottomLeftPos.x)
	{
		// positive value, because X is outside the AABR
		distanceX = m_bottomLeftPos.x - x;
	}
	else if (x > m_topRightPos.x)
	{
		// positive value, because X is outside the AABR
		distanceX = x - m_topRightPos.x;
	}
	else
	{
		distanceX = x - m_bottomLeftPos.x;
		if ((m_topRightPos.x - x) < distanceX)
		{
			distanceX = m_topRightPos.x - x;
		}
		distanceX = -distanceX;
		// negative value, because X is inside the AABR
	}
	return distanceX;
}

Math::Real Math::AABR::CalcDistanceToNearestFaceY(Real y) const noexcept
{
	Real distanceY;
	if (y < m_topRightPos.y)
	{
		// positive value, because Y is outside the AABR
		distanceY = m_topRightPos.y - y;
	}
	else if (y > m_bottomLeftPos.y)
	{
		// positive value, because Y is outside the AABR
		distanceY = y - m_bottomLeftPos.y;
	}
	else
	{
		distanceY = y - m_topRightPos.y;
		if ((m_bottomLeftPos.y - y) < distanceY)
		{
			distanceY = m_bottomLeftPos.y - y;
		}
		distanceY = -distanceY;
		// negative value, because Y is inside the AABR
	}
	return distanceY;
}