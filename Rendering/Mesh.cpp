#include "StdAfx.h"
#include "Mesh.h"
#include "OBJModel.h"
#include "Math\Vector.h"

#include "Utility\Utility.h"
#include "Utility\Log.h"

#ifdef MEASURE_TIME_ENABLED
#include <time.h>
#endif
#include <fstream>

using namespace Rendering;
using namespace Utility;
using namespace std;
using namespace Math;

/* static */ std::map<std::string, MeshData*> Mesh::meshResourceMap;

Mesh::Mesh(Vertex* vertices, int vertSize, unsigned short* indices, int indexSize, bool calcNormalsEnabled /* = true */) :
	fileName(),
	meshData(NULL)
{
	AddVertices(vertices, vertSize, indices, indexSize, calcNormalsEnabled);
}

Mesh::Mesh(const std::string& fileName) :
	fileName(fileName),
	meshData(NULL)
{
	std::string name = fileName;
	const char *tmp = strrchr(name.c_str(), '\\');
	if (tmp != NULL)
	{
		name.assign(tmp + 1);
	}
	//std::string extension = name.substr(name.find_last_of(".") + 1);
	//stdlog(Utility::Delocust, LOGPLACE, "Extension is = \"%s\"", extension.c_str());

	std::map<std::string, MeshData*>::const_iterator itr = meshResourceMap.find(fileName);
	if (itr == meshResourceMap.end()) // the mesh has not been loaded yet
	{
#ifdef MEASURE_TIME_ENABLED
		clock_t begin = clock();
#endif

		stdlog(Info, LOGPLACE, "Loading model from file \"%s\"", name.c_str());
		IndexedModel model = OBJModel(fileName).ToIndexedModel();

		ASSERT(model.PositionsSize() == model.TexCoordsSize());
		ASSERT(model.PositionsSize() == model.NormalsSize());
		if (model.PositionsSize() != model.TexCoordsSize())
		{
			stdlog(Error, LOGPLACE, "Created model does not have an identical number of positions and texture coordinates (%d and %d respectively)",
				model.PositionsSize(), model.TexCoordsSize());
		}
		if (model.PositionsSize() != model.NormalsSize())
		{
			stdlog(Error, LOGPLACE, "Created model does not have an identical number of positions and normals (%d and %d respectively)",
				model.PositionsSize(), model.NormalsSize());
		}

		std::vector<Vertex> vertices;
		for (unsigned int i = 0; i < model.PositionsSize(); ++i)
		{
			vertices.push_back(Vertex(model.GetPosition(i), model.GetTexCoord(i), model.GetNormal(i)));
		}
		AddVertices(&vertices[0], vertices.size(), model.GetIndices(), model.IndicesSize(), false);

		meshResourceMap.insert(std::pair<std::string, MeshData*>(fileName, meshData));

#ifdef MEASURE_TIME_ENABLED
		clock_t end = clock();
		stdlog(Debug, LOGPLACE, "Loading a model took %.2f [ms]", 1000.0 * static_cast<double>(end - begin) / (CLOCKS_PER_SEC));
#endif
	}
	else // (itr != meshResourceMap.end()) // the mesh has already been loaded
	{
		stdlog(Info, LOGPLACE, "Model \"%s\" is already loaded. Using already loaded mesh data.", name.c_str());
		meshData = itr->second;
		meshData->AddReference();
	}

	//LoadFromFile(fileName);
}

Mesh::~Mesh(void)
{
	ASSERT(meshData != NULL);
	if (meshData == NULL)
	{
		stdlog(Utility::Warning, LOGPLACE, "Mesh data is already NULL");
		return;
	}
	meshData->RemoveReference();
	if (! meshData->IsReferenced())
	{
		if (fileName.length() > 0)
		{
			meshResourceMap.erase(fileName);
		}

		delete meshData;
		meshData = NULL;
	}
}

void Mesh::AddVertices(Vertex* vertices, int vertSize, const unsigned short* indices, int indexSize, bool calcNormalsEnabled /* = true */)
{
	meshData = new MeshData(indexSize);

	if (calcNormalsEnabled)
	{
		this->CalcNormals(vertices, vertSize, indices, indexSize);
	}

	ASSERT(meshData != NULL);
	if (meshData == NULL)
	{
		stdlog(Critical, LOGPLACE, "Mesh data instance is NULL");
		exit(EXIT_FAILURE);
	}
	glBindBuffer(GL_ARRAY_BUFFER, meshData->GetVBO());
	glBufferData(GL_ARRAY_BUFFER, vertSize * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshData->GetIBO());
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize * sizeof(unsigned short), indices, GL_STATIC_DRAW);
}

void Mesh::Draw() const
{
	ASSERT(meshData != NULL);
	if (meshData == NULL)
	{
		stdlog(Critical, LOGPLACE, "Mesh data instance is NULL");
		exit(EXIT_FAILURE);
	}

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, meshData->GetVBO());
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)sizeof(Vector3D));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(Vector3D) + sizeof(Vector2D)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshData->GetIBO());
	glDrawElements(GL_TRIANGLES, meshData->GetSize(), GL_UNSIGNED_SHORT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void Mesh::CalcNormals(Vertex* vertices, int vertSize, const unsigned short* indices, int indexSize)
{
	const int iterationStep = 3; // we are iterating through faces which are triangles (each triangle has 3 vertices)
	for(int i = 0; i < indexSize; i += iterationStep)
	{
		int i0 = indices[i];
		int i1 = indices[i + 1];
		int i2 = indices[i + 2];
		
		Vector3D v1 = vertices[i1].pos - vertices[i0].pos;
		Vector3D v2 = vertices[i2].pos - vertices[i0].pos;
		Vector3D normalVec = v1.Cross(v2).Normalized();
		
		vertices[i0].normal += normalVec;
		vertices[i1].normal += normalVec;
		vertices[i2].normal += normalVec;
	}
	
	for(int i = 0; i < vertSize; i++)
	{
		vertices[i].normal = vertices[i].normal.Normalized();
	}
}