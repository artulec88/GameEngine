#pragma once
#include "GameComponent.h"
#include "Mesh.h"
#include "Shader.h"
#include "Math\Matrix.h"
#include <string>

namespace Rendering
{

class Material;
class Renderer;

class TextRenderer
{
/* ==================== Constructors and destructors begin ==================== */
public:
	TextRenderer(Texture* fontTexture, Math::Real fontSize = 32.0f);
	virtual ~TextRenderer(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	void DrawString(Math::Real x, Math::Real y, const std::string& str, Renderer* renderer);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	Material* fontMaterial;
	Math::Real fontSize;
	Shader* textShader;

	Math::Matrix4D projection;

	GLuint vaoID;
	GLuint texCoordBuffer;
	GLuint vertexBuffer;
/* ==================== Non-static member variables end ==================== */
}; /* end class TextRenderer */

} /* end namespace Rendering */