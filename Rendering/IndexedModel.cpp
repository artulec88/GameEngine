#include "StdAfx.h"
#include "IndexedModel.h"

#include "Utility\Log.h"

using namespace Rendering;

void IndexedModel::CalcNormals()
{
    for(unsigned int i = 0; i < indices.size(); i += 3 /* number of vertices in one triangle face */)
    {
        unsigned short i0 = indices[i];
        unsigned short i1 = indices[i + 1];
        unsigned short i2 = indices[i + 2];

        Math::Vector3D edge1 = positions[i1] - positions[i0];
        Math::Vector3D edge2 = positions[i2] - positions[i0];
        
        Math::Vector3D normal = edge1.Cross(edge2).Normalized();// glm::normalize(glm::cross(edge1, edge2));
            
        normals[i0] += normal;
        normals[i1] += normal;
        normals[i2] += normal;
    }
    
    for(unsigned int i = 0; i < normals.size(); ++i)
        normals[i] = normals[i].Normalized(); //glm::normalize(normals[i]);
}

void IndexedModel::CalcTangents()
{
    for(unsigned int i = 0; i < indices.size(); i += 3 /* number of vertices in one triangle face */)
    {
        unsigned short i0 = indices[i];
        unsigned short i1 = indices[i + 1];
        unsigned short i2 = indices[i + 2];

        Math::Vector3D edge1 = positions[i1] - positions[i0];
        Math::Vector3D edge2 = positions[i2] - positions[i0];

		Math::Real deltaU1 = texCoords[i1].GetX() - texCoords[i0].GetX();
		Math::Real deltaV1 = texCoords[i1].GetY() - texCoords[i0].GetY();
		Math::Real deltaU2 = texCoords[i2].GetX() - texCoords[i0].GetX();
		Math::Real deltaV2 = texCoords[i2].GetY() - texCoords[i0].GetY();

		Math::Real factor = 1.0f / (deltaU1 * deltaV2 - deltaU2 * deltaV1);
		
		Math::Real x = factor * (deltaV2 * edge1.GetX() - deltaV1 * edge2.GetX());
		Math::Real y = factor * (deltaV2 * edge1.GetY() - deltaV1 * edge2.GetY());
		Math::Real z = factor * (deltaV2 * edge1.GetZ() - deltaV1 * edge2.GetZ());
		Math::Vector3D tangent(x, y, z);

		tangents[i0] += tangent;
		tangents[i1] += tangent;
		tangents[i2] += tangent;
    }
    
    for(unsigned int i = 0; i < tangents.size(); ++i)
        tangents[i] = tangents[i].Normalized(); //glm::normalize(tangents[i]);
}