#pragma once

#include "Rendering.h"

#include "Utility\ReferenceCounter.h"

namespace Rendering
{

class TextureData : public Utility::ReferenceCounter
{
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
	void Bind(int textureIndex);
	void BindAsRenderTarget();
private:
	void InitTextures(unsigned char** data, GLfloat* filters, GLenum* internalFormat, GLenum* format, bool clampEnabled);
	void InitRenderTargets(GLenum* attachments);
/* ==================== Non-static member functions end ==================== */
}; /* end class TextureData */

} /* end namespace Rendering */