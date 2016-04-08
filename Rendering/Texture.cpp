#include "StdAfx.h"
#include "Texture.h"
//#include "CoreEngine.h"
#include "stb_image.h"
#include "Utility\Utility.h"
#include "Utility\ILogger.h"
#include "Math\Math.h"

using namespace Rendering;

/* static */ const int TextureData::MAX_BOUND_TEXTURES_COUNT = 32;

TextureData::TextureData(GLenum textureTarget, int width, int height, int texturesCount, unsigned char** data, GLfloat* filters, GLenum* internalFormat, GLenum* format, bool clampEnabled, GLenum* attachments) :
	m_textureTarget(textureTarget),
	m_texturesCount(texturesCount),
	m_width(width),
	m_height(height),
	m_framebuffer(0),
	m_renderbuffer(0)
{
	CHECK_CONDITION_EXIT(m_texturesCount > 0, Utility::Emergency, "Incorrect number of textures specified (%d).", m_texturesCount);
	CHECK_CONDITION_EXIT(m_texturesCount <= MAX_BOUND_TEXTURES_COUNT, Utility::Error, "Maximum number of bound textures exceeded. Buffer overrun might occur.");
	m_textureID = new GLuint[m_texturesCount];

	CheckErrorCode(__FUNCTION__, "Creating texture data");
	InitTextures(data, filters, internalFormat, format, clampEnabled);
	InitRenderTargets(attachments);
}

