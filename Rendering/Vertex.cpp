#include "stdafx.h"
#include "Vertex.h"
#include "Math\FloatingPoint.h"

#include <sstream>

using namespace Rendering;
using namespace Math;

Vertex::Vertex(const Vector3D& position, const Vector2D& texCoord /* = Vector2D(0.0, 0.0) */, const Vector3D& normal /* = Vector3D(0.0, 0.0, 0.0) */,
	const Math::Vector3D& tangent /* = Math::Vector3D(0, 0, 0) */) :
	pos(position),
	texCoord(texCoord),
	normal(normal),
	tangent(tangent)
{
}

//Vertex::Vertex(Vector3D& position, Vector2D& texCoord, Vector3D& normal) :
//	pos(position),
//	texCoord(texCoord),
//	normal(normal)
//{
//}

bool Vertex::operator==(const Vertex& vertex) const
{
	return ((this->pos == vertex.pos) && (this->texCoord == vertex.texCoord) && (this->normal == vertex.normal));
}

bool Vertex::operator!=(const Vertex& vertex) const
{
	return (!((*this) == vertex));
}

std::string Vertex::ToString() const
{
	std::stringstream ss("");

	ss << "{ pos = ";
	ss << pos.ToString();
	ss << "; texCoord = ";
	ss << texCoord.ToString();
	//ss << "; normal = ";
	//ss << normal.ToString();
	ss << " } ";

	return ss.str();
}