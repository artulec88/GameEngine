#include "stdafx.h"
#include "IntersectInfo.h"


Math::IntersectInfo::IntersectInfo(Real distance) :
	m_isIntersecting(distance < REAL_ZERO),
	m_distance(distance)
{
}


Math::IntersectInfo::~IntersectInfo()
{
}
