#include "stdafx.h"
#include "KDTree.h"

using namespace Math;

KDTree::KDTree(Math::Vector3D* positions, int positionsCount) :
	treeNode(NULL)
{
}

KDTree::~KDTree(void)
{
	//SAFE_DELETE(treeNode);
	if (treeNode != NULL)
	{
		delete treeNode;
		treeNode = NULL;
	}
}

void KDTree::BuildTree(Math::Vector3D* positions, int positionsCount)
{
	treeNode = new KDTreeNode(Math::Vector2D(positions[0].GetX(), positions[0].GetZ()), positions[0].GetY());
}