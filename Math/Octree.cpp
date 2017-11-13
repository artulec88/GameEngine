#include "stdafx.h"
#include "Octree.h"


Math::SpacePartitioning::Octree::Octree() :
	topNorthWestNode(nullptr),
	topNorthEastNode(nullptr),
	topSouthWestNode(nullptr),
	topSouthEastNode(nullptr),
	bottomNorthWestNode(nullptr),
	bottomNorthEastNode(nullptr),
	bottomSouthWestNode(nullptr),
	bottomSouthEastNode(nullptr)
{
}


Math::SpacePartitioning::Octree::~Octree()
{
}

bool Math::SpacePartitioning::Octree::HasChildren() const
{
	return ((topNorthWestNode != nullptr) || (topNorthEastNode != nullptr) || (topSouthWestNode != nullptr) || (topSouthEastNode != nullptr) ||
		(bottomNorthWestNode != nullptr) || (bottomNorthEastNode != nullptr) || (bottomSouthWestNode != nullptr) || (bottomSouthEastNode != nullptr));
}

bool Math::SpacePartitioning::Octree::IsLeaf() const
{
	return !HasChildren();
}