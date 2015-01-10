#include "StdAfx.h"
#include "Texture.h"
#include "stb_image.h"
#include "Utility\Utility.h"
#include "Utility\ILogger.h"
#include "Math\Math.h"

using namespace Rendering;

TextureData::TextureData(GLenum textureTarget, int width, int height, int texturesCount /* = 1 */, unsigned char** data, GLfloat* filters, GLenum* internalFormat, GLenum* format, bool clampEnabled, GLenum* attachments) :
	textureTarget(textureTarget),
	texturesCount(texturesCount),
	framebuffer(0),
	renderbuffer(0)
{
	ASSERT(texturesCount > 0);
	if (texturesCount < 1)
	{
		LOG(Utility::Emergency, LOGPLACE, "Incorrect number of textures specified");
		exit(EXIT_FAILURE);
	}
	if (texturesCount > MAX_BOUND_TEXTURES_COUNT) //Assert to be sure no buffer overrun should occur
	{
		LOG(Utility::Error, LOGPLACE, "Maximum number of bound textures exceeded. Buffer overrun might occur.");
		exit(EXIT_FAILURE);
	}
	textureID = new GLuint[texturesCount];
	this->width = width;
	this->height = height;

	InitTextures(data, filters, internalFormat, format, clampEnabled);
	InitRenderTargets(attachments);
}

