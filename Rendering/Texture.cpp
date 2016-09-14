#include "StdAfx.h"
#include "Texture.h"
//#include "CoreEngine.h"
#include "stb_image.h"
#include "Utility\Utility.h"
#include "Utility\ILogger.h"
#include "Math\Math.h"

/* static */ const int Rendering::TextureData::MAX_BOUND_TEXTURES_COUNT = 32;

//Rendering::TextureData::TextureData(const std::string& fileName) :
//	m_textureTarget()
//{
//}

Rendering::TextureData::TextureData(GLenum textureTarget, int width, int height, int texturesCount, unsigned char** data, GLfloat* filters, GLenum* internalFormats, GLenum* formats, GLenum wrapping, GLenum* attachments) :
	m_textureTarget(textureTarget),
	m_texturesCount(texturesCount),
	m_textureIDs(texturesCount),
	m_width(width),
	m_height(height),
	m_framebuffer(0),
	m_renderbuffer(0)
{
	CHECK_CONDITION_EXIT_RENDERING(m_texturesCount > 0, Utility::Logging::EMERGENCY, "Incorrect number of textures specified (", m_texturesCount, ").");
	CHECK_CONDITION_EXIT_RENDERING(m_texturesCount <= MAX_BOUND_TEXTURES_COUNT, Utility::Logging::ERR, "Maximum number of bound textures (", MAX_BOUND_TEXTURES_COUNT, ") exceeded. Buffer overrun might occur.");

	CheckErrorCode(__FUNCTION__, "Creating texture data");
	InitTextures(data, filters, internalFormats, formats, wrapping);
	if (attachments != NULL)
	{
		InitRenderTargets(attachments);
	}
}

Rendering::TextureData::TextureData(unsigned char** cubeMapTextureData, int width, int height, int depth) :
	m_textureTarget(GL_TEXTURE_CUBE_MAP),
	m_texturesCount(1),
	m_textureIDs(1),
	m_width(width),
	m_height(height),
	m_framebuffer(0),
	m_renderbuffer(0)
{
	// Init textures begin
	const int NUMBER_OF_CUBE_MAP_FACES = 6;
	for (int i = 0; i < NUMBER_OF_CUBE_MAP_FACES; ++i)
	{
		if (cubeMapTextureData[i] == NULL)
		{
			DEBUG_LOG_RENDERING("Cannot initialize texture. Passed cube map texture data is NULL (face ", i, ")");
			//return;
		}
	}
	
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
	for (int i = 0; i < NUMBER_OF_CUBE_MAP_FACES; ++i)
	{
		glTexImage2D(targets[i], 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, cubeMapTextureData[i]);
		stbi_image_free(cubeMapTextureData[i]);
	}
	glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(m_textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(m_textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(m_textureTarget, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// Init textures end

	InitRenderTargets(GL_NONE);
}

Rendering::TextureData::~TextureData(void)
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

Rendering::TextureData::TextureData(TextureData&& textureData) :
	m_textureTarget(std::move(textureData.m_textureTarget)),
	m_texturesCount(std::move(textureData.m_texturesCount)),
	m_textureIDs(std::move(textureData.m_textureIDs)),
	m_width(std::move(textureData.m_width)),
	m_height(std::move(textureData.m_height)),
	m_framebuffer(std::move(textureData.m_framebuffer)),
	m_renderbuffer(std::move(textureData.m_renderbuffer))
{
	CRITICAL_LOG_RENDERING("TextureData moved.");
	textureData.m_textureIDs.clear(); // TODO: Is it correct?
	textureData.m_framebuffer = 0;
	textureData.m_renderbuffer = 0;
}

void Rendering::TextureData::InitTextures(unsigned char** data, GLfloat* filters, GLenum* internalFormat, GLenum* format, GLenum wrapping)
{
	if (filters == NULL)
	{
		WARNING_LOG_RENDERING("The filter array is NULL.");
	}

	glGenTextures(m_texturesCount, &m_textureIDs[0]);
	CheckErrorCode(__FUNCTION__, "Generating textures");
	for (int i = 0; i < m_texturesCount; ++i)
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
		glTexImage2D(m_textureTarget, 0, internalFormat[i], m_width, m_height, 0, format[i], GL_UNSIGNED_BYTE, (data == NULL) ? NULL : data[i]);

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
			glTexParameterf(m_textureTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, Math::Clamp(0.0f, 8.0f, maxAnisotropy));
			//glTexParameterf(m_textureTarget, GL_TEXTURE_LOD_BIAS, -0.4f /* the lower the value the higher the resolution->lower performance */); //TODO: The value we give here is crucial for the performance. Reasearch that!
		}
		else
		{
			glTexParameteri(m_textureTarget, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(m_textureTarget, GL_TEXTURE_MAX_LEVEL, 0);
		}
	}
}

void Rendering::TextureData::Bind(int textureIndex) const
{
	CHECK_CONDITION_RETURN_VOID_RENDERING(textureIndex >= 0 && textureIndex < m_texturesCount, Utility::Logging::CRITICAL,
		"Cannot bind the texture with textureID=", textureIndex, ". This value is out of range [0; ", m_texturesCount, ")");
	glBindTexture(m_textureTarget, m_textureIDs[textureIndex]);
}

void Rendering::TextureData::InitRenderTargets(GLenum* attachments)
{
	CHECK_CONDITION_RETURN_VOID_ALWAYS_RENDERING(attachments != NULL, Utility::Logging::DEBUG, "Cannot initialize rendering targets for the texture. No attachments specified.");
	CHECK_CONDITION_EXIT_RENDERING(m_texturesCount <= MAX_BOUND_TEXTURES_COUNT, Utility::Logging::ERR, "Maximum number of bound textures (", MAX_BOUND_TEXTURES_COUNT, ") exceeded. Buffer overrun might occur.");

	std::vector<GLenum> drawBuffers(m_texturesCount);
	bool hasDepth = false;

	for (int i = 0; i < m_texturesCount; ++i)
	{
		DEBUG_LOG_RENDERING("The texture uses ", attachments[i], " as an attachment");
		if ( (attachments[i] == GL_DEPTH_ATTACHMENT) || (attachments[i] == GL_STENCIL_ATTACHMENT) )
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
		DEBUG_LOG_RENDERING("The texture does not have any depth attachment. Creating the render buffer is started.");
		glGenRenderbuffers(1, &m_renderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_renderbuffer);
	}
	glDrawBuffers(m_texturesCount, drawBuffers.data());

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	CHECK_CONDITION_EXIT_RENDERING(status == GL_FRAMEBUFFER_COMPLETE, Utility::Logging::CRITICAL, "Framebuffer creation failed. The framebuffer status is not GL_FRAMEBUFFER_COMPLETE. Instead it is ", status, ".");
}

void Rendering::TextureData::BindAsRenderTarget() const
{
	glBindTexture(GL_TEXTURE_2D, 0); // just to make sure the texture is not bound
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
	glViewport(0, 0, m_width, m_height);
}

// TODO: Don't use function pointers. Instead, create TextureComparator object and pass it instead as a function-object. Function-objects tend to inlined almost always, which isn't the case for function pointers.
bool Rendering::TextureData::Compare(const TextureData& textureData) const
{
	if ((m_textureTarget != textureData.m_textureTarget) || (m_width != textureData.m_width) || (m_height != textureData.m_height) ||
		(m_framebuffer != textureData.m_framebuffer) || (m_renderbuffer != textureData.m_renderbuffer) || (m_texturesCount != textureData.m_texturesCount))
	{
		return false;
	}

	for (int i = 0; i < m_texturesCount; ++i)
	{
		if (m_textureIDs[i] != textureData.m_textureIDs[i])
		{
			return false;
		}
	}
	return true;
}

/* static */ std::map<std::string, std::shared_ptr<Rendering::TextureData>> Rendering::Texture::s_textureResourceMap;

Rendering::Texture::Texture(const std::string& fileName, GLenum textureTarget /* = GL_TEXTURE_2D */, GLfloat filter /* = GL_LINEAR_MIPMAP_LINEAR */, GLenum internalFormat /* = GL_RGBA */,
	GLenum format /* = GL_RGBA */, GLenum wrapping /* = GL_REPEAT */, GLenum attachment /* = GL_NONE */) :
	m_textureData(nullptr),
	m_fileName(fileName)
{
	// TODO: Check whether the fileName is a full path or just a fileName. Act accordingly.
	std::string name = fileName;
	const char *tmp = strrchr(fileName.c_str(), '\\');
	if (tmp != NULL)
	{
		name.assign(tmp + 1);
	}

	std::map<std::string, std::shared_ptr<Rendering::TextureData>>::const_iterator itr = s_textureResourceMap.find(fileName);
	if (itr == s_textureResourceMap.end()) // texture has not been loaded yet
	{
		INFO_LOG_RENDERING("Loading texture from file \"", name, "\".");
		//std::string extension = name.substr(name.find_last_of(".") + 1);
		//DELOCUST_LOG_RENDERING("Extension is = \"", extension, "\"");

		int x, y, bytesPerPixel;
		//unsigned char* data = stbi_load((Core::CoreEngine::GetCoreEngine()->GetTexturesDirectory() + fileName).c_str(), &x, &y, &bytesPerPixel, 4 /* req_comp */);
		unsigned char* data = stbi_load(("C:\\Users\\aosesik\\Documents\\Visual Studio 2015\\Projects\\GameEngine\\Textures\\" + fileName).c_str(), &x, &y, &bytesPerPixel, 4 /* req_comp */);
		CHECK_CONDITION_EXIT_RENDERING(data != NULL, Utility::Logging::ERR, "Unable to load texture from the file \"", name, "\"");
		m_textureData = std::make_shared<TextureData>(textureTarget, x, y, 1, &data, &filter, &internalFormat, &format, wrapping, &attachment);
		stbi_image_free(data);
		s_textureResourceMap.insert(std::make_pair(fileName, m_textureData));
		DEBUG_LOG_RENDERING("Loading texture from file \"", name, "\" finished successfully");
	}
	else // (itr != textureResourceMap.end()) // texture has already been loaded
	{
		m_textureData = itr->second;
	}

	//LoadFromFile(fileName);
}

Rendering::Texture::Texture(int width /* = 0 */, int height /* = 0 */, unsigned char* data /* = 0 */, GLenum textureTarget /* = GL_TEXTURE_2D */, GLfloat filter /* = GL_LINEAR_MIPMAP_LINEAR */,
	GLenum internalFormat /* = GL_RGBA */, GLenum format /* = GL_RGBA */, GLenum wrapping /* = GL_REPEAT */, GLenum attachment /* = GL_NONE */) :
	m_textureData(nullptr),
	m_fileName()
{
	CHECK_CONDITION_RENDERING((width > 0) && (height > 0), Utility::Logging::ERR, "Cannot initialize texture. Passed texture size is incorrect (width=", width, "; height=", height, ")");
	m_textureData = std::make_shared<TextureData>(textureTarget, width, height, 1, &data, &filter, &internalFormat, &format, wrapping, &attachment);
	CHECK_CONDITION_EXIT_RENDERING(m_textureData != nullptr, Utility::Logging::ERR, "Texture data creation failed. Texture data is NULL.");
}

Rendering::Texture::Texture(int texturesCount, int width, int height, unsigned char** data, GLenum textureTarget, GLfloat* filters, GLenum* internalFormats, GLenum* formats, GLenum wrapping, GLenum* attachments) :
	m_textureData(nullptr),
	m_fileName()
{
	//m_textureData = new TextureData(textureTarget, width, height, 1, &data, &filter, &internalFormat, &format, clampEnabled, &attachment);
	m_textureData = std::make_shared<TextureData>(textureTarget, width, height, texturesCount, data, filters, internalFormats, formats, wrapping, attachments);
	CHECK_CONDITION_EXIT_RENDERING(m_textureData != nullptr, Utility::Logging::ERR, "Texture data creation failed. Texture data is NULL.");
}

Rendering::Texture::Texture(const std::string& posXFileName, const std::string& negXFileName, const std::string& posYFileName, const std::string& negYFileName, const std::string& posZFileName, const std::string& negZFileName)
{
	const int NUMBER_OF_CUBE_MAP_FACES = 6;

	unsigned char* cubeMapData[NUMBER_OF_CUBE_MAP_FACES];
	int x[NUMBER_OF_CUBE_MAP_FACES], y[NUMBER_OF_CUBE_MAP_FACES], bytesPerPixel[NUMBER_OF_CUBE_MAP_FACES];

	const std::string filenames[NUMBER_OF_CUBE_MAP_FACES] = { posXFileName, negXFileName, posYFileName, negYFileName, posZFileName, negZFileName };
	for (int i = 0; i < NUMBER_OF_CUBE_MAP_FACES; ++i)
	{
		//cubeMapData[i] = stbi_load((Core::CoreEngine::GetCoreEngine()->GetTexturesDirectory() + filenames[i]).c_str(), &x[i], &y[i], &bytesPerPixel[i], 4 /* req_comp */);
		cubeMapData[i] = stbi_load(("C:\\Users\\aosesik\\Documents\\Visual Studio 2015\\Projects\\GameEngine\\Textures\\" + filenames[i]).c_str(), &x[i], &y[i], &bytesPerPixel[i], 4 /* req_comp */);
		if (cubeMapData[i] == NULL)
		{
			std::string name = filenames[i];
			const char *tmp = strrchr(filenames[i].c_str(), '\\');
			if (tmp != NULL)
			{
				name.assign(tmp + 1);
			}
			ERROR_LOG_RENDERING("Unable to load texture from the file \"", name, "\"");
			exit(EXIT_FAILURE);
		}
		if (i > 0)
		{
			if (x[i] != x[i - 1])
			{
				ERROR_LOG_RENDERING("All cube map texture's faces must have the same width, but face ", i, " has width=", x[i], " and face ", i + 1, " has width=", x[i + 1]);
			}
			if (y[i] != y[i - 1])
			{
				ERROR_LOG_RENDERING("All cube map texture's faces must have the same height, but face ", i, " has height=", y[i], " and face ", i + 1, " has height=", y[i + 1]);
			}
		}
	}

	// TODO: Pass correct values for width, height and depth. The values below will only work for square textures with each face having the same size.
	int width = x[0];
	int height = x[0];
	int depth = x[0];
	m_textureData = std::make_shared<TextureData>(cubeMapData, width, height, depth);
}

Rendering::Texture::~Texture(void)
{
	DELOCUST_LOG_RENDERING("Texture \"", m_fileName, "\" destroyed.");
}

Rendering::Texture::Texture(Texture&& texture) :
	m_textureData(std::move(texture.m_textureData)), // http://stackoverflow.com/questions/29643974/using-stdmove-with-stdshared-ptr
	m_fileName(std::move(texture.m_fileName))
{
	DELOCUST_LOG_RENDERING("Texture \"", m_fileName, "\" moved.");
}

void Rendering::Texture::Bind(unsigned int unit /* = 0 */, unsigned int textureIndex /* = 0 */) const
{
	CHECK_CONDITION_EXIT_RENDERING(m_textureData != NULL, Utility::Logging::EMERGENCY, "Cannot bind the texture. Texture data is NULL.");
	CHECK_CONDITION_RENDERING((unit >= 0) && (unit < TextureData::MAX_BOUND_TEXTURES_COUNT), Utility::Logging::ERR, "Specified unit ", unit, " is outside of range [0; ", TextureData::MAX_BOUND_TEXTURES_COUNT, ")");
	
	glActiveTexture(GL_TEXTURE0 + unit);
	m_textureData->Bind(textureIndex);
}

void Rendering::Texture::BindAsRenderTarget() const
{
	CHECK_CONDITION_EXIT_RENDERING(m_textureData != NULL, Utility::Logging::EMERGENCY, "Cannot bind the texture as render target. Texture data is NULL.");
	m_textureData->BindAsRenderTarget();
}


Rendering::GuiTexture::GuiTexture(const std::string& fileName, const Math::Vector2D& position, const Math::Vector2D& scale, GLenum textureTarget /* = GL_TEXTURE_2D */,
	GLfloat filter /* = GL_LINEAR_MIPMAP_LINEAR */, GLenum internalFormat /* = GL_RGBA */, GLenum format /* = GL_RGBA */, GLenum wrapping /* = GL_REPEAT */, GLenum attachment /* = GL_NONE */) :
	Texture(fileName, textureTarget, filter, internalFormat, format, wrapping, attachment),
	m_transformationMatrix(position, scale),
	m_position(position),
	m_scale(scale)
{
	DELOCUST_LOG_RENDERING("GUI texture \"", fileName, "\" created.");
}

Rendering::GuiTexture::~GuiTexture(void)
{
	DELOCUST_LOG_RENDERING("GUI texture destroyed.");
}

Rendering::GuiTexture::GuiTexture(GuiTexture&& guiTexture) :
	Texture(std::move(guiTexture)),
	m_transformationMatrix(std::move(guiTexture.m_transformationMatrix)),
	m_position(std::move(guiTexture.m_position)),
	m_scale(std::move(guiTexture.m_scale))
{
	DELOCUST_LOG_RENDERING("GUI texture moved.");
}

Rendering::Particles::ParticleTexture::ParticleTexture(const std::string& fileName, int rowsCount, bool isAdditive) :
	Texture(fileName, GL_TEXTURE_2D, GL_NEAREST, GL_RGBA, GL_RGBA, GL_REPEAT, GL_NONE),
	m_rowsCount(rowsCount),
	m_isAdditive(isAdditive)
{
}

Rendering::Particles::ParticleTexture::~ParticleTexture(void)
{
}

Rendering::Particles::ParticleTexture::ParticleTexture(ParticleTexture&& particleTexture) :
	Texture(std::move(particleTexture)),
	m_rowsCount(std::move(particleTexture.m_rowsCount)),
	m_isAdditive(std::move(particleTexture.m_isAdditive))
{
	DELOCUST_LOG_RENDERING("Particle texture moved.");
}

//Rendering::CubeShadowMapTexture::CubeShadowMapTexture(int windowWidth, int windowHeight)
//{
//	GLenum attachments [2] = { GL_DEPTH_ATTACHMENT, GL_COLOR_ATTACHMENT0 };
//	m_textureData = new TextureData(GL_TEXTURE_CUBE_MAP, 512 /* width */, 512 /* height */, 2 /* textures count */, NULL /* data */, NULL /* filters */, NULL /* internal format */, NULL /* format */, true /* clamp enabled */, attachments);
//}
//
//Rendering::CubeShadowMapTexture::~CubeShadowMapTexture(void)
//{
//}
