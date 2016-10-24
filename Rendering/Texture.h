#ifndef __RENDERING_TEXTURE_H__
#define __RENDERING_TEXTURE_H__

#include "Rendering.h"

#include "Math\Matrix.h"

#include <string>
#include <map>

//#define STORE_TEXTURE_FILE_NAME

namespace Rendering
{

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
	public:
		TextureData(const std::string& fileName, GLenum textureTarget, GLfloat filter, GLenum internalFormat, GLenum format, GLenum wrapping, GLenum attachment);
		TextureData(GLenum textureTarget, int width, int height, int texturesCount, unsigned char** data, GLfloat* filters, GLenum* internalFormats, GLenum* formats, GLenum wrapping, GLenum* attachments);
		/// <summary>
		/// Cube map texture data constructor.
		/// </summary>
		TextureData(const std::string& posXFileName, const std::string& negXFileName, const std::string& posYFileName, const std::string& negYFileName, const std::string& posZFileName, const std::string& negZFileName);
		~TextureData(void);
		TextureData(const TextureData& textureData) = delete;
		TextureData(TextureData&& textureData);
		TextureData& operator=(const TextureData& textureData) = delete;
		TextureData& operator=(TextureData&& textureData) = delete;
	/* ==================== Constructors and destructors end ==================== */

	/* ==================== Non-static member functions begin ==================== */
	public:
		void Bind(int textureIndex) const;
		void BindAsRenderTarget() const;
		inline int GetWidth() const { return m_width; }
		inline int GetHeight() const { return m_height; }
	private:
		void InitTextures(unsigned char** data, GLfloat* filters, GLenum* internalFormat, GLenum* format, GLenum wrapping);
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
		/// <summary> Texture constructor. </summary>
		/// <param name="fileName"> The file name of the texture. </param>
		/// <param name="textureTarget"> The OpenGL texture target. </param>
		/// <param name="filter">
		/// The minifying and magnifying function that is used whenever the level-of-detail function used when sampling from the texture determines that the texture should be minified or magnified.
		/// </param>
		/// <param name="internalFormat">
		/// Specifies the number of color components in the texture. For details see https://www.opengl.org/sdk/docs/man/html/glTexImage2D.xhtml.
		/// </param>
		/// <param name="format">
		/// Specifies the format of the pixel data. The following symbolic values are accepted: <code>GL_RED</code>, <code>GL_RG</code>, <code>GL_RGB</code>,
		/// <code>GL_BGR</code>, <code>GL_RGBA</code>, <code>GL_BGRA</code>, <code>GL_RED_INTEGER</code>, <code>GL_RG_INTEGER</code>, <code>GL_RGB_INTEGER</code>,
		/// <code>GL_BGR_INTEGER</code>, <code>GL_RGBA_INTEGER</code>, <code>GL_BGRA_INTEGER</code>, <code>GL_STENCIL_INDEX</code>, <code>GL_DEPTH_COMPONENT</code>,
		/// <code>GL_DEPTH_STENCIL</code>. For details see https://www.opengl.org/sdk/docs/man/html/glTexImage2D.xhtml.
		/// </param>
		/// <param name="wrapping">
		/// The wrapping parameter for both (s, t) texture coordinates. Can be either <code>GL_CLAMP_TO_EDGE</code>, <code>GL_CLAMP_TO_BORDER</code>, <code>GL_MIRRORED_REPEAT</code>,
		/// <code>GL_REPEAT</code>, or <code>GL_MIRROR_CLAMP_TO_EDGE</code>. See https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glTexParameter.xml for details.
		/// </param>
		/// <param name="attachment"> The attachments for the rendering targets of the texture. Particularly, useful for rendering to texture mechanism. </param>
		RENDERING_API Texture(const std::string& fileName, GLenum textureTarget = GL_TEXTURE_2D, GLfloat filter = GL_LINEAR_MIPMAP_LINEAR, GLenum internalFormat = GL_RGBA, GLenum format = GL_RGBA, GLenum wrapping = GL_REPEAT, GLenum attachment = GL_NONE);
		RENDERING_API Texture(int width = 0, int height = 0, unsigned char* data = NULL, GLenum textureTarget = GL_TEXTURE_2D, GLfloat filter = GL_LINEAR_MIPMAP_LINEAR, GLenum internalFormat = GL_RGBA, GLenum format = GL_RGBA, GLenum wrapping = GL_REPEAT, GLenum attachment = GL_NONE);
		RENDERING_API Texture(int texturesCount, int width, int height, unsigned char** data, GLenum textureTarget, GLfloat* filters, GLenum* internalFormats, GLenum* formats, GLenum wrapping, GLenum* attachments);
		RENDERING_API Texture(const std::string& posXFileName, const std::string& negXFileName, const std::string& posYFileName, const std::string& negYFileName, const std::string& posZFileName, const std::string& negZFileName);
		RENDERING_API virtual ~Texture(void);
		/// <summary> Texture copy constructor. </summary>
		Texture(const Texture& texture) = delete;
		/// <summary> Texture move constructor. </summary>
		RENDERING_API Texture(Texture&& texture);
		/// <summary> Texture copy assignment operator. </summary>
		Texture& operator=(const Texture& texture) = delete;
		/// <summary> Texture move assignment operator. </summary>
		Texture& operator=(Texture&& texture) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		RENDERING_API void Bind(unsigned int unit = 0, unsigned int textureIndex = 0) const;
		RENDERING_API void BindAsRenderTarget() const;
		RENDERING_API inline int GetWidth() const { return m_textureData.GetWidth(); }
		RENDERING_API inline int GetHeight() const { return m_textureData.GetHeight(); }
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		TextureData m_textureData;
#ifdef STORE_TEXTURE_FILE_NAME
		std::string m_fileName;
#endif
		/* ==================== Non-static member variables end ==================== */
	}; /* end class Texture */

	class GuiTexture : public Texture
	{
		/* ==================== Static variables begin ==================== */
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> GUI texture constructor. </summary>
		/// <param name="fileName"> The file name of the texture. </param>
		/// <param name="position"> The screen position of the GUI texture. </param>
		/// <param name="rotationAngle"> The rotation angle on the screen of the GUI texture. </param>
		/// <param name="scale"> The scale used when rendering the GUI texture. </param>
		/// <param name="textureTarget"> The OpenGL texture target. </param>
		/// <param name="filter">
		/// The minifying and magnifying function that is used whenever the level-of-detail function used when sampling from the texture determines that the texture should be minified or magnified.
		/// </param>
		/// <param name="internalFormat">
		/// Specifies the number of color components in the texture. For details see https://www.opengl.org/sdk/docs/man/html/glTexImage2D.xhtml.
		/// </param>
		/// <param name="format">
		/// Specifies the format of the pixel data. The following symbolic values are accepted: <code>GL_RED</code>, <code>GL_RG</code>, <code>GL_RGB</code>,
		/// <code>GL_BGR</code>, <code>GL_RGBA</code>, <code>GL_BGRA</code>, <code>GL_RED_INTEGER</code>, <code>GL_RG_INTEGER</code>, <code>GL_RGB_INTEGER</code>,
		/// <code>GL_BGR_INTEGER</code>, <code>GL_RGBA_INTEGER</code>, <code>GL_BGRA_INTEGER</code>, <code>GL_STENCIL_INDEX</code>, <code>GL_DEPTH_COMPONENT</code>,
		/// <code>GL_DEPTH_STENCIL</code>. For details see https://www.opengl.org/sdk/docs/man/html/glTexImage2D.xhtml.
		/// </param>
		/// <param name="wrapping">
		/// The wrapping parameter for both (s, t) texture coordinates. Can be either <code>GL_CLAMP_TO_EDGE</code>, <code>GL_CLAMP_TO_BORDER</code>, <code>GL_MIRRORED_REPEAT</code>,
		/// <code>GL_REPEAT</code>, or <code>GL_MIRROR_CLAMP_TO_EDGE</code>. See https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glTexParameter.xml for details.
		/// </param>
		/// <param name="attachment"> The attachments for the rendering targets of the texture. Particularly, useful for rendering to texture mechanism. </param>
		RENDERING_API GuiTexture(const std::string& fileName, const Math::Vector2D& position, const Math::Angle& rotationAngle, const Math::Vector2D& scale, GLenum textureTarget = GL_TEXTURE_2D,
			GLfloat filter = GL_LINEAR_MIPMAP_LINEAR, GLenum internalFormat = GL_RGBA, GLenum format = GL_RGBA, GLenum wrapping = GL_REPEAT, GLenum attachment = GL_NONE);
		/// <summary> GUI texture destructor. </summary>
		RENDERING_API virtual ~GuiTexture(void);
		/// <summary> GUI texture copy constructor. </summary>
		/// <param name="guiTexture"> The GUI texture we copy from. </param>
		GuiTexture(const GuiTexture& guiTexture) = delete;
		/// <summary> GUI texture move constructor. </summary>
		/// <param name="guiTexture"> The GUI texture we move from. </param>
		GuiTexture(GuiTexture&& guiTexture);
		/// <summary> GUI texture copy assignment operator. </summary>
		/// <param name="guiTexture"> The GUI texture we copy from. </param>
		GuiTexture& operator=(const GuiTexture& guiTexture) = delete;
		/// <summary> GUI texture move assignment operator. </summary>
		/// <param name="guiTexture"> The GUI texture we move from. </param>
		GuiTexture& operator=(GuiTexture&& guiTexture) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		/// <summary> Retrieves the transformation matrix of the GUI texture. </param>
		/// <returns> The transformation matrix of the GUI texture. </param>
		RENDERING_API const Math::Matrix4D& GetTransformationMatrix() const { return m_transformationMatrix; }
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
		/// The scale of the GUI texture.
		/// </summary>
		Math::Vector2D m_scale;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class GuiTexture */

	namespace Particles
	{
		class ParticleTexture : public Texture
		{
			/* ==================== Static variables begin ==================== */
			/* ==================== Static variables end ==================== */

			/* ==================== Constructors and destructors begin ==================== */
		public:
			RENDERING_API ParticleTexture(const std::string& fileName, int rowsCount, bool isAdditive);
			RENDERING_API virtual ~ParticleTexture(void);
			/// <summary> Particle texture copy constructor. </summary>
			ParticleTexture(const ParticleTexture& particleTexture) = delete;
			/// <summary> Particle texture move constructor. </summary>
			RENDERING_API ParticleTexture(ParticleTexture&& particleTexture);
			/// <summary> Particle texture copy assignment operator. </summary>
			ParticleTexture& operator=(const ParticleTexture& particleTexture) = delete;
			/// <summary> Particle texture move assignment operator. </summary>
			ParticleTexture& operator=(ParticleTexture&& particleTexture) = delete;
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
	} /* end namespace Particles */

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

#endif /* __RENDERING_TEXTURE_H__ */