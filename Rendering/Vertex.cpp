#include "stdafx.h"
#include "Vertex.h"

#ifdef _DEBUG
#include <sstream>
#endif

using namespace Rendering;
using namespace Math;

//Vertex::Vertex(const Vector3D& position, const Vector2D& texCoord /* = Vector2D(REAL_ZERO, REAL_ZERO) */, const Vector3D& normal /* = Vector3D(REAL_ZERO, REAL_ZERO, REAL_ZERO) */,
//	const Math::Vector3D& tangent /* = Math::Vector3D(REAL_ZERO, REAL_ZERO, REAL_ZERO) */, const Math::Vector3D& bitangent /* = Math::Vector3D(REAL_ZERO, REAL_ZERO, REAL_ZERO) */) :
Vertex::Vertex(const Vector3D& position, const Vector2D& texCoord /* = Vector2D(REAL_ZERO, REAL_ZERO) */, const Vector3D& normal /* = Vector3D(REAL_ZERO, REAL_ZERO, REAL_ZERO) */,
	const Math::Vector3D& tangent /* = Math::Vector3D(REAL_ZERO, REAL_ZERO, REAL_ZERO) */) :
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

#ifdef _DEBUG
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
#endif