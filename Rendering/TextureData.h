#pragma once

#include "Rendering.h"

#include "Utility\ReferenceCounter.h"

namespace Rendering
{

class TextureData : public Utility::ReferenceCounter
{
/* ==================== Static variables and functions begin ==================== */
public:
	static const int MAX_BOUND_TEXTURES_COUNT = 32; // The max number of bound textures in OpenGL
/* ==================== Static variables and functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	GLenum textureTarget;
	int texturesCount;
	GLuint* textureID;
	int width;
	int height;
	GLuint framebuffer;
	GLuint renderbuffer;
/* ==================== Non-static member variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	TextureData(GLenum textureTarget, int width, int height, int texturesCount, unsigned char** data, GLfloat* filters, GLenum* internalFormat, GLenum* format, bool clampEnabled, GLenum* attachments);
	virtual ~TextureData(void);
private:
	TextureData(const TextureData& textureData) {} // don't implement
	void operator=(const TextureData& textureData) {} // don't implement
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	void Bind(int textureIndex) const;
	void BindAsRenderTarget() const;
	inline int GetWidth() const { return this->width; }
	inline int GetHeight() const { return this->height; }
private:
	void InitTextures(unsigned char** data, GLfloat* filters, GLenum* internalFormat, GLenum* format, bool clampEnabled);
	void InitRenderTargets(GLenum* attachments);
/* ==================== Non-static member functions end ==================== */
}; /* end class TextureData */

} /* end namespace Rendering */
