#include "stdafx.h"
#include "Octree.h"


math::space_partitioning::Octree::Octree() :
	m_topNorthWestNode(nullptr),
	m_topNorthEastNode(nullptr),
	m_topSouthWestNode(nullptr),
	m_topSouthEastNode(nullptr),
	m_bottomNorthWestNode(nullptr),
	m_bottomNorthEastNode(nullptr),
	m_bottomSouthWestNode(nullptr),
	m_bottomSouthEastNode(nullptr)
{
}


math::space_partitioning::Octree::~Octree()
{
}

bool math::space_partitioning::Octree::HasChildren() const
{
	return m_topNorthWestNode != nullptr || m_topNorthEastNode != nullptr || m_topSouthWestNode != nullptr || m_topSouthEastNode != nullptr ||
		m_bottomNorthWestNode != nullptr || m_bottomNorthEastNode != nullptr || m_bottomSouthWestNode != nullptr || m_bottomSouthEastNode != nullptr;
}

bool math::space_partitioning::Octree::IsLeaf() const
{
	return !HasChildren();
}