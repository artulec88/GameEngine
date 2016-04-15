#pragma once

#include "Rendering.h"
#include "Utility\ReferenceCounter.h"
#include <string>
#include <map>
#include "Math\Matrix.h"

namespace Rendering
{

/// <summary>
/// The low level representation of the texture.
/// </summary>
class TextureData : public Utility::ReferenceCounter
{
/* ==================== Static variables and functions begin ==================== */
public:
	/// <summary> The maximum numbers of bound textures in OpenGL. </summary>
	static const int MAX_BOUND_TEXTURES_COUNT;
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	TextureData(GLenum textureTargets, int widths, int height, int texturesCount, unsigned char** data, GLfloat* filters, GLenum* internalFormat, GLenum* format, bool clampEnabled, GLenum* attachments);
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
	bool Compare(const TextureData& textureData) const;
private:
	void InitTextures(unsigned char** data, GLfloat* filters, GLenum* internalFormat, GLenum* format, bool clampEnabled);
	void InitRenderTargets(GLenum* attachments);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	GLenum m_textureTarget;

	/// <summary>
	/// The number of textures held by the TextureData object.
	/// It is used to associate more than one rendering targets for the texture.
	/// </summary>
	/// <remarks>
	/// See https://www.youtube.com/watch?v=5bIpaXPiPIA&list=PLEETnX-uPtBVG1ao7GCESh2vOayJXDbAl&index=6 (starting around 7:00).
	/// </remarks>
	int m_texturesCount;
	GLuint* m_textureID;
	int m_width;
	int m_height;

	/// <summary>
	/// The framebuffer. In fact, not every texture is going to need a framebuffer,
	/// because we are not going to render to every texture in the scene.
	/// </summary>
	GLuint m_framebuffer;

	/// <summary> The rendering buffer. </summary>
	/// <remarks>
	/// See https://www.youtube.com/watch?v=5bIpaXPiPIA&list=PLEETnX-uPtBVG1ao7GCESh2vOayJXDbAl&index=6 (starting around 40:00).
	/// </remarks>
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
	RENDERING_API Texture(int texturesCount, int width, int height, unsigned char** data, GLenum textureTarget, GLfloat* filters, GLenum* internalFormats, GLenum* formats, bool clampEnabled, GLenum* attachments);
	RENDERING_API virtual ~Texture(void);
//protected:
//	Texture(); // TODO: Uncomment when any class starts inheriting from the Texture class.
private:
	Texture(const Texture& texture) {}
	void operator=(const Texture& texture) {}
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	RENDERING_API void Bind(unsigned int unit = 0, unsigned int textureIndex = 0) const;
	RENDERING_API void BindAsRenderTarget() const;
	RENDERING_API inline int GetWidth() const { return m_textureData->GetWidth(); }
	RENDERING_API inline int GetHeight() const { return m_textureData->GetHeight(); }
	bool Compare(const Texture& texture) const { return m_textureData->Compare(*texture.m_textureData); }
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	TextureData* m_textureData;
	std::string m_fileName;
/* ==================== Non-static member variables end ==================== */
}; /* end class Texture */

class GuiTexture : public Texture
{
/* ==================== Static variables begin ==================== */
/* ==================== Static variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	GuiTexture(const std::string& fileName, const Math::Vector2D& position, const Math::Vector2D scale, GLenum textureTarget = GL_TEXTURE_2D, GLfloat filter = GL_LINEAR_MIPMAP_LINEAR, GLenum internalFormat = GL_RGBA, GLenum format = GL_RGBA, bool clampEnabled = false, GLenum attachment = GL_NONE);
	virtual ~GuiTexture(void);
private:
	GuiTexture(const GuiTexture& guiTexture);
	void operator=(const GuiTexture& guiTexture);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	const Math::Matrix4D& GetTransformationMatrix() const { return m_transformationMatrix; }
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	/// <summary> The transformation matrix. </summary>
	Math::Matrix4D m_transformationMatrix;

	/// <summary>
	/// Position on the screen.
	/// </summary>
	Math::Vector2D m_position;

	/// <summary>
	/// The scale of the texture.
	/// </summary>
	Math::Vector2D m_scale;
/* ==================== Non-static member variables end ==================== */
}; /* end class GuiTexture */

class ParticleTexture : public Texture
{
/* ==================== Static variables begin ==================== */
/* ==================== Static variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	RENDERING_API ParticleTexture(const std::string& fileName, int rowsCount, bool isAdditive);
	RENDERING_API virtual ~ParticleTexture(void);
private:
	ParticleTexture(const ParticleTexture& particleTexture);
	void operator=(const ParticleTexture& particleTexture);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	RENDERING_API int GetRowsCount() const { return m_rowsCount; }
	RENDERING_API bool IsAdditive() const { return m_isAdditive; }
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	/// <summary>
	/// The number of rows (and columns too) in the texture atlas.
	/// </summary>
	int m_rowsCount;

	/// <summary>
	/// For some particles we want additive blending (e.g. magic effects) and for others we want GL_ONE_MINUS_SRC_ALPHA (e.g. smoke).
	/// </summary>
	bool m_isAdditive;
/* ==================== Non-static member variables end ==================== */
}; /* end class ParticleTexture */

//class CubeShadowMapTexture : public Texture
//{
/* ==================== Constructors and destructors begin ==================== */
//public:
//	RENDERING_API CubeShadowMapTexture(int windowWidth, int windowHeight);
//	RENDERING_API virtual ~CubeShadowMapTexture(void);
//private:
//	CubeShadowMapTexture(const CubeShadowMapTexture& cubeShadowMapTexture) {}
//	void operator=(const CubeShadowMapTexture& cubeShadowMapTexture) {}
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
/* ==================== Non-static member variables end ==================== */
//}; /* end class CubeShadowMapTexture */

} /* end namespace Rendering */
