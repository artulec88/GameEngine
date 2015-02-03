#pragma once

#include "Rendering.h"
#include "Utility\ReferenceCounter.h"
#include <string>
#include <map>

namespace Rendering
{

class TextureData : public Utility::ReferenceCounter
{
/* ==================== Static variables and functions begin ==================== */
public:
	static const int MAX_BOUND_TEXTURES_COUNT = 32; // The max number of bound textures in OpenGL
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	TextureData(GLenum textureTarget, int width, int height, int texturesCount, unsigned char** data, GLfloat* filters, GLenum* internalFormat, GLenum* format, bool clampEnabled, GLenum* attachments);
	TextureData(unsigned char** cubeMapTextureData, int width, int height, int depth); // cube map texture data
	virtual ~TextureData(void);
private:
	TextureData(const TextureData& textureData) {} // don't implement
	void operator=(const TextureData& textureData) {} // don't implement
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	void Bind(int textureIndex) const;
	void BindAsRenderTarget() const;
	inline int GetWidth() const { return m_width; }
	inline int GetHeight() const { return m_height; }
private:
	void InitTextures(unsigned char** data, GLfloat* filters, GLenum* internalFormat, GLenum* format, bool clampEnabled);
	void InitRenderTargets(GLenum* attachments);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	GLenum m_textureTarget;
	int m_texturesCount;
	GLuint* m_textureID;
	int m_width;
	int m_height;
	GLuint m_framebuffer;
	GLuint m_renderbuffer;
/* ==================== Non-static member variables end ==================== */
}; /* end class TextureData */

class Texture
{
/* ==================== Static variables begin ==================== */
protected:
	static std::map<std::string, TextureData*> s_textureResourceMap;
/* ==================== Static variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	RENDERING_API Texture(const std::string& fileName, GLenum textureTarget = GL_TEXTURE_2D, GLfloat filter = GL_LINEAR_MIPMAP_LINEAR, GLenum internalFormat = GL_RGBA, GLenum format = GL_RGBA, bool clampEnabled = false, GLenum attachment = GL_NONE);
	RENDERING_API Texture(const std::string& posXFileName, const std::string& negXFileName, const std::string& posYFileName, const std::string& negYFileName, const std::string& posZFileName, const std::string& negZFileName);
	RENDERING_API Texture(int width = 0, int height = 0, unsigned char* data = NULL, GLenum textureTarget = GL_TEXTURE_2D, GLfloat filter = GL_LINEAR_MIPMAP_LINEAR, GLenum internalFormat = GL_RGBA, GLenum format = GL_RGBA, bool clampEnabled = false, GLenum attachment = GL_NONE);
	RENDERING_API virtual ~Texture(void);
//protected:
//	Texture(
private:
	Texture(const Texture& texture) {}
	void operator=(const Texture& texture) {}
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	RENDERING_API void Bind(unsigned int unit = 0) const;
	RENDERING_API void BindAsRenderTarget() const;
	RENDERING_API inline int GetWidth() const { return m_textureData->GetWidth(); }
	RENDERING_API inline int GetHeight() const { return m_textureData->GetHeight(); }
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	TextureData* m_textureData;
private:
	std::string m_fileName;
/* ==================== Non-static member variables end ==================== */
}; /* end class Texture */

} /* end namespace Rendering */