TextureData::TextureData(unsigned char** cubeMapTextureData, int width, int height, int depth) :
	m_textureTarget(GL_TEXTURE_CUBE_MAP),
	m_texturesCount(1),
	m_width(width),
	m_height(height),
	m_framebuffer(0),
	m_renderbuffer(0)
{
	m_textureID = new GLuint[m_texturesCount];

	// Init textures begin
	const int NUMBER_OF_CUBE_MAP_FACES = 6;
	for (int i = 0; i < NUMBER_OF_CUBE_MAP_FACES; ++i)
	{
		if (cubeMapTextureData[i] == NULL)
		{
			DEBUG_LOG("Cannot initialize texture. Passed cube map texture data is NULL (face %d)", i);
			//return;
		}
	}
	
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(m_texturesCount, m_textureID);
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

TextureData::~TextureData(void)
{
	if (*m_textureID)
	{
		glDeleteTextures(m_texturesCount, m_textureID);
	}
	SAFE_DELETE_JUST_TABLE(m_textureID);
	if (m_framebuffer)
	{
		glDeleteFramebuffers(1, &m_framebuffer);
	}
	if (m_renderbuffer)
	{
		glDeleteRenderbuffers(1, &m_renderbuffer);
	}
}

void TextureData::InitTextures(unsigned char** data, GLfloat* filters, GLenum* internalFormat, GLenum* format, bool clampEnabled)
{
	if (data == NULL)
	{
		DEBUG_LOG("No data passed to the Texture object.");
		//return;
	}
	if (filters == NULL)
	{
		WARNING_LOG("The filter array is NULL.");
	}

	glGenTextures(m_texturesCount, m_textureID);
	CheckErrorCode(__FUNCTION__, "Generating textures");
	for (int i = 0; i < m_texturesCount; ++i)
	{
		//ASSERT(data[i] != NULL)
		if (data[i] == NULL)
		{
			DEBUG_LOG("Texture data[%d] is NULL.", i);
		}
		glBindTexture(m_textureTarget, m_textureID[i]);
		glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, filters[i]);
		CheckErrorCode(__FUNCTION__, "Setting the GL_TEXTURE_MIN_FILTER");
		if (filters[i] == GL_NEAREST)
		{
			/**
			 * GL_TEXTURE_MAG_FILTER only accepts two possible values: GL_NEAREST and GL_LINEAR (default).
			 * We cannot use GL_*_MIPMAP_* values as they give errors GL_INVALID_ENUM.
			 * See link https://www.opengl.org/wiki/GLAPI/glTexParameter (search for GL_TEXTURE_MAG_FILTER)
			 */
			glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, filters[i] /* GL_NEAREST */);
			CheckErrorCode(__FUNCTION__, "Setting the GL_TEXTURE_MAG_FILTER");
		}

		if (clampEnabled)
		{
			glTexParameterf(m_textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(m_textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			//glTexParameterf(textureTarget, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); // TODO: Check if this parameter is ok for all texture targets. It is ok though for GL_TEXTURE_CUBE_MAP texture target. What about GL_TEXTURE_2D?
		}
		else
		{
			//glTexParameterf(m_textureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
			//glTexParameterf(m_textureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
			//glTexParameterf(textureTarget, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); // TODO: Check if this parameter is ok for all texture targets. It is ok though for GL_TEXTURE_CUBE_MAP texture target. What about GL_TEXTURE_2D?
		}

		// If we want to use mipmapping we must submit the data to the texture before.
		glTexImage2D(m_textureTarget, 0, internalFormat[i], m_width, m_height, 0, format[i], GL_UNSIGNED_BYTE, data[i]);

		if(filters[i] == GL_NEAREST_MIPMAP_NEAREST ||
			filters[i] == GL_NEAREST_MIPMAP_LINEAR ||
			filters[i] == GL_LINEAR_MIPMAP_NEAREST ||
			filters[i] == GL_LINEAR_MIPMAP_LINEAR) // do we use mipmapping?
		{
			glGenerateMipmap(m_textureTarget);
			CheckErrorCode(__FUNCTION__, "Generating mipmaps");
			// Watch https://www.youtube.com/watch?v=psD5Eoabbng&index=19&list=PLEETnX-uPtBVG1ao7GCESh2vOayJXDbAl (starts around 8:30) to know more about anisotropic filtering.
			// Basically, anisotropic filtering is for making the mipmapping work a little bit better when viewing the textured model from a very shallow angle.
			//GLfloat maxAnisotropy; // the maximum number of samples per pixel used for mipmapping filtering
			//glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
			//DEBUG_LOG("Maximum anisotropy supported = %.5f", maxAnisotropy);
			//glTexParameterf(m_textureTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, Math::Clamp(0.0f, 8.0f, maxAnisotropy));
			glTexParameterf(m_textureTarget, GL_TEXTURE_LOD_BIAS, -0.4f /* the lower the value the higher the resolution->lower performance */); //TODO: The value we give here is crucial for the performance. Reasearch that!
		}
		else
		{
			glTexParameteri(m_textureTarget, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(m_textureTarget, GL_TEXTURE_MAX_LEVEL, 0);
		}
	}
}

void TextureData::Bind(int textureIndex) const
{
	CHECK_CONDITION_RETURN_VOID(textureIndex >= 0 && textureIndex < m_texturesCount, Utility::Critical, "Cannot bind the texture with textureID=%d. This value is out of range [%d; %d)", textureIndex, 0, m_texturesCount);
	glBindTexture(m_textureTarget, m_textureID[textureIndex]);
}

void TextureData::InitRenderTargets(GLenum* attachments)
{
	if (attachments == NULL)
	{
		DELOCUST_LOG("No attachments used");
		return;
	}
	CHECK_CONDITION_EXIT(m_texturesCount <= MAX_BOUND_TEXTURES_COUNT, Utility::Error, "Maximum number of bound textures exceeded. Buffer overrun might occur.");

	GLenum* drawBuffers = new GLenum[m_texturesCount];
	bool hasDepth = false;

	for (int i = 0; i < m_texturesCount; ++i)
	{
		DEBUG_LOG("The texture uses 0x%x as an attachment", attachments[i]);
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
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachments[i], m_textureTarget, m_textureID[i], 0 /* which mipmap level of the texture we want to use */);
	}

	if (m_framebuffer == 0)
	{
		DEBUG_LOG("Framebuffer will not be used by the texture (textures count = %d)", m_texturesCount);
		return;
	}
	if (!hasDepth)
	{
		DEBUG_LOG("The texture does not have any depth attachment. Creating the render buffer is started.");
		glGenRenderbuffers(1, &m_renderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_renderbuffer);
	}
	glDrawBuffers(m_texturesCount, drawBuffers);
	//SAFE_DELETE_JUST_TABLE(drawBuffers);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	CHECK_CONDITION_EXIT(status == GL_FRAMEBUFFER_COMPLETE, Utility::Critical, "Framebuffer creation failed. The framebuffer status is not GL_FRAMEBUFFER_COMPLETE. Instead it is 0x%x.", status);
}

void TextureData::BindAsRenderTarget() const
{
	glBindTexture(GL_TEXTURE_2D, 0); // just to make sure the texture is not bound
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
	glViewport(0, 0, m_width, m_height);
}

bool TextureData::Compare(const TextureData& textureData) const
{
	if ((m_textureTarget != textureData.m_textureTarget) || (m_width != textureData.m_width) || (m_height != textureData.m_height) ||
		(m_framebuffer != textureData.m_framebuffer) || (m_renderbuffer != textureData.m_renderbuffer) || (m_texturesCount != textureData.m_texturesCount))
	{
		return false;
	}

	for (int i = 0; i < m_texturesCount; ++i)
	{
		if (m_textureID[i] != textureData.m_textureID[i])
		{
			return false;
		}
	}
	return true;
}

/* static */ std::map<std::string, TextureData*> Texture::s_textureResourceMap;

Texture::Texture(const std::string& fileName, GLenum textureTarget /* = GL_TEXTURE_2D */, GLfloat filter /* = GL_LINEAR_MIPMAP_LINEAR */, GLenum internalFormat /*=GL_RGBA*/, GLenum format /*=GL_RGBA*/, bool clampEnabled /*=false*/, GLenum attachment /*= GL_NONE*/) :
	m_textureData(NULL),
	m_fileName(fileName)
{
	// TODO: Check whether the fileName is a full path or just a fileName. Act accordingly.
	std::string name = fileName;
	const char *tmp = strrchr(fileName.c_str(), '\\');
	if (tmp != NULL)
	{
		name.assign(tmp + 1);
	}

	std::map<std::string, TextureData*>::const_iterator itr = s_textureResourceMap.find(fileName);
	if (itr == s_textureResourceMap.end()) // texture has not been loaded yet
	{
		INFO_LOG("Loading texture from file \"%s\"", name.c_str());
		//std::string extension = name.substr(name.find_last_of(".") + 1);
		//DELOCUST_LOG("Extension is = \"%s\"", extension.c_str());

		int x, y, bytesPerPixel;
		//unsigned char* data = stbi_load((Core::CoreEngine::GetCoreEngine()->GetTexturesDirectory() + fileName).c_str(), &x, &y, &bytesPerPixel, 4 /* req_comp */);
		unsigned char* data = stbi_load(("C:\\Users\\aosesik\\Documents\\Visual Studio 2015\\Projects\\GameEngine\\Textures\\" + fileName).c_str(), &x, &y, &bytesPerPixel, 4 /* req_comp */);

		CHECK_CONDITION_EXIT(data != NULL, Utility::Error, "Unable to load texture from the file \"%s\"", name.c_str());
		m_textureData = new TextureData(textureTarget, x, y, 1, &data, &filter, &internalFormat, &format, clampEnabled, &attachment);
		stbi_image_free(data);
		s_textureResourceMap.insert(std::pair<std::string, TextureData*>(fileName, m_textureData));
		DEBUG_LOG("Loading texture from file \"%s\" finished successfully", name.c_str());
	}
	else // (itr != textureResourceMap.end()) // texture has already been loaded
	{
		m_textureData = itr->second;
		m_textureData->AddReference();
	}

	//LoadFromFile(fileName);
}

Texture::Texture(const std::string& posXFileName, const std::string& negXFileName, const std::string& posYFileName, const std::string& negYFileName, const std::string& posZFileName, const std::string& negZFileName)
{
	const int NUMBER_OF_CUBE_MAP_FACES = 6;

	unsigned char* cubeMapData [NUMBER_OF_CUBE_MAP_FACES];
	int x[NUMBER_OF_CUBE_MAP_FACES], y[NUMBER_OF_CUBE_MAP_FACES], bytesPerPixel[NUMBER_OF_CUBE_MAP_FACES];

	const std::string filenames [NUMBER_OF_CUBE_MAP_FACES] = { posXFileName, negXFileName, posYFileName, negYFileName, posZFileName, negZFileName };
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
			ERROR_LOG("Unable to load texture from the file \"%s\"", name.c_str());
			exit(EXIT_FAILURE);
		}
		if (i > 0)
		{
			if (x[i] != x[i-1])
			{
				ERROR_LOG("All cube map texture's faces must have the same width, but face %d has width=%d and face %d has width=%d", i, x[i], i+1, x[i+1]);
			}
			if (y[i] != y[i-1])
			{
				ERROR_LOG("All cube map texture's faces must have the same height, but face %d has height=%d and face %d has height=%d", i, x[i], i+1, x[i+1]);
			}
		}
	}

	// TODO: Pass correct values for width, height and depth. The values below will only work for square textures with each face having the same size.
	int width = x[0];
	int height = x[0];
	int depth = x[0];
	m_textureData = new TextureData(cubeMapData, width, height, depth);
}

Texture::Texture(int width /* = 0 */, int height /* = 0 */, unsigned char* data /* = 0 */, GLenum textureTarget /* = GL_TEXTURE_2D */, GLfloat filter /* = GL_LINEAR_MIPMAP_LINEAR */, GLenum internalFormat /* = GL_RGBA */, GLenum format /* = GL_RGBA */, bool clampEnabled /* = false */, GLenum attachment /* = GL_NONE */) :
	m_textureData(NULL),
	m_fileName()
{
	CHECK_CONDITION((width > 0) && (height > 0), Utility::Error, "Cannot initialize texture. Passed texture size is incorrect (width=%d; height=%d)", width, height);
	if (data == NULL)
	{
		DEBUG_LOG("Cannot initialize texture. Passed texture data is NULL");
	}
	m_textureData = new TextureData(textureTarget, width, height, 1, &data, &filter, &internalFormat, &format, clampEnabled, &attachment);
	CHECK_CONDITION_EXIT(m_textureData != NULL, Utility::Error, "Texture data creation failed. Texture data is NULL.");
}

Texture::Texture(int texturesCount, int width, int height, unsigned char** data, GLenum textureTarget, GLfloat* filters, GLenum* internalFormats, GLenum* formats, bool clampEnabled, GLenum* attachments) :
	m_textureData(NULL),
	m_fileName()
{
	//m_textureData = new TextureData(textureTarget, width, height, 1, &data, &filter, &internalFormat, &format, clampEnabled, &attachment);
	CHECK_CONDITION_EXIT(m_textureData != NULL, Utility::Error, "Texture data creation failed. Texture data is NULL.");
	m_textureData = new TextureData(textureTarget, width, height, texturesCount, data, filters, internalFormats, formats, clampEnabled, attachments);

}

Texture::~Texture(void)
{
	CHECK_CONDITION_RETURN_VOID_ALWAYS(m_textureData != NULL, Utility::Error, "Error encountered while destructing the texture. Texture data is already NULL.");
	
	m_textureData->RemoveReference();
	if (! m_textureData->IsReferenced())
	{
		if (m_fileName.length() > 0)
		{
			s_textureResourceMap.erase(m_fileName);
		}
		SAFE_DELETE(m_textureData);
	}
}

void Texture::Bind(unsigned int unit /* = 0 */, unsigned int textureIndex /* = 0 */) const
{
	CHECK_CONDITION_EXIT(m_textureData != NULL, Utility::Emergency, "Cannot bind the texture. Texture data is NULL.");
	CHECK_CONDITION((unit >= 0) && (unit < TextureData::MAX_BOUND_TEXTURES_COUNT), Utility::Error, "Specified unit %d is outside of range [0; %d)", unit, TextureData::MAX_BOUND_TEXTURES_COUNT);
	
	glActiveTexture(GL_TEXTURE0 + unit);
	m_textureData->Bind(textureIndex);
}

void Texture::BindAsRenderTarget() const
{
	CHECK_CONDITION_EXIT(m_textureData != NULL, Utility::Emergency, "Cannot bind the texture as render target. Texture data is NULL.");
	m_textureData->BindAsRenderTarget();
}


GuiTexture::GuiTexture(const std::string& fileName, const Math::Vector2D& position, const Math::Vector2D scale, GLenum textureTarget /* = GL_TEXTURE_2D */, GLfloat filter /* = GL_LINEAR_MIPMAP_LINEAR */, GLenum internalFormat /* = GL_RGBA */, GLenum format /* = GL_RGBA */, bool clampEnabled /* = false */, GLenum attachment /* = GL_NONE */) :
	Texture(fileName, textureTarget, filter, internalFormat, format, clampEnabled, attachment),
	m_transformationMatrix(position, scale),
	m_position(position),
	m_scale(scale)
{
}

GuiTexture::~GuiTexture(void)
{
}

ParticleTexture::ParticleTexture(const std::string& fileName, int rowsCount) :
	Texture(fileName, GL_TEXTURE_2D, GL_NEAREST, GL_RGBA, GL_RGBA, false, GL_NONE),
	m_rowsCount(rowsCount)
{
}

ParticleTexture::~ParticleTexture(void)
{
}

//CubeShadowMapTexture::CubeShadowMapTexture(int windowWidth, int windowHeight)
//{
//	GLenum attachments [2] = { GL_DEPTH_ATTACHMENT, GL_COLOR_ATTACHMENT0 };
//	m_textureData = new TextureData(GL_TEXTURE_CUBE_MAP, 512 /* width */, 512 /* height */, 2 /* textures count */, NULL /* data */, NULL /* filters */, NULL /* internal format */, NULL /* format */, true /* clamp enabled */, attachments);
//}
//
//CubeShadowMapTexture::~CubeShadowMapTexture(void)
//{
//}
