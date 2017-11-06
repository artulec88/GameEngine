#include "stdafx.h"
#include "Terrain.h"


Rendering::Terrain::Terrain(Math::Real* heights, int heightsCount, const Math::Transform& terrainTransform) :
	//m_terrainMesh(terrainMesh),
	m_x(0),
	m_z(0),
#ifdef HEIGHTS_KD_TREE
	m_kdTree(transformationMatrix.TransformPositions(static_cast<Math::Vector3D*>(terrainMesh->GetBufferData(MeshBufferTypes::POSITIONS, nullptr)), terrainMesh->GetBufferSize(MeshBufferTypes::POSITIONS) / sizeof(Math::Vector3D)),
		terrainMesh->GetBufferSize(MeshBufferTypes::POSITIONS) / sizeof(Math::Vector3D), 2, 0)
#elif defined HEIGHTS_SURFACE
	// TODO: Pass data of the mesh instead of nullptr.
	m_surface(terrainTransform.GetPos().GetXZ(), 50, 50, heightsCount / 2, heightsCount / 2, heights)
#endif
{
}


Rendering::Terrain::~Terrain()
{
}

Math::Real Rendering::Terrain::GetHeightAt(Math::Real x, Math::Real z) const
{
#ifdef HEIGHTS_KD_TREE
#ifdef DEBUG_LOGGING_ENABLED
	Math::Real y = m_kdTree.SearchNearestValue(x, z);
	DEBUG_LOG_RENDERING("Height ", y, " returned for position [", x, "; ", z, "].");
	return y;
#else
	return m_kdTree.SearchNearestValue(x, z);
#endif
#elif defined HEIGHTS_SURFACE
#ifdef DEBUG_LOGGING_ENABLED
	Math::Real y = m_surface.GetHeightAt(x, z);
	DEBUG_LOG_RENDERING("Height ", y, " returned for position [", x, "; ", z, "].");
	return y;
#else
	return m_surface.GetHeightAt(x, z);
#endif
#endif
}