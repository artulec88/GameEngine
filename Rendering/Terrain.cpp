#include "stdafx.h"
#include "Terrain.h"


rendering::Terrain::Terrain(const math::Surface& surface, const math::Transform& terrainTransform) :
	//m_terrainMesh(terrainMesh),
	m_x(0),
	m_z(0),
#ifdef HEIGHTS_KD_TREE
	m_kdTree(transformationMatrix.TransformPositions(static_cast<math::Vector3D*>(terrainMesh->GetBufferData(MeshBufferTypes::POSITIONS, nullptr)), terrainMesh->GetBufferSize(MeshBufferTypes::POSITIONS) / sizeof(math::Vector3D)),
		terrainMesh->GetBufferSize(MeshBufferTypes::POSITIONS) / sizeof(math::Vector3D), 2, 0)
#elif defined HEIGHTS_SURFACE
	m_surface(surface)
#endif
{
}


rendering::Terrain::~Terrain()
{
}

math::Real rendering::Terrain::GetHeightAt(math::Real x, math::Real z) const
{
#ifdef HEIGHTS_KD_TREE
#ifdef DEBUG_LOGGING_ENABLED
	math::Real y = m_kdTree.SearchNearestValue(x, z);
	DEBUG_LOG_RENDERING("Height ", y, " returned for position [", x, "; ", z, "].");
	return y;
#else
	return m_kdTree.SearchNearestValue(x, z);
#endif
#elif defined HEIGHTS_SURFACE
#ifdef DEBUG_LOGGING_ENABLED
	const auto y = m_surface.GetHeightAt(x, z);
	DEBUG_LOG_RENDERING("Height ", y, " returned for position [", x, "; ", z, "].");
	return y;
#else
	return m_surface.GetHeightAt(x, z);
#endif
#endif
}