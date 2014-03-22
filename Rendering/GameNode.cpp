#include "StdAfx.h"
#include "GameNode.h"

#include "Vertex.h"

#include "Utility\Log.h"

using namespace Rendering;
using namespace std;

GameNode::GameNode(void)
{
}


GameNode::~GameNode(void)
{
}

void GameNode::Init()
{
	//std::vector<Vertex> vertices;
	//vertices.push_back(Vertex(Math::Vector3D(-1.0, -1.0, 0.0)));
	//vertices.push_back(Vertex(Math::Vector3D(1.0, -1.0, 0.0)));
	//vertices.push_back(Vertex(Math::Vector3D(0.0, 1.0, 0.0)));

	//glGenBuffers(1, &vertexBuffer);
	//glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	//GLsizeiptr size = 3 * sizeof(vertices);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(struct Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	//static const unsigned short indices[] = { 0, 1, 2 };

	//glGenBuffers(1, &elementBuffer);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);


/* ================================================================================================================= */
	// triangle
	const GLfloat g_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};
	// pyramid
	//const GLfloat g_vertex_buffer_data[] = {
	//	-1.0f, -1.0f, 0.0f,
	//	1.0f, -1.0f, 0.0f,
	//	0.0f, 1.0f, 0.0f,
	//	0.0f, -1.0f, 1.0f
	//};

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	const unsigned short indices[] = { 0, 1, 2 };

	glGenBuffers(1, &elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) * sizeof(unsigned short), &indices[0] /*indices*/, GL_STATIC_DRAW);
}

void GameNode::Update()
{
	//for (list<GameNode&>::iterator gameNodeIt = childrenGameNodes.begin(); gameNodeIt != childrenGameNodes.end(); ++gameNodeIt)
	//{
	//	(*gameNodeIt).Update();
	//}
}

void GameNode::Render()
{
	//glEnableVertexAttribArray(0);
	////glEnableVertexAttribArray(1);
	////glEnableVertexAttribArray(2);

	//glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0 /* sizeof(struct Vertex) */, 0 /*reinterpret_cast<void*>(offsetof(Vertex, pos))*/);
	////glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)sizeof(Math::Vector3D));
	////glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(Math::Vector3D) + sizeof(Math::Vector2D)));

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	////int size;
	////glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0);

	//glDisableVertexAttribArray(0);
	////glDisableVertexAttribArray(1);
	////glDisableVertexAttribArray(2);
/* ================================================================================================================= */
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glDrawElements(GL_TRIANGLES, 3 /*indices.size()*/, GL_UNSIGNED_SHORT, (void*)0);

	glDisableVertexAttribArray(0);


	//for (list<GameNode&>::iterator gameNodeIt = childrenGameNodes.begin(); gameNodeIt != childrenGameNodes.end(); ++gameNodeIt)
	//{
	//	(*gameNodeIt).Render();
	//}
}