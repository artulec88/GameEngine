#pragma once

#include <vector>
#include <string>

#include "Math\Vector.h"

namespace Rendering
{

/**
 * @brief File format-independent, optimized representation of the model
 */
class IndexedModel
{
/* ==================== Non-static member variables begin ==================== */
public:
    std::vector<Math::Vector3D> positions;
    std::vector<Math::Vector2D> texCoords;
    std::vector<Math::Vector3D> normals;
    std::vector<unsigned short> indices;
/* ==================== Non-static member variables end ==================== */


/* ==================== Non-static member functions begin ==================== */
public:
	void CalcNormals();
/* ==================== Non-static member functions end ==================== */
}; /* end class IndexedModel */

} /* end namespace Rendering */