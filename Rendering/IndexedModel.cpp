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

        Math::Vector3D v1 = positions[i1] - positions[i0];
        Math::Vector3D v2 = positions[i2] - positions[i0];
        
        Math::Vector3D normal = v1.Cross(v2).Normalized();// glm::normalize(glm::cross(v1, v2));
            
        normals[i0] += normal;
        normals[i1] += normal;
        normals[i2] += normal;
    }
    
    for(unsigned int i = 0; i < normals.size(); i++)
        normals[i] = normals[i].Normalized();//glm::normalize(normals[i]);
}