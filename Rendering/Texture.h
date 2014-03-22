#pragma once

#include "Rendering.h"
#include <string>

namespace Rendering
{

class RENDERING_API Texture
{
/* ==================== Non-static member variables begin ==================== */
private:
	GLuint m_textureID;
	GLenum m_textureTarget;
	GLfloat m_filter;
/* ==================== Non-static member variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	Texture(const std::string& fileName, GLenum textureTarget = GL_TEXTURE_2D, GLfloat filter = GL_LINEAR);
	virtual ~Texture(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	void Bind(unsigned int unit = 0) const;
	GLuint GetID() const { return this->m_textureID; };

protected:
	void LoadFromFile(const std::string& fileName);
	void Init(int width, int height, unsigned char* data);
/* ==================== Non-static member functions end ==================== */
}; /* end class Texture */

} /* end namespace Rendering */