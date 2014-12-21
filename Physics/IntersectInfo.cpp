#include "StdAfx.h"
#include "IntersectInfo.h"

using namespace Physics;

IntersectInfo::IntersectInfo(const bool isIntersecting, const Math::Real distance) :
	m_isIntersecting(isIntersecting),
	m_distance(distance)
{
}


IntersectInfo::~IntersectInfo(void)
{
}