TextureData::TextureData(unsigned char** cubeMapTextureData, int width, int height, int depth) :
	textureTarget(GL_TEXTURE_CUBE_MAP),
	texturesCount(1),
	width(width),
	height(height),
	framebuffer(0),
	renderbuffer(0)
{
	textureID = new GLuint[texturesCount];

	// Init textures begin
	const int NUMBER_OF_CUBE_MAP_FACES = 6;
	for (int i = 0; i < NUMBER_OF_CUBE_MAP_FACES; ++i)
	{
		if (cubeMapTextureData[i] == NULL)
		{
			LOG(Utility::Debug, LOGPLACE, "Cannot initialize texture. Passed cube map texture data is NULL (face %d)", i);
			//return;
		}
	}
	
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(texturesCount, textureID);
	CheckErrorCode(__FUNCTION__, "Generating textures");
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
	glTexParameterf(textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(textureTarget, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// Init textures end

	InitRenderTargets(GL_NONE);
}

TextureData::~TextureData(void)
{
	if (*textureID)
	{
		glDeleteTextures(texturesCount, textureID);
	}
	if (textureID != NULL)
	{
		delete [] textureID;
		textureID = NULL;
	}
	if (framebuffer)
	{
		glDeleteFramebuffers(1, &framebuffer);
	}
	if (renderbuffer)
	{
		glDeleteRenderbuffers(1, &renderbuffer);
	}
}

void TextureData::InitTextures(unsigned char** data, GLfloat* filters, GLenum* internalFormat, GLenum* format, bool clampEnabled)
{
	//ASSERT(data.size == texturesCount);
	//ASSERT(filter.size == texturesCount);
	//ASSERT(internalFormat.size == texturesCount);
	//ASSERT(format.size == texturesCount);
	
	//ASSERT(data != NULL);
	//ASSERT(filter != NULL);
	if (data == NULL)
	{
		LOG(Utility::Debug, LOGPLACE, "Cannot initialize texture. Passed texture data is NULL");
		//return;
	}
	if (filters == NULL)
	{
		LOG(Utility::Warning, LOGPLACE, "The filter array is NULL.");
	}
	
	glGenTextures(texturesCount, textureID);
	CheckErrorCode(__FUNCTION__, "Generating textures");
	for (int i = 0; i < texturesCount; ++i)
	{
		//ASSERT(data[i] != NULL)
		if (data[i] == NULL)
		{
			LOG(Utility::Debug, LOGPLACE, "Texture data[%d] is NULL.", i);
		}
		glBindTexture(textureTarget, textureID[i]);
		glTexParameterf(textureTarget, GL_TEXTURE_MIN_FILTER, filters[i]);
		CheckErrorCode(__FUNCTION__, "Setting the GL_TEXTURE_MIN_FILTER");
		if (filters[i] == GL_NEAREST)
		{
			/**
			 * GL_TEXTURE_MAG_FILTER only accepts two possible values: GL_NEAREST and GL_LINEAR (default).
			 * We cannot use GL_*_MIPMAP_* values as they give errors GL_INVALID_ENUM.
			 * See link https://www.opengl.org/wiki/GLAPI/glTexParameter (search for GL_TEXTURE_MAG_FILTER)
			 */
			glTexParameterf(textureTarget, GL_TEXTURE_MAG_FILTER, filters[i] /* GL_NEAREST */);
			CheckErrorCode(__FUNCTION__, "Setting the GL_TEXTURE_MAG_FILTER");
		}

		if (clampEnabled)
		{
			glTexParameterf(textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			//glTexParameterf(textureTarget, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); // TODO: Check if this parameter is ok for all texture targets. It is ok though for GL_TEXTURE_CUBE_MAP texture target. What about GL_TEXTURE_2D?
		}

		glTexImage2D(textureTarget, 0, internalFormat[i], width, height, 0, format[i], GL_UNSIGNED_BYTE, data[i]);

		if(filters[i] == GL_NEAREST_MIPMAP_NEAREST ||
			filters[i] == GL_NEAREST_MIPMAP_LINEAR ||
			filters[i] == GL_LINEAR_MIPMAP_NEAREST ||
			filters[i] == GL_LINEAR_MIPMAP_LINEAR) // do we use mipmapping?
		{
			glGenerateMipmap(textureTarget);
			CheckErrorCode(__FUNCTION__, "Generating mipmaps");
			GLfloat maxAnisotropy; // the maximum number of samples per pixel used for mipmapping filtering
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
			glTexParameterf(textureTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, Math::Clamp(0.0f, 8.0f, maxAnisotropy));
		}
		else
		{
			glTexParameteri(textureTarget, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(textureTarget, GL_TEXTURE_MAX_LEVEL, 0);
		}
	}
}

void TextureData::Bind(int textureIndex) const
{
	ASSERT(textureIndex >= 0);
	ASSERT(textureIndex < texturesCount);
	if ((textureIndex < 0) || (textureIndex >= texturesCount))
	{
		LOG(Utility::Error, LOGPLACE,
			"Cannot bind the texture with textureID=%d. This value is out of range [%d; %d)", textureIndex, 0, texturesCount);
		return;
	}
	glBindTexture(textureTarget, textureID[textureIndex]);
}

void TextureData::InitRenderTargets(GLenum* attachments)
{
	if (attachments == NULL)
	{
		LOG(Utility::Debug, LOGPLACE, "No attachments used");
		return;
	}
	if (texturesCount > MAX_BOUND_TEXTURES_COUNT) //Assert to be sure no buffer overrun should occur
	{
		LOG(Utility::Error, LOGPLACE, "Maximum number of bound textures exceeded. Buffer overrun might occur.");
		exit(EXIT_FAILURE);
	}

	GLenum* drawBuffers= new GLenum[texturesCount];
	bool hasDepth = false;

	for (int i = 0; i < texturesCount; ++i)
	{
		if ( (attachments[i] == GL_DEPTH_ATTACHMENT) || (attachments[i] == GL_STENCIL_ATTACHMENT) )
		{
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
		if (framebuffer == 0)
		{
			glGenFramebuffers(1, &framebuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		}
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachments[i], textureTarget, textureID[0], 0); // associate framebuffer with texture
	}

	if (framebuffer == 0)
	{
		LOG(Utility::Debug, LOGPLACE, "Framebuffer is 0");
		return;
	}
	if (!hasDepth)
	{
		glGenRenderbuffers(1, &renderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);
	}
	glDrawBuffers(texturesCount, drawBuffers);
	//delete [] drawBuffers;
	//drawBuffers = NULL;

	ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG(Utility::Critical, LOGPLACE, "Framebuffer creation failed. The framebuffer status is not GL_FRAMEBUFFER_COMPLETE.");
		exit(EXIT_FAILURE);
	}

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void TextureData::BindAsRenderTarget() const
{
	//glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glViewport(0, 0, width, height);
}

/* static */ std::map<std::string, TextureData*> Texture::textureResourceMap;

Texture::Texture(const std::string& fileName, GLenum textureTarget /* = GL_TEXTURE_2D */, GLfloat filter /* = GL_LINEAR_MIPMAP_LINEAR */, GLenum internalFormat /*=GL_RGBA*/, GLenum format /*=GL_RGBA*/, bool clampEnabled /*=false*/, GLenum attachment /*= GL_NONE*/) :
	textureData(NULL),
	fileName(fileName)
{
	// TODO: Check whether the fileName is a full path or just a fileName. Act accordingly.
	std::string name = fileName;
	const char *tmp = strrchr(fileName.c_str(), '\\');
	if (tmp != NULL)
	{
		name.assign(tmp + 1);
	}

	std::map<std::string, TextureData*>::const_iterator itr = textureResourceMap.find(fileName);
	if (itr == textureResourceMap.end()) // texture has not been loaded yet
	{
		LOG(Utility::Info, LOGPLACE, "Loading texture from file \"%s\"", name.c_str());
		//std::string extension = name.substr(name.find_last_of(".") + 1);
		//LOG(Utility::Delocust, LOGPLACE, "Extension is = \"%s\"", extension.c_str());

		int x, y, bytesPerPixel;
		unsigned char* data = stbi_load(fileName.c_str(), &x, &y, &bytesPerPixel, 4 /* req_comp */);

		if (data == NULL)
		{
			LOG(Utility::Error, LOGPLACE, "Unable to load texture from the file \"%s\"", name.c_str());
			return;
		}
		textureData = new TextureData(textureTarget, x, y, 1, &data, &filter, &internalFormat, &format, clampEnabled, &attachment);
		stbi_image_free(data);
		ASSERT(textureData);
		textureResourceMap.insert(std::pair<std::string, TextureData*>(fileName, textureData));
		LOG(Utility::Debug, LOGPLACE, "Loading texture from file \"%s\" finished successfully", name.c_str());
	}
	else // (itr != textureResourceMap.end()) // texture has already been loaded
	{
		textureData = itr->second;
		textureData->AddReference();
	}

	//LoadFromFile(fileName);
}

Texture::Texture(const std::string& posXFileName, const std::string& negXFileName, const std::string& posYFileName, const std::string& negYFileName, const std::string& posZFileName, const std::string& negZFileName)
{
	const int NUMBER_OF_CUBE_MAP_FACES = 6;

	unsigned char* cubeMapData [NUMBER_OF_CUBE_MAP_FACES];
	int x[NUMBER_OF_CUBE_MAP_FACES], y[NUMBER_OF_CUBE_MAP_FACES], bytesPerPixel[NUMBER_OF_CUBE_MAP_FACES];
	unsigned int index = 0;
	cubeMapData[index] = stbi_load(posXFileName.c_str(), &x[index], &y[index], &bytesPerPixel[index], 4 /* req_comp */);
	if (cubeMapData[index] == NULL)
	{
		std::string name = posXFileName;
		const char *tmp = strrchr(posXFileName.c_str(), '\\');
		if (tmp != NULL)
		{
			name.assign(tmp + 1);
		}
		LOG(Utility::Error, LOGPLACE, "Unable to load texture from the file \"%s\"", name.c_str());
		exit(EXIT_FAILURE);
	}

	index++;
	cubeMapData[index] = stbi_load(negXFileName.c_str(), &x[index], &y[index], &bytesPerPixel[index], 4 /* req_comp */);
	if (cubeMapData[index] == NULL)
	{
		std::string name = negXFileName;
		const char *tmp = strrchr(negXFileName.c_str(), '\\');
		if (tmp != NULL)
		{
			name.assign(tmp + 1);
		}
		LOG(Utility::Error, LOGPLACE, "Unable to load texture from the file \"%s\"", name.c_str());
		exit(EXIT_FAILURE);
	}

	index++;
	cubeMapData[index] = stbi_load(posYFileName.c_str(), &x[index], &y[index], &bytesPerPixel[index], 4 /* req_comp */);
	if (cubeMapData[index] == NULL)
	{
		std::string name = posYFileName;
		const char *tmp = strrchr(posYFileName.c_str(), '\\');
		if (tmp != NULL)
		{
			name.assign(tmp + 1);
		}
		LOG(Utility::Error, LOGPLACE, "Unable to load texture from the file \"%s\"", name.c_str());
		exit(EXIT_FAILURE);
	}

	index++;
	cubeMapData[index] = stbi_load(negYFileName.c_str(), &x[index], &y[index], &bytesPerPixel[index], 4 /* req_comp */);
	if (cubeMapData[index] == NULL)
	{
		std::string name = negYFileName;
		const char *tmp = strrchr(negYFileName.c_str(), '\\');
		if (tmp != NULL)
		{
			name.assign(tmp + 1);
		}
		LOG(Utility::Error, LOGPLACE, "Unable to load texture from the file \"%s\"", name.c_str());
		exit(EXIT_FAILURE);
	}

	index++;
	cubeMapData[index] = stbi_load(posZFileName.c_str(), &x[index], &y[index], &bytesPerPixel[index], 4 /* req_comp */);
	if (cubeMapData[index] == NULL)
	{
		std::string name = posZFileName;
		const char *tmp = strrchr(posZFileName.c_str(), '\\');
		if (tmp != NULL)
		{
			name.assign(tmp + 1);
		}
		LOG(Utility::Error, LOGPLACE, "Unable to load texture from the file \"%s\"", name.c_str());
		exit(EXIT_FAILURE);
	}

	index++;
	cubeMapData[index] = stbi_load(negZFileName.c_str(), &x[index], &y[index], &bytesPerPixel[index], 4 /* req_comp */);
	if (cubeMapData[index] == NULL)
	{
		std::string name = negZFileName;
		const char *tmp = strrchr(negZFileName.c_str(), '\\');
		if (tmp != NULL)
		{
			name.assign(tmp + 1);
		}
		LOG(Utility::Error, LOGPLACE, "Unable to load texture from the file \"%s\"", name.c_str());
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < NUMBER_OF_CUBE_MAP_FACES - 1; ++i)
	{
		if (x[i] != x[i+1])
		{
			LOG(Utility::Error, LOGPLACE, "All cube map texture's faces must have the same width, but face %d has width=%d and face %d has width=%d", i, x[i], i+1, x[i+1]);
		}
		if (y[i] != y[i+1])
		{
			LOG(Utility::Error, LOGPLACE, "All cube map texture's faces must have the same height, but face %d has height=%d and face %d has height=%d", i, x[i], i+1, x[i+1]);
		}
	}

	int width = x[0];
	int height = y[0];
	int depth = 0;
	textureData = new TextureData(cubeMapData, width, height, depth);
}

Texture::Texture(int width /* = 0 */, int height /* = 0 */, unsigned char* data /* = 0 */, GLenum textureTarget /* = GL_TEXTURE_2D */, GLfloat filter /* = GL_LINEAR_MIPMAP_LINEAR */, GLenum internalFormat /*=GL_RGBA*/, GLenum format /*=GL_RGBA*/, bool clampEnabled /*=false*/, GLenum attachment /*= GL_NONE*/) :
	textureData(NULL),
	fileName()
{
	//ASSERT(data != NULL);
	ASSERT(width > 0);
	ASSERT(height > 0);
	if (data == NULL)
	{
		LOG(Utility::Debug, LOGPLACE, "Cannot initialize texture. Passed texture data is NULL");
	}
	if ((width <= 0) || (height <= 0))
	{
		LOG(Utility::Error, LOGPLACE, "Cannot initialize texture. Passed texture size is incorrect (width=%d; height=%d)", width, height);
		return;
	}
	textureData = new TextureData(textureTarget, width, height, 1, &data, &filter, &internalFormat, &format, clampEnabled, &attachment);
	ASSERT(textureData != NULL);
	if (textureData == NULL)
	{
		LOG(Utility::Error, LOGPLACE, "Texture data creation error. Texture data is NULL.");
	}
}

Texture::~Texture(void)
{
	ASSERT(textureData != NULL);
	if (textureData == NULL)
	{
		LOG(Utility::Warning, LOGPLACE, "Texture data is already NULL");
		return;
	}
	
	textureData->RemoveReference();
	if (! textureData->IsReferenced())
	{
		if (fileName.length() > 0)
		{
			textureResourceMap.erase(fileName);
		}
		delete textureData;
		textureData = NULL;
	}
}

void Texture::Bind(unsigned int unit /* = 0 */, bool is3DTexture /* = false */) const
{
	ASSERT(textureData != NULL);
	ASSERT((unit >= 0) && (unit < TextureData::MAX_BOUND_TEXTURES_COUNT));
	if (textureData == NULL)
	{
		LOG(Utility::Emergency, LOGPLACE, "Cannot bind the texture. Texture data is NULL");
		return;
	}
	
	if (is3DTexture)
	{
		glActiveTexture(GL_TEXTURE3);
	}
	else
	{
		glActiveTexture(GL_TEXTURE0 + unit);
	}
	textureData->Bind(0);
}

void Texture::BindAsRenderTarget() const
{
	if (textureData == NULL)
	{
		LOG(Utility::Error, LOGPLACE, "Cannot bind the texture data as render target. Texture data is NULL.");
		return;
	}
	textureData->BindAsRenderTarget();
}

//CubeMapTexture::CubeMapTexture(const Texture& posXTexture, const Texture& negXTexture, const Texture& posYTexture, const Texture& negYTexture, const Texture& posZTexture, const Texture& negZTexture)
//{
//	textureData = new TextureData(GL_TEXTURE_CUBE_MAP, posYTexture.GetWidth(), posXTexture.GetHeight(), 1, NULL, NULL, NULL, NULL, true, NULL);
//}
//
//CubeMapTexture::~CubeMapTexture(void)
//{
//}