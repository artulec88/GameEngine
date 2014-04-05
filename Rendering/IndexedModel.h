#pragma once

#include <vector>
#include <string>

#include "Math\Vector.h"

#include "Utility\Utility.h"

namespace Rendering
{

/**
 * @brief File format-independent, optimized representation of the model
 */
class IndexedModel
{
/* ==================== Non-static member variables begin ==================== */
private:
    std::vector<Math::Vector3D> positions;
    std::vector<Math::Vector2D> texCoords;
    std::vector<Math::Vector3D> normals;
    std::vector<unsigned short> indices;
/* ==================== Non-static member variables end ==================== */


/* ==================== Non-static member functions begin ==================== */
public:
	unsigned int PositionsSize() const { return positions.size(); }
	unsigned int TexCoordsSize() const { return texCoords.size(); }
	unsigned int NormalsSize() const { return normals.size(); }
	unsigned int IndicesSize() const { return indices.size(); }
	
	void AddPosition(const Math::Vector3D& pos) { positions.push_back(pos); }
	void AddTexCoord(const Math::Vector2D& texCoord) { texCoords.push_back(texCoord); }
	void AddNormal(const Math::Vector3D& normal) { normals.push_back(normal); }
	void AddIndex(unsigned short index) { indices.push_back(index); }
	
	Math::Vector3D GetPosition(int i) const { return positions[i]; }
	Math::Vector2D GetTexCoord(int i) const { return texCoords[i]; }
	Math::Vector3D GetNormal(int i) const { return normals[i]; }
	unsigned short GetIndex(int i) const { return indices[i]; }
	const unsigned short* GetIndices() const { return &indices[0]; }
	
	void SetPosition(int i, const Math::Vector3D& pos)
	{
		ASSERT(i >= 0);
		ASSERT(i < static_cast<int>(positions.size()));
		positions[i] = pos;
	}
	void SetTexCoord(int i, const Math::Vector2D& texCoord)
	{
		ASSERT(i >= 0);
		ASSERT(i < static_cast<int>(texCoords.size()));
		texCoords[i] = texCoord;
	}
	void SetNormal(int i, const Math::Vector3D& normal)
	{
		ASSERT(i >= 0);
		ASSERT(i < static_cast<int>(normals.size()));
		normals[i] = normal;
	}

	void CalcNormals();
/* ==================== Non-static member functions end ==================== */
}; /* end class IndexedModel */

} /* end namespace Rendering */