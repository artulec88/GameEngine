#include "StdAfx.h"
#include "MeshData.h"

using namespace Rendering;
using namespace Utility;

MeshData::MeshData(int indexSize) :
	ReferenceCounter()
{
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);
	size = indexSize;
}


MeshData::~MeshData(void)
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
