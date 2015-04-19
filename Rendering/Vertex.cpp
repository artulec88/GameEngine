#include "stdafx.h"
#include "Vertex.h"
#include "Math\FloatingPoint.h"

#include <sstream>

using namespace Rendering;
using namespace Math;

//Vertex::Vertex(const Vector3D& position, const Vector2D& texCoord /* = Vector2D(0.0, 0.0) */, const Vector3D& normal /* = Vector3D(0.0, 0.0, 0.0) */,
//	const Math::Vector3D& tangent /* = Math::Vector3D(0, 0, 0) */, const Math::Vector3D& bitangent /* = Math::Vector3D(0, 0, 0) */) :
Vertex::Vertex(const Vector3D& position, const Vector2D& texCoord /* = Vector2D(0.0, 0.0) */, const Vector3D& normal /* = Vector3D(0.0, 0.0, 0.0) */,
	const Math::Vector3D& tangent /* = Math::Vector3D(0, 0, 0) */) :
	m_pos(position),
	m_texCoord(texCoord),
	m_normal(normal),
	m_tangent(tangent)//,
	//m_bitangent(bitangent)
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
	return ((m_pos == vertex.m_pos) && (m_texCoord == vertex.m_texCoord) && (m_normal == vertex.m_normal));
}

bool Vertex::operator!=(const Vertex& vertex) const
{
	return (!((*this) == vertex));
}

std::string Vertex::ToString() const
{
	std::stringstream ss("");

	ss << "{ pos = ";
	ss << m_pos.ToString();
	ss << "; texCoord = ";
	ss << m_texCoord.ToString();
	//ss << "; normal = ";
	//ss << m_normal.ToString();
	ss << " } ";

	return ss.str();
}