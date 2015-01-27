#include "stdafx.h"
#include "KDTree.h"
#include "ISort.h"
#include "SortingParameters.h"
#include <sstream>

using namespace Math;

KDTree::KDTree(Math::Vector3D* positions, int positionsCount) :
	m_leftTree(NULL),
	m_rightTree(NULL)
{
	BuildTree(positions, positionsCount, 0);
}

KDTree::KDTree(Math::Vector3D* positions, int positionsCount, int depth) :
	m_leftTree(NULL),
	m_rightTree(NULL)
{
	BuildTree(positions, positionsCount, depth);
}

KDTree::~KDTree(void)
{
	SAFE_DELETE(m_leftTree);
	SAFE_DELETE(m_rightTree);
}

void KDTree::BuildTree(Math::Vector3D* positions, int positionsCount, int depth)
{
	SortingKey sortingKey;
	switch (depth % 2)
	{
	case 0:
		sortingKey = COMPONENT_X;
		break;
	case 1:
		sortingKey = COMPONENT_Z;
		break;
	default: // shouldn't occur at all
		LOG(Utility::Warning, LOGPLACE, "Cannot determine the sorting key for the process of constructing the k-d tree");
		sortingKey = COMPONENT_X;
		break;
	}
	LOG(Utility::Notice, LOGPLACE, "Before sorting: depth = %d", depth);
	for (int i = 0; i < positionsCount; ++i)
	{
		LOG(Utility::Info, LOGPLACE, "depth = %d) positions[%d] = %s", depth, i, positions[i].ToString().c_str());
	}
	SortingParametersChain sortingParameters(sortingKey, ASCENDING);
	ISort::GetSortingObject(ISort::MERGE_SORT)->Sort(positions, positionsCount, sortingParameters);
	LOG(Utility::Notice, LOGPLACE, "After sorting: depth = %d", depth);
	for (int i = 0; i < positionsCount; ++i)
	{
		LOG(Utility::Info, LOGPLACE, "depth = %d) positions[%d] = %s", depth, i, positions[i].ToString().c_str());
	}

	int medianIndex = positionsCount / 2;
	m_position = Vector2D(positions[medianIndex].GetXZ());
	m_value = positions[medianIndex].GetY();

	if (medianIndex > 0)
	{
		m_leftTree = new KDTree(positions, medianIndex, depth + 1);
	}
	if (positionsCount - medianIndex - 1 > 0)
	{
		m_rightTree = new KDTree(&positions[medianIndex + 1], positionsCount - medianIndex - 1, depth + 1);
	}
}

std::string KDTree::ToString() const
{
	return ToString(0);
}

std::string KDTree::ToString(int depth) const
{
	std::stringstream s("");
	for (int i = 0; i < depth; ++i)
	{
		s << "  "; // TODO: create a const static field representing the indentation string
	}
	s << "Parent: " << m_position.ToString() << " with value " << m_value << " has following children:" << std::endl;
	for (int i = 0; i < depth; ++i)
	{
		s << "  "; // TODO: create a const static field representing the indentation string
	}
	if (m_leftTree == NULL)
	{
		//s << "\t1. NULL" << std::endl;
		s << "\t1. NULL" << std::endl;
	}
	else
	{
		//s << "\t1. " << m_leftTree->GetPosition().ToString() << " with value " << m_leftTree->GetValue() << std::endl;
		s << m_leftTree->ToString(depth + 1) << std::endl;
	}
	for (int i = 0; i < depth; ++i)
	{
		s << "  "; // TODO: create a const static field representing the indentation string
	}
	if (m_rightTree == NULL)
	{
		//s << "\t2. NULL" << std::endl;
		s << "2. NULL" << std::endl;
	}
	else
	{
		s << m_rightTree->ToString(depth + 1) << std::endl;
		//s << "\t2. " << m_rightTree->GetPosition().ToString() << " with value " << m_rightTree->GetValue() << std::endl;
	}

	//if (m_leftTree != NULL)
	//{
	//	s << m_leftTree->ToString();
	//}
	//if (m_rightTree != NULL)
	//{
	//	s << m_rightTree->ToString();
	//}

	return s.str();	
}
