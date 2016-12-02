#include "stdafx.h"
#include "Terrain.h"


Rendering::Terrain::Terrain(const Mesh* terrainMesh, const Math::Matrix4D& transformationMatrix) :
	//m_terrainMesh(terrainMesh),
	m_x(0),
	m_z(0),
	m_kdTree(transformationMatrix.TransformPositions(static_cast<Math::Vector3D*>(terrainMesh->GetBufferData(MeshBufferTypes::POSITIONS, nullptr)), terrainMesh->GetBufferSize(MeshBufferTypes::POSITIONS) / sizeof(Math::Vector3D)),
		terrainMesh->GetBufferSize(MeshBufferTypes::POSITIONS) / sizeof(Math::Vector3D), 2, 0)
{
}


Rendering::Terrain::~Terrain()
{
}

Math::Real Rendering::Terrain::GetHeightAt(Math::Real x, Math::Real z) const
{
#ifdef DEBUG_LOGGING_ENABLED
	Math::Real y = m_kdTree.SearchNearestValue(x, z);
	DEBUG_LOG_RENDERING("Height ", y, " returned for position [", x, "; ", z, "].");
	return y;
#else
	return m_kdTree.SearchNearestValue(x, z);
#endif
}