#include "StdAfx.h"
#include "Texture.h"
#include "stb_image.h"
#include "Image.h"
#include "TextureBuilder.h"

#include "Math/Math.h"

#include "Utility/Utility.h"
#include "Utility/ILogger.h"

/* ==================== TextureData class implementation begin ==================== */
rendering::TextureData::TextureData(const std::string& fileName, GLenum textureTarget, GLfloat filter, GLenum internalFormat, GLenum format, GLenum wrapping,
	GLenum attachment) :
	m_textureTarget(textureTarget),
	m_texturesCount(1),
	m_textureIDs(m_texturesCount),
	m_width(0),
	m_height(0),
	m_framebuffer(0),
	m_renderbuffer(0)
{
	CHECK_CONDITION_EXIT_RENDERING(m_texturesCount > 0, utility::logging::EMERGENCY, "Incorrect number of textures specified (", m_texturesCount, ").");
	CHECK_CONDITION_EXIT_RENDERING(m_texturesCount <= MAX_BOUND_TEXTURES_COUNT, utility::logging::ERR, "Maximum number of bound textures (", MAX_BOUND_TEXTURES_COUNT, ") exceeded. Buffer overrun might occur.");
	CheckErrorCode(__FUNCTION__, "Creating texture data");

	Image image(fileName, STBI_rgb_alpha);
	m_width = image.GetWidth();
	m_height = image.GetHeight();
	auto imageData = image.GetData();
	InitTextures(&imageData, &filter, &internalFormat, &format, wrapping);
	if (attachment != GL_NONE)
	{
		InitRenderTargets(&attachment);
	}
	DEBUG_LOG_RENDERING("Loading texture from file \"", fileName, "\" finished successfully");
}

rendering::TextureData::TextureData(GLenum textureTarget, int width, int height, int texturesCount, unsigned char** data, GLfloat* filters, GLenum* internalFormats, GLenum* formats, GLenum wrapping, GLenum* attachments) :
	m_textureTarget(textureTarget),
	m_texturesCount(texturesCount),
	m_textureIDs(texturesCount),
	m_width(width),
	m_height(height),
	m_framebuffer(0),
	m_renderbuffer(0)
{
	CHECK_CONDITION_EXIT_RENDERING(m_texturesCount > 0, utility::logging::EMERGENCY, "Incorrect number of textures specified (", m_texturesCount, ").");
	CHECK_CONDITION_EXIT_RENDERING(m_texturesCount <= MAX_BOUND_TEXTURES_COUNT, utility::logging::ERR, "Maximum number of bound textures (", MAX_BOUND_TEXTURES_COUNT, ") exceeded. Buffer overrun might occur.");

	CheckErrorCode(__FUNCTION__, "Creating texture data");
	InitTextures(data, filters, internalFormats, formats, wrapping);
	if (attachments != nullptr)
	{
		InitRenderTargets(attachments);
	}
}

