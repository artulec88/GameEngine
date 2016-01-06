#include "stdafx.h"
#include "IntersectInfo.h"


Math::IntersectInfo::IntersectInfo(bool isIntersecting, Math::Real distance) :
	m_isIntersecting(isIntersecting),
	m_distance(distance)
{
}


Math::IntersectInfo::~IntersectInfo()
{
}
