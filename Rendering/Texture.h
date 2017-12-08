#ifndef __RENDERING_TEXTURE_H__
#define __RENDERING_TEXTURE_H__

#include "Rendering.h"

#include "Math/Matrix.h"

#include <string>
#include <vector>

//#define STORE_TEXTURE_FILE_NAME

namespace rendering
{
	class TextureBuilder;

	/// <summary>
	/// The low level representation of the texture.
	/// </summary>
	class TextureData
	{
		/* ==================== Static variables and functions begin ==================== */
	public:
		/// <summary> The maximum numbers of bound textures in OpenGL. </summary>
		static constexpr int MAX_BOUND_TEXTURES_COUNT = 32;
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
		TextureData(const std::string& fileName, GLenum textureTarget, GLfloat filter, GLenum internalFormat, GLenum format, GLenum wrapping, GLenum attachment);
		TextureData(GLenum textureTarget, int width, int height, int texturesCount, unsigned char** data, GLfloat* filters, GLenum* internalFormats, GLenum* formats, GLenum wrapping, GLenum* attachments);

		/// <summary>
		/// Cube map texture data constructor.
		/// </summary>
		TextureData(const std::string& posXFileName, const std::string& negXFileName, const std::string& posYFileName, const std::string& negYFileName, const std::string& posZFileName, const std::string& negZFileName);

		~TextureData();

		TextureData(const TextureData& textureData) = delete;
		TextureData(TextureData&& textureData) noexcept;
		TextureData& operator=(const TextureData& textureData) = delete;
		TextureData& operator=(TextureData&& textureData) = delete;
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
		void Bind(int textureIndex) const;
		void BindAsRenderTarget() const;
		int GetWidth() const { return m_width; }
		int GetHeight() const { return m_height; }
	private:
		void InitTextures(unsigned char** data, GLfloat* filters, GLenum* internalFormat, GLenum* format, GLenum wrapping);
		void InitRenderTargets(GLenum* attachments);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		GLenum m_textureTarget;

		/// <summary>
		/// The number of textures held by the TextureData object.
		/// It is used to associate more than one rendering targets for the texture.
		/// </summary>
		/// <remarks>
		/// See https://www.youtube.com/watch?v=5bIpaXPiPIA&list=PLEETnX-uPtBVG1ao7GCESh2vOayJXDbAl&index=6 (starting around 7:00).
		/// </remarks>
		int m_texturesCount;
		std::vector<GLuint> m_textureIDs;
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
		/// <summary>
		/// The <code>Texture</code> objects comparator.
		/// </summary>
		/// <remarks>
		/// It is more efficient to pass a function-object in a function call than pass function pointers.
		/// That is why, instead of creating a Compare(const Texture& other) method in the <code>Texture</code> class itself and pass the pointer to this function whenever it is needed,
		/// it is better to create a <code>TextureComparator</code> object and pass its temporary instance. The reason is that function-objects are almost always inlined by the compiler.
		/// See https://en.wikibooks.org/wiki/Optimizing_C%2B%2B/Writing_efficient_code/Performance_improving_features#Function-objects.
		/// </remarks>
		//class TextureComparator
		//{
		//public:
		//	bool operator() (const Texture& lhs, const Texture& rhs) const
		//	{
		//		return lhs.m_textureData < rhs.m_textureData;
		//	}
		//}; // end class TextureComparator

		/* ==================== Static variables begin ==================== */
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		// TODO: Remove all other constructors in the future. Only the one with the TextureBuilder should remain.
		/// <summary> Texture constructor. </summary>
		/// <param name="textureBuilder">
		/// The texture builder storing all the required information to properly initialize a texture.
		/// </param>
		RENDERING_API explicit Texture(const TextureBuilder& textureBuilder);
		RENDERING_API explicit Texture(const std::string& fileName, GLenum textureTarget = GL_TEXTURE_2D, GLfloat filter = GL_LINEAR_MIPMAP_LINEAR, GLenum internalFormat = GL_RGBA, GLenum format = GL_RGBA, GLenum wrapping = GL_REPEAT, GLenum attachment = GL_NONE);
		RENDERING_API explicit Texture(int width = 0, int height = 0, unsigned char* data = nullptr, GLenum textureTarget = GL_TEXTURE_2D, GLfloat filter = GL_LINEAR_MIPMAP_LINEAR, GLenum internalFormat = GL_RGBA, GLenum format = GL_RGBA, GLenum wrapping = GL_REPEAT, GLenum attachment = GL_NONE);
		RENDERING_API Texture(int texturesCount, int width, int height, unsigned char** data, GLenum textureTarget, GLfloat* filters, GLenum* internalFormats, GLenum* formats, GLenum wrapping, GLenum* attachments);
		RENDERING_API Texture(const std::string& posXFileName, const std::string& negXFileName, const std::string& posYFileName, const std::string& negYFileName, const std::string& posZFileName, const std::string& negZFileName);

		RENDERING_API ~Texture();

		/// <summary> Texture copy constructor. </summary>
		Texture(const Texture& texture) = delete;

		/// <summary> Texture move constructor. </summary>
		RENDERING_API Texture(Texture&& texture) noexcept;

		/// <summary> Texture copy assignment operator. </summary>
		Texture& operator=(const Texture& texture) = delete;

		/// <summary> Texture move assignment operator. </summary>
		Texture& operator=(Texture&& texture) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		RENDERING_API void Bind(unsigned int unit = 0, unsigned int textureIndex = 0) const;
		RENDERING_API void BindAsRenderTarget() const;
		RENDERING_API int GetWidth() const { return m_textureData.GetWidth(); }
		RENDERING_API int GetHeight() const { return m_textureData.GetHeight(); }
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		TextureData m_textureData;
#ifdef STORE_TEXTURE_FILE_NAME
		std::string m_fileName;
#endif
		/* ==================== Non-static member variables end ==================== */
	}; /* end class Texture */

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

} /* end namespace rendering */

#endif /* __RENDERING_TEXTURE_H__ */