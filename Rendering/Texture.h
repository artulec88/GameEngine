#pragma once

#include "Rendering.h"
#include "TextureData.h"
#include <string>
#include <map>

namespace Rendering
{

class RENDERING_API Texture
{
/* ==================== Static variables begin ==================== */
private:
	static std::map<std::string, TextureData*> textureResourceMap;
/* ==================== Static variables end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	TextureData* textureData;
	std::string fileName;
/* ==================== Non-static member variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	Texture(const std::string& fileName, GLenum textureTarget = GL_TEXTURE_2D, GLfloat filter = GL_LINEAR);
	Texture(int width = 0, int height = 0, unsigned char* data = 0, GLenum textureTarget = GL_TEXTURE_2D, GLfloat filter = GL_LINEAR);
	virtual ~Texture(void);
private:
	Texture(const Texture& texture) {}
	void operator=(const Texture& texture) {}
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	void Bind(unsigned int unit = 0) const;

protected:
	void Init(int width, int height, unsigned char* data, GLenum textureTarget, GLfloat filter);
/* ==================== Non-static member functions end ==================== */
}; /* end class Texture */

} /* end namespace Rendering */