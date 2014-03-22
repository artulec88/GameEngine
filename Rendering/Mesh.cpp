#include "StdAfx.h"
#include "Mesh.h"
#include "OBJModel.h"
#include "Math\Vector.h"

#include "Utility\Utility.h"
#include "Utility\Log.h"

#include <fstream>

using namespace Rendering;
using namespace Utility;
using namespace std;
//using namespace Math;

// This function is to be removed in the future once nice model loader is created
/* static */ void Mesh::LoadFromFile(const std::string& fileName, Mesh& mesh)
{
	// TODO: Check whether the fileName is a full path or just a fileName. Act accordingly.
	std::string name = fileName;
	const char *tmp = strrchr(fileName.c_str(), '\\');
	if (tmp != NULL)
	{
		name.assign(tmp + 1);
	}
	stdlog(Info, LOGPLACE, "Loading mesh from file \"%s\"", name.c_str());
	std::string extension = name.substr(name.find_last_of(".") + 1);
	stdlog(Delocust, LOGPLACE, "Extension is = \"%s\"", extension.c_str());

	if (extension != "obj")
	{
		stdlog(Error, LOGPLACE, "File format \"%s\" is not supported for mesh data", extension.c_str());
		return;
	}

	vector<Vertex> vertices;
	vector<unsigned short> indices;

	ifstream file(fileName.c_str());
	if (!file.is_open())
	{
		stdlog(Error, LOGPLACE, "Unable to open 3D model file \"%s\". Check the path.", name.c_str());
	}
	string line;

	while (file.good())
	{
		ASSERT(file.is_open());
		getline(file, line);

		vector<string> tokens;
		CutToTokens(line, tokens);
		// TODO: Make sure tokens does not have empty entries
		if ( (tokens.size() == 0) || (tokens[0] == "#") )
		{
			continue;
		}

		if (tokens[0] == "v")
		{
			ASSERT(tokens.size() >= 4);
			Math::Real x = static_cast<Math::Real>(atof(tokens[1].c_str()));
			Math::Real y = static_cast<Math::Real>(atof(tokens[2].c_str()));
			Math::Real z = static_cast<Math::Real>(atof(tokens[3].c_str()));
			vertices.push_back(Vertex(Math::Vector3D(x, y, z)));
		}
		else if (tokens[0] == "f")
		{
			ASSERT(tokens.size() >= 4);
			 // subtracting 1, because OBJ starts from index 1 (not zero)
			unsigned short i1 = static_cast<unsigned short>(atoi(tokens[1].c_str()) - 1);
			unsigned short i2 = static_cast<unsigned short>(atoi(tokens[2].c_str()) - 1);
			unsigned short i3 = static_cast<unsigned short>(atoi(tokens[3].c_str()) - 1);
			indices.push_back(i1);
			indices.push_back(i2);
			indices.push_back(i3);

			if (tokens.size() > 4)
			{
				stdlog(Warning, LOGPLACE, "Quads defined in the OBJ file");
				i1 = static_cast<unsigned short>(atoi(tokens[1].c_str()) - 1);
				i2 = static_cast<unsigned short>(atoi(tokens[3].c_str()) - 1);
				i3 = static_cast<unsigned short>(atoi(tokens[4].c_str()) - 1);
				indices.push_back(i1);
				indices.push_back(i2);
				indices.push_back(i3);
			}
		}
	}

	ASSERT(file.is_open());
	file.close();

	for (int i = 0; i < vertices.size(); ++i)
	{
		cout << i+1 << "): " << vertices[i].ToString() << endl;
	}
	mesh.AddVertices(&vertices[0], vertices.size(), &indices[0], indices.size(), false);

	return;
}

Mesh::Mesh(void) :
	vbo(0),
	ibo(0),
	size(0)
{
}

Mesh::Mesh(const std::string& fileName) :
	vbo(0),
	ibo(0),
	size(0)
{
	// TODO: Loading a model from the given file
	//IndexedModel model = OBJModel(fileName).ToIndexedModel();

	//std::vector<Vertex> vertices;
	//for (unsigned int i = 0; i < model.positions.size(); ++i)
	//{
	//	vertices.push_back(Vertex(model.positions[i], model.texCoords[i]));//, model.normals[i]));
	//}
	//for (int i = 0; i < vertices.size(); ++i)
	//{
	//	cout << i+1 << "): " << vertices[i].ToString() << endl;
	//}
	//AddVertices(&vertices[0], vertices.size(), (unsigned short*)&model.indices[0], model.indices.size(), false);

	LoadFromFile(fileName, *this);
}

Mesh::~Mesh(void)
{
	if (vbo)
	{
		glDeleteBuffers(1, &vbo);
	}
	if (ibo)
	{
		glDeleteBuffers(1, &ibo);
	}
}

void Mesh::AddVertices(Vertex* vertices, int vertSize, unsigned short* indices, int indexSize, bool calcNormalsEnabled /* = true */)
{
	if (!vbo)
	{
		glGenBuffers(1, &vbo);
	}
	if (!ibo)
	{
		glGenBuffers(1, &ibo);
	}
	this->size = indexSize;

	if (calcNormalsEnabled)
	{
		//this->CalcNormals(vertices, vertSize, indices, indexSize);
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertSize * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize * sizeof(unsigned short), indices, GL_STATIC_DRAW);
}

void Mesh::Draw() const
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	//glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)sizeof(Math::Vector3D));
	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(Math::Vector3D) + sizeof(Math::Vector2D)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glDrawElements(GL_TRIANGLES, this->size, GL_UNSIGNED_SHORT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	//glDisableVertexAttribArray(2);
}

//void Mesh::CalcNormals(Vertex* vertices, int vertSize, int* indices, int indexSize)
//{
//	for(int i = 0; i < indexSize; i += 3)
//	{
//		int i0 = indices[i];
//		int i1 = indices[i + 1];
//		int i2 = indices[i + 2];
//		
//		Vector3D v1 = vertices[i1].pos - vertices[i0].pos;
//		Vector3D v2 = vertices[i2].pos - vertices[i0].pos;
//		Vector3D normal = v1.Cross(v2).Normalized();
//		
//		vertices[i0].normal += normal;
//		vertices[i1].normal += normal;
//		vertices[i2].normal += normal;
//	}
//	
//	for(int i = 0; i < vertSize; i++)
//	{
//		vertices[i].normal = vertices[i].normal.Normalized();
//	}
//}

//void Mesh::LoadFromFile(const std::string& fileName)
//{
//
//}