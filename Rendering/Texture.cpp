#include "StdAfx.h"
#include "Texture.h"
#include "stb_image.h"
#include "Utility\Utility.h"
#include "Utility\ILogger.h"

using namespace Rendering;

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

Texture::Texture(int width /* = 0 */, int height /* = 0 */, unsigned char* data /* = 0 */, GLenum textureTarget /* = GL_TEXTURE_2D */, GLfloat filter /* = GL_LINEAR_MIPMAP_LINEAR */, GLenum internalFormat /*=GL_RGBA*/, GLenum format /*=GL_RGBA*/, bool clampEnabled /*=false*/, GLenum attachment /*= GL_NONE*/) :
	textureData(NULL),
	fileName()
{
	//ASSERT(data != NULL);
	ASSERT(width > 0);
	ASSERT(height > 0);
	if (data == NULL)
	{
		LOG(Utility::Warning, LOGPLACE, "Cannot initialize texture. Passed texture data is NULL");
		//return;
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

void Texture::Bind(unsigned int unit /* = 0 */) const
{
	ASSERT(textureData != NULL);
	ASSERT((unit >= 0) && (unit < TextureData::MAX_BOUND_TEXTURES_COUNT));
	if (textureData == NULL)
	{
		LOG(Utility::Emergency, LOGPLACE, "Cannot bind the texture. Texture data is NULL");
		return;
	}
	
	glActiveTexture(GL_TEXTURE0 + unit);
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