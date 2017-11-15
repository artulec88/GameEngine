#include "stdafx.h"
#include "Octree.h"


math::space_partitioning::Octree::Octree() :
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


math::space_partitioning::Octree::~Octree()
{
}

bool math::space_partitioning::Octree::HasChildren() const
{
	return ((topNorthWestNode != nullptr) || (topNorthEastNode != nullptr) || (topSouthWestNode != nullptr) || (topSouthEastNode != nullptr) ||
		(bottomNorthWestNode != nullptr) || (bottomNorthEastNode != nullptr) || (bottomSouthWestNode != nullptr) || (bottomSouthEastNode != nullptr));
}

bool math::space_partitioning::Octree::IsLeaf() const
{
	return !HasChildren();
}