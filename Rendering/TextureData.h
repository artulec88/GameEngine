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
	GLuint textureID;
/* ==================== Non-static member variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	TextureData(GLenum textureTarget);
	virtual ~TextureData(void);
private:
	TextureData(const TextureData& textureData) {} // don't implement
	void operator=(const TextureData& textureData) {} // don't implement
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	GLenum GetTextureTarget() const;
	GLuint GetTextureID() const;
/* ==================== Non-static member functions end ==================== */
}; /* end class TextureData */

} /* end namespace Rendering */