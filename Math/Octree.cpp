#include "stdafx.h"
#include "Octree.h"


Math::SpacePartitioning::Octree::Octree() :
	topNorthWestNode(NULL),
	topNorthEastNode(NULL),
	topSouthWestNode(NULL),
	topSouthEastNode(NULL),
	bottomNorthWestNode(NULL),
	bottomNorthEastNode(NULL),
	bottomSouthWestNode(NULL),
	bottomSouthEastNode(NULL)
{
}


Math::SpacePartitioning::Octree::~Octree()
{
}

bool Math::SpacePartitioning::Octree::HasChildren() const
{
	return ((topNorthWestNode != NULL) || (topNorthEastNode != NULL) || (topSouthWestNode != NULL) || (topSouthEastNode != NULL) ||
		(bottomNorthWestNode != NULL) || (bottomNorthEastNode != NULL) || (bottomSouthWestNode != NULL) || (bottomSouthEastNode != NULL));
}

bool Math::SpacePartitioning::Octree::IsLeaf() const
{
	return !HasChildren();
}