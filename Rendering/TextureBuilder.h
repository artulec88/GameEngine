#ifndef __RENDERING_TEXTURE_BUILDER_H__
#define __RENDERING_TEXTURE_BUILDER_H__

#include "Rendering.h"

#include "Utility/Builder.h"

namespace rendering
{
	class Texture;

	/// <summary>
	/// The texture builder.
	/// </summary>
	class TextureBuilder : public utility::Builder<Texture>
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Texture builder constructor. </summary>
		/// <param name="fileName">
		/// The name of the file in the operating system storing the texture image data.
		/// </param>
		RENDERING_API TextureBuilder(const std::string& fileName);

		/// <summary> Texture builder destructor. </summary>
		RENDERING_API virtual ~TextureBuilder();

		/// <summary> Texture builder copy constructor. </summary>
		/// <param name="textureBuilder"> The reference to texture builder to copy construct from. </param>
		TextureBuilder(TextureBuilder& textureBuilder) = delete;

		/// <summary> Texture builder move constructor. </summary>
		/// <param name="textureBuilder"> The r-value reference to texture builder to move construct from. </param>
		TextureBuilder(TextureBuilder&& textureBuilder) = delete;

		/// <summary> Texture builder copy assignment operator. </summary>
		/// <param name="textureBuilder"> The reference to texture builder to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned texture builder. </returns>
		TextureBuilder& operator=(const TextureBuilder& textureBuilder) = delete;

		/// <summary> Texture builder move assignment operator. </summary>
		/// <param name="textureBuilder"> The r=value reference to texture builder to move assign from. </param>
		/// <returns> The reference to the newly move-assigned texture builder. </returns>
		TextureBuilder& operator=(TextureBuilder&& textureBuilder) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		RENDERING_API void SetDefault() override
		{
			m_fileName = "";
			m_target = m_defaultTarget;
			m_filter = m_defaultFilter;
			m_internalFormat = m_defaultInternalFormat;
			m_format = m_defaultFormat;
			m_wrapping = m_defaultWrapping;
			m_attachment = m_defaultAttachment;
		}

		RENDERING_API Texture Build() override;

		RENDERING_API TextureBuilder& SetFileName(const std::string& fileName)
		{
			m_fileName = fileName;
			return *this;
		}
		RENDERING_API TextureBuilder& SetTarget(GLenum target)
		{
			m_target = target;
			return *this;
		}
		RENDERING_API TextureBuilder& SetFilter(GLfloat filter)
		{
			m_filter = filter;
			return *this;
		}
		RENDERING_API TextureBuilder& SetInternalFormat(GLenum internalFormat)
		{
			m_internalFormat = internalFormat;
			return *this;
		}
		RENDERING_API TextureBuilder& SetFormat(GLenum format)
		{
			m_format = format;
			return *this;
		}
		RENDERING_API TextureBuilder& SetWrapping(GLenum wrapping)
		{
			m_wrapping = wrapping;
			return *this;
		}
		RENDERING_API TextureBuilder& SetAttachment(GLenum attachment)
		{
			m_attachment = attachment;
			return *this;
		}

		const std::string& GetFileName() const { return m_fileName; }
		GLenum GetTarget() const { return m_target; }
		GLfloat GetFilter() const { return m_filter; }
		GLenum GetInternalFormat() const { return m_internalFormat; }
		GLenum GetFormat() const { return m_format; }
		GLenum GetWrapping() const { return m_wrapping; }
		GLenum GetAttachment() const { return m_attachment; }
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		// TODO: These const variables could be constexpr static
		const GLenum m_defaultTarget;
		const GLfloat m_defaultFilter;
		const GLenum m_defaultInternalFormat;
		const GLenum m_defaultFormat;
		const GLenum m_defaultWrapping;
		const GLenum m_defaultAttachment;

		/// <summary> The file name of the texture. </summary>
		std::string m_fileName;

		/// <summary> The OpenGL texture target. </summary>
		GLenum m_target;

		/// <summary>
		/// The minifying and magnifying function that is used whenever the level-of-detail function
		/// used when sampling from the texture determines that the texture should be minified or magnified.
		/// </summary>
		GLfloat m_filter;

		/// <summary>
		/// Specifies the number of color components in the texture. For details see https://www.opengl.org/sdk/docs/man/html/glTexImage2D.xhtml.
		/// </summary>
		GLenum m_internalFormat;

		/// <summary>
		/// Specifies the format of the pixel data.
		/// </summary>
		/// <remarks>The following symbolic values are accepted:
		/// <list type="number">
		/// <item><description><code>GL_RED</code></description></item>
		/// <item><description><code>GL_RG</code></description></item>
		/// <item><description><code>GL_RGB</code></description></item>
		/// <item><description><code>GL_BGR</code></description></item>
		/// <item><description><code>GL_RGBA</code></description></item>
		/// <item><description><code>GL_BGRA</code></description></item>
		/// <item><description><code>GL_RED_INTEGER</code></description></item>
		/// <item><description><code>GL_RG_INTEGER</code></description></item>
		/// <item><description><code>GL_RGB_INTEGER</code></description></item>
		/// <item><description><code>GL_BGR_INTEGER</code></description></item>
		/// <item><description><code>GL_RGBA_INTEGER</code></description></item>
		/// <item><description><code>GL_BGRA_INTEGER</code></description></item>
		/// <item><description><code>GL_STENCIL_INDEX</code></description></item>
		/// <item><description><code>GL_DEPTH_COMPONENT</code></description></item>
		/// <item><description><code>GL_DEPTH_STENCIL</code></description></item>
		/// </list>
		/// <para>For details see https://www.opengl.org/sdk/docs/man/html/glTexImage2D.xhtml. </para>
		/// </remarks>
		GLenum m_format;

		/// <summary>
		/// The wrapping parameter for both (s, t) texture coordinates.
		/// </summary>
		/// <remarks>The following symbolic values are accepted:
		/// <list type="number">
		/// <item><description><code>GL_CLAMP_TO_EDGE</code></description></item>
		/// <item><description><code>GL_CLAMP_TO_BORDER</code></description></item>
		/// <item><description><code>GL_MIRRORED_REPEAT</code></description></item>
		/// <item><description><code>GL_REPEAT</code></description></item>
		/// <item><description><code>GL_MIRROR_CLAMP_TO_EDGE</code></description></item>
		/// </list>
		/// <para>For details see https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glTexParameter.xml. </para>
		/// </remarks>
		GLenum m_wrapping;

		/// <summary>
		/// The attachments for the rendering targets of the texture. Particularly, useful for rendering to texture mechanism.
		/// </summary>
		GLenum m_attachment;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class TextureBuilder */

} /* end namespace rendering */

#endif /* __RENDERING_TEXTURE_BUILDER_H__ */