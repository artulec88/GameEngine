#include "stdafx.h"
#include "IntersectInfo.h"


math::IntersectInfo::IntersectInfo(Real distance) :
	m_isIntersecting(distance < REAL_ZERO),
	m_distance(distance)
{
}


math::IntersectInfo::~IntersectInfo()
{
}
