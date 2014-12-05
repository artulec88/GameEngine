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

/* ==================== Constructors and destructors begin ==================== */
public:
	Texture(const std::string& fileName, GLenum textureTarget = GL_TEXTURE_2D, GLfloat filter = GL_LINEAR, GLenum internalFormat = GL_RGBA, GLenum format = GL_RGBA, bool clampEnabled = false, GLenum attachment = GL_NONE);
	Texture(int width = 0, int height = 0, unsigned char* data = 0, GLenum textureTarget = GL_TEXTURE_2D, GLfloat filter = GL_LINEAR, GLenum internalFormat = GL_RGBA, GLenum format = GL_RGBA, bool clampEnabled = false, GLenum attachment = GL_NONE);
	virtual ~Texture(void);
private:
	Texture(const Texture& texture) {}
	void operator=(const Texture& texture) {}
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	void Bind(unsigned int unit = 0) const;
	void BindAsRenderTarget() const;
	inline int GetWidth() const { return textureData->GetWidth(); }
	inline int GetHeight() const { return textureData->GetHeight(); }
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	TextureData* textureData;
	std::string fileName;
/* ==================== Non-static member variables end ==================== */
}; /* end class Texture */

} /* end namespace Rendering */