rendering::TextureData::TextureData(const std::string& posXFileName, const std::string& negXFileName, const std::string& posYFileName, const std::string& negYFileName, const std::string& posZFileName, const std::string& negZFileName) :
	m_textureTarget(GL_TEXTURE_CUBE_MAP),
	m_texturesCount(1),
	m_textureIDs(1),
	m_width(0),
	m_height(0),
	m_framebuffer(0),
	m_renderbuffer(0)
{
	constexpr auto cubeMapFacesCount = 6;

	std::array<Image, cubeMapFacesCount> cubeMapImages = { Image(posXFileName, STBI_rgb_alpha), Image(negXFileName, STBI_rgb_alpha), Image(posYFileName, STBI_rgb_alpha), Image(negYFileName, STBI_rgb_alpha), Image(posZFileName, STBI_rgb_alpha), Image(negZFileName, STBI_rgb_alpha) };
	for (auto i = 0; i < cubeMapFacesCount; ++i)
	{
		CHECK_CONDITION_EXIT_ALWAYS_RENDERING(cubeMapImages[i].GetData() != nullptr, utility::logging::EMERGENCY,
			"Unable to load texture #", i, " for the cube map");
		if (i > 0)
		{
			if (cubeMapImages[i].GetWidth() != cubeMapImages[i - 1].GetWidth())
			{
				ERROR_LOG_RENDERING("All cube map texture's faces must have the same width, but face ", i, " has width=",
					cubeMapImages[i].GetWidth(), " and face ", i - 1, " has width=", cubeMapImages[i - 1].GetWidth());
			}
			if (cubeMapImages[i].GetHeight() != cubeMapImages[i - 1].GetHeight())
			{
				ERROR_LOG_RENDERING("All cube map texture's faces must have the same height, but face ", i, " has height=",
					cubeMapImages[i].GetHeight(), " and face ", i - 1, " has height=", cubeMapImages[i - 1].GetHeight());
			}
		}
	}

	// TODO: Pass correct values for width, height and depth. The values below will only work for square textures with each face having the same size.
	m_width = cubeMapImages[0].GetWidth();
	m_height = cubeMapImages[0].GetHeight();
	//int depth = cubeMapImages[0].GetWidth();
	
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(m_texturesCount, &m_textureIDs[0]);
	CheckErrorCode(__FUNCTION__, "Generating cube map texture");
	Bind(0);

	const GLuint targets[] = {
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};
	for (auto i = 0; i < cubeMapFacesCount; ++i)
	{
		glTexImage2D(targets[i], 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, cubeMapImages[i].GetData());
	}
	glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(m_textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(m_textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(m_textureTarget, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// Init textures end

	InitRenderTargets(GL_NONE);
}

rendering::TextureData::~TextureData()
{
	if (!m_textureIDs.empty())
	{
		glDeleteTextures(m_texturesCount, &m_textureIDs[0]);
		DELOCUST_LOG_RENDERING("TextureData destroyed all textures.");
	}
	if (m_framebuffer)
	{
		glDeleteFramebuffers(1, &m_framebuffer);
		DELOCUST_LOG_RENDERING("TextureData destroyed the framebuffer.");
	}
	if (m_renderbuffer)
	{
		glDeleteRenderbuffers(1, &m_renderbuffer);
		DELOCUST_LOG_RENDERING("TextureData destroyed the renderbuffer.");
	}
	DELOCUST_LOG_RENDERING("TextureData destroyed.");
}

rendering::TextureData::TextureData(TextureData&& textureData) noexcept :
	m_textureTarget(std::move(textureData.m_textureTarget)),
	m_texturesCount(std::move(textureData.m_texturesCount)),
	m_textureIDs(move(textureData.m_textureIDs)),
	m_width(std::move(textureData.m_width)),
	m_height(std::move(textureData.m_height)),
	m_framebuffer(std::move(textureData.m_framebuffer)),
	m_renderbuffer(std::move(textureData.m_renderbuffer))
{
	textureData.m_textureIDs.clear(); // TODO: Is it correct?
	textureData.m_framebuffer = 0;
	textureData.m_renderbuffer = 0;
}

void rendering::TextureData::InitTextures(unsigned char** data, GLfloat* filters, GLenum* internalFormat, GLenum* format, GLenum wrapping)
{
	if (filters == nullptr)
	{
		WARNING_LOG_RENDERING("The filter array is NULL.");
	}

	glGenTextures(m_texturesCount, &m_textureIDs[0]);
	CheckErrorCode(__FUNCTION__, "Generating textures");
	for (auto i = 0; i < m_texturesCount; ++i)
	{
		glBindTexture(m_textureTarget, m_textureIDs[i]);
		glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, filters[i]);
		CheckErrorCode(__FUNCTION__, "Setting the GL_TEXTURE_MIN_FILTER");
		if (filters[i] == GL_NEAREST /* || filters[i] == GL_LINEAR is not necessary, because GL_LINEAR is used even when GL_TEXTURE_MAG_FILTER is not specified. */)
		{
			/**
			 * GL_TEXTURE_MAG_FILTER only accepts two possible values: GL_NEAREST and GL_LINEAR (default).
			 * We cannot use GL_*_MIPMAP_* values as they give errors GL_INVALID_ENUM.
			 * See link https://www.opengl.org/wiki/GLAPI/glTexParameter (search for GL_TEXTURE_MAG_FILTER)
			 */
			glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, filters[i] /* GL_NEAREST */);
			CheckErrorCode(__FUNCTION__, "Setting the GL_TEXTURE_MAG_FILTER");
		}

		glTexParameterf(m_textureTarget, GL_TEXTURE_WRAP_S, wrapping);
		glTexParameterf(m_textureTarget, GL_TEXTURE_WRAP_T, wrapping);
		// TODO: Check if this parameter is ok for all texture targets. It is ok though for GL_TEXTURE_CUBE_MAP texture target. What about GL_TEXTURE_2D?
		//glTexParameterf(textureTarget, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		CheckErrorCode(__FUNCTION__, "Setting the GL_TEXTURE_WRAP_* wrapping parameters.");

		// If we want to use mipmapping we must submit the data to the texture before.
		glTexImage2D(m_textureTarget, 0, internalFormat[i], m_width, m_height, 0, format[i], GL_UNSIGNED_BYTE, data == nullptr ? nullptr : data[i]);

		if(filters[i] == GL_NEAREST_MIPMAP_NEAREST ||
			filters[i] == GL_NEAREST_MIPMAP_LINEAR ||
			filters[i] == GL_LINEAR_MIPMAP_NEAREST ||
			filters[i] == GL_LINEAR_MIPMAP_LINEAR) // do we use mipmapping?
		{
			glGenerateMipmap(m_textureTarget);
			CheckErrorCode(__FUNCTION__, "Generating mipmaps");
			// Watch https://www.youtube.com/watch?v=psD5Eoabbng&index=19&list=PLEETnX-uPtBVG1ao7GCESh2vOayJXDbAl (starts around 8:30) to know more about anisotropic filtering.
			// Basically, anisotropic filtering is for making the mipmapping work a little bit better when viewing the textured model from a very shallow angle.
			GLfloat maxAnisotropy; // the maximum number of samples per pixel used for mipmapping filtering
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
			//DEBUG_LOG_RENDERING("Maximum anisotropy supported = ", maxAnisotropy);
			glTexParameterf(m_textureTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, math::Clamp(0.0f, 8.0f, maxAnisotropy));
			//glTexParameterf(m_textureTarget, GL_TEXTURE_LOD_BIAS, -0.4f /* the lower the value the higher the resolution->lower performance */); //TODO: The value we give here is crucial for the performance. Reasearch that!
		}
		else
		{
			glTexParameteri(m_textureTarget, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(m_textureTarget, GL_TEXTURE_MAX_LEVEL, 0);
		}
	}
}

void rendering::TextureData::Bind(int textureIndex) const
{
	CHECK_CONDITION_RETURN_VOID_RENDERING(textureIndex >= 0 && textureIndex < m_texturesCount, utility::logging::CRITICAL,
		"Cannot bind the texture with textureID=", textureIndex, ". This value is out of range [0; ", m_texturesCount, ")");
	glBindTexture(m_textureTarget, m_textureIDs[textureIndex]);
}

void rendering::TextureData::InitRenderTargets(GLenum* attachments)
{
	CHECK_CONDITION_RETURN_VOID_ALWAYS_RENDERING(attachments != nullptr, utility::logging::DEBUG, "Cannot initialize rendering targets for the texture. No attachments specified.");
	CHECK_CONDITION_EXIT_RENDERING(m_texturesCount <= MAX_BOUND_TEXTURES_COUNT, utility::logging::ERR, "Maximum number of bound textures (", MAX_BOUND_TEXTURES_COUNT, ") exceeded. Buffer overrun might occur.");

	std::vector<GLenum> drawBuffers(m_texturesCount);
	auto hasDepth = false;

	for (auto i = 0; i < m_texturesCount; ++i)
	{
		DELOCUST_LOG_RENDERING("The texture uses ", attachments[i], " as an attachment");
		if ( attachments[i] == GL_DEPTH_ATTACHMENT || attachments[i] == GL_STENCIL_ATTACHMENT )
		{
			/**
			 * The draw buffers are only concerned about the fragment shader's output so in fact they are not needed for depth and stencil buffers.
			 * The reason for this is that one does not write to depth nor stencil buffer in the FS.
			 * That is why for GL_DEPTH_ATTACHMENT and GL_STENCIL_ATTACHMENT we set the drawBuffers to GL_NONE.
			 */
			drawBuffers[i] = GL_NONE;
			if (attachments[i] == GL_DEPTH_ATTACHMENT)
			{
				hasDepth = true;
			}
		}
		else
		{
			drawBuffers[i] = attachments[i];
		}
		if (attachments[i] == GL_NONE)
		{
			continue;
		}
		if (m_framebuffer == 0)
		{
			glGenFramebuffers(1, &m_framebuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
		}
		// Associate the texture with the framebuffer
		//glFramebufferTexture(GL_FRAMEBUFFER, attachments[i], m_textureID[i], 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachments[i], m_textureTarget, m_textureIDs[i], 0 /* which mipmap level of the texture we want to use */);
	}

	if (m_framebuffer == 0)
	{
		DEBUG_LOG_RENDERING("Framebuffer will not be used by the texture (textures count = ", m_texturesCount, ")");
		return;
	}
	if (!hasDepth)
	{
		DELOCUST_LOG_RENDERING("The texture does not have any depth attachment. Creating the render buffer is started.");
		glGenRenderbuffers(1, &m_renderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_renderbuffer);
	}
	glDrawBuffers(m_texturesCount, drawBuffers.data());

	CHECK_CONDITION_EXIT_RENDERING(GL_FRAMEBUFFER_COMPLETE == glCheckFramebufferStatus(GL_FRAMEBUFFER), utility::logging::CRITICAL,
		"Framebuffer creation failed. The framebuffer status is not GL_FRAMEBUFFER_COMPLETE. Instead it is ", status, ".");
}

void rendering::TextureData::BindAsRenderTarget() const
{
	glBindTexture(GL_TEXTURE_2D, 0); // just to make sure the texture is not bound
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
	glViewport(0, 0, m_width, m_height);
}
/* ==================== TextureData class implementation end ==================== */

/* ==================== Texture class implementation begin ==================== */
rendering::Texture::Texture(const TextureBuilder& textureBuilder) :
	m_textureData(textureBuilder.GetFileName(), textureBuilder.GetTarget(), textureBuilder.GetFilter(), textureBuilder.GetInternalFormat(),
		textureBuilder.GetFormat(), textureBuilder.GetWrapping(), textureBuilder.GetAttachment())
#ifdef STORE_TEXTURE_FILE_NAME
	, m_fileName(textureBuilder.GetFileName())
#endif
{
}

rendering::Texture::Texture(const std::string& fileName, GLenum textureTarget /* = GL_TEXTURE_2D */, GLfloat filter /* = GL_LINEAR_MIPMAP_LINEAR */, GLenum internalFormat /* = GL_RGBA */,
	GLenum format /* = GL_RGBA */, GLenum wrapping /* = GL_REPEAT */, GLenum attachment /* = GL_NONE */) :
	m_textureData(fileName, textureTarget, filter, internalFormat, format, wrapping, attachment)
#ifdef STORE_TEXTURE_FILE_NAME
	, m_fileName(fileName)
#endif
{
	DEBUG_LOG_RENDERING("Texture \"", fileName, "\" has been created.");
}

rendering::Texture::Texture(int width /* = 0 */, int height /* = 0 */, unsigned char* data /* = 0 */, GLenum textureTarget /* = GL_TEXTURE_2D */, GLfloat filter /* = GL_LINEAR_MIPMAP_LINEAR */,
	GLenum internalFormat /* = GL_RGBA */, GLenum format /* = GL_RGBA */, GLenum wrapping /* = GL_REPEAT */, GLenum attachment /* = GL_NONE */) :
	m_textureData(textureTarget, width, height, 1, &data, &filter, &internalFormat, &format, wrapping, &attachment)
#ifdef STORE_TEXTURE_FILE_NAME
	, m_fileName()
#endif
{
	CHECK_CONDITION_RENDERING((width > 0) && (height > 0), utility::logging::ERR, "Cannot initialize texture. Passed texture size is incorrect (width=", width, "; height=", height, ")");
	CHECK_CONDITION_EXIT_RENDERING(m_textureData != nullptr, utility::logging::ERR, "Texture data creation failed. Texture data is NULL.");
}

rendering::Texture::Texture(int texturesCount, int width, int height, unsigned char** data, GLenum textureTarget, GLfloat* filters, GLenum* internalFormats, GLenum* formats, GLenum wrapping, GLenum* attachments) :
	m_textureData(textureTarget, width, height, texturesCount, data, filters, internalFormats, formats, wrapping, attachments)
#ifdef STORE_TEXTURE_FILE_NAME
	, m_fileName()
#endif
{
	CHECK_CONDITION_EXIT_RENDERING(m_textureData != nullptr, utility::logging::ERR, "Texture data creation failed. Texture data is NULL.");
}

rendering::Texture::Texture(const std::string& posXFileName, const std::string& negXFileName, const std::string& posYFileName, const std::string& negYFileName, const std::string& posZFileName, const std::string& negZFileName) :
	m_textureData(posXFileName, negXFileName, posYFileName, negYFileName, posZFileName, negZFileName)
#ifdef STORE_TEXTURE_FILE_NAME
	, m_fileName()
#endif
{
}

rendering::Texture::~Texture()
{
#ifdef STORE_TEXTURE_FILE_NAME
	DELOCUST_LOG_RENDERING("Texture \"", m_fileName, "\" destroyed.");
#else
	DELOCUST_LOG_RENDERING("Texture destroyed.");
#endif
}

rendering::Texture::Texture(Texture&& texture) noexcept :
	m_textureData(std::move(texture.m_textureData)) // http://stackoverflow.com/questions/29643974/using-stdmove-with-stdshared-ptr
#ifdef STORE_TEXTURE_FILE_NAME
	, m_fileName(std::move(texture.m_fileName))
#endif
{
#ifdef STORE_TEXTURE_FILE_NAME
	DELOCUST_LOG_RENDERING("Texture \"", m_fileName, "\" moved.");
#else
	DELOCUST_LOG_RENDERING("Texture moved.");
#endif
}

void rendering::Texture::Bind(unsigned int unit /* = 0 */, unsigned int textureIndex /* = 0 */) const
{
	CHECK_CONDITION_EXIT_RENDERING(m_textureData != nullptr, utility::logging::EMERGENCY, "Cannot bind the texture. Texture data is NULL.");
	CHECK_CONDITION_RENDERING((unit >= 0) && (unit < TextureData::MAX_BOUND_TEXTURES_COUNT), utility::logging::ERR, "Specified unit ", unit, " is outside of range [0; ", TextureData::MAX_BOUND_TEXTURES_COUNT, ")");
	
	glActiveTexture(GL_TEXTURE0 + unit);
	m_textureData.Bind(textureIndex);
}

void rendering::Texture::BindAsRenderTarget() const
{
	CHECK_CONDITION_EXIT_RENDERING(m_textureData != nullptr, utility::logging::EMERGENCY, "Cannot bind the texture as render target. Texture data is NULL.");
	m_textureData.BindAsRenderTarget();
}
/* ==================== Texture class implementation end ==================== */

//Rendering::CubeShadowMapTexture::CubeShadowMapTexture(int windowWidth, int windowHeight)
//{
//	GLenum attachments [2] = { GL_DEPTH_ATTACHMENT, GL_COLOR_ATTACHMENT0 };
//	m_textureData = new TextureData(GL_TEXTURE_CUBE_MAP, 512 /* width */, 512 /* height */, 2 /* textures count */, nullptr /* data */, nullptr /* filters */, nullptr /* internal format */, nullptr /* format */, true /* clamp enabled */, attachments);
//}
//
//Rendering::CubeShadowMapTexture::~CubeShadowMapTexture(void)
//{
//}