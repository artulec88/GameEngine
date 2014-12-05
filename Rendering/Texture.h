#pragma once

#include "Rendering.h"
#include "TextureData.h"
#include <string>
#include <map>

namespace Rendering
{

class Texture
{
/* ==================== Static variables begin ==================== */
private:
	static std::map<std::string, TextureData*> textureResourceMap;
/* ==================== Static variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	RENDERING_API Texture(const std::string& fileName, GLenum textureTarget = GL_TEXTURE_2D, GLfloat filter = GL_LINEAR, GLenum internalFormat = GL_RGBA, GLenum format = GL_RGBA, bool clampEnabled = false, GLenum attachment = GL_NONE);
	RENDERING_API Texture(int width = 0, int height = 0, unsigned char* data = 0, GLenum textureTarget = GL_TEXTURE_2D, GLfloat filter = GL_LINEAR, GLenum internalFormat = GL_RGBA, GLenum format = GL_RGBA, bool clampEnabled = false, GLenum attachment = GL_NONE);
	RENDERING_API virtual ~Texture(void);
private:
	Texture(const Texture& texture) {}
	void operator=(const Texture& texture) {}
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	RENDERING_API void Bind(unsigned int unit = 0) const;
	RENDERING_API void BindAsRenderTarget() const;
	RENDERING_API inline int GetWidth() const { return textureData->GetWidth(); }
	RENDERING_API inline int GetHeight() const { return textureData->GetHeight(); }
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	TextureData* textureData;
	std::string fileName;
/* ==================== Non-static member variables end ==================== */
}; /* end class Texture */

} /* end namespace Rendering */
