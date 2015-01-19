#include "stdafx.h"
#include "KDTreeNode.h"

using namespace Math;

KDTreeNode::KDTreeNode(const Math::Vector2D& position, float value) :
	position(position),
	value(value)
{
}

KDTreeNode::~KDTreeNode(void)
{
}
