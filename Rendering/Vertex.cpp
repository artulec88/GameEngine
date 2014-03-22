#include "stdafx.h"
#include "Vertex.h"

#include <sstream>

using namespace Rendering;
using namespace Math;

Vertex::Vertex(Vector3D& position) :
	pos(position)//,
	//texCoord(Vector2D(0, 0)),
	//normal(Vector3D(0, 0, 0))
{
}

//Vertex::Vertex(Vector3D& position, Vector2D& texCoord, Vector3D& normal) :
//	pos(position),
//	texCoord(texCoord),
//	normal(normal)
//{
//}

std::string Vertex::ToString() const
{
	std::stringstream ss("");

	ss << "{ pos = ";
	ss << pos.ToString();
	//ss << "; texCoord = ";
	//ss << texCoord.ToString();
	//ss << "; normal = ";
	//ss << normal.ToString();
	ss << " } ";

	return ss.str();
}