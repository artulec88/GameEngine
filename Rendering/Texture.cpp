#include "StdAfx.h"
#include "Texture.h"
#include "stb_image.h"
#include "Utility\Utility.h"
#include "Utility\Log.h"

using namespace Rendering;

/* static */ const TextureData* Texture::lastBindTexture = 0;
/* static */ std::map<std::string, TextureData*> Texture::textureResourceMap;

Texture::Texture(const std::string& fileName, GLenum textureTarget /* = GL_TEXTURE_2D */, GLfloat filter /* = GL_LINEAR */) :
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
		stdlog(Utility::Info, LOGPLACE, "Loading texture from file \"%s\"", name.c_str());
		//std::string extension = name.substr(name.find_last_of(".") + 1);
		//stdlog(Utility::Delocust, LOGPLACE, "Extension is = \"%s\"", extension.c_str());

		int x, y, bytesPerPixel;
		unsigned char* data = stbi_load(fileName.c_str(), &x, &y, &bytesPerPixel, 4 /* req_comp */);

		if (data == NULL)
		{
			stdlog(Utility::Error, LOGPLACE, "Unable to load texture from the file \"%s\"", name.c_str());
			return;
		}

		Init(x, y, data, textureTarget, filter);
		stbi_image_free(data);
		ASSERT(textureData);
		textureResourceMap.insert(std::pair<std::string, TextureData*>(fileName, textureData));
	}
	else // (itr != textureResourceMap.end()) // texture has already been loaded
	{
		textureData = itr->second;
		textureData->AddReference();
	}

	//LoadFromFile(fileName);
}

Texture::Texture(int width /* = 0 */, int height /* = 0 */, unsigned char* data /* = 0 */, GLenum textureTarget /* = GL_TEXTURE_2D */, GLfloat filter /* = GL_LINEAR */) :
	textureData(NULL),
	fileName()
{
	Init(width, height, data, textureTarget, filter);
}

Texture::~Texture(void)
{
	ASSERT(textureData != NULL);
	if (textureData == NULL)
	{
		stdlog(Utility::Warning, LOGPLACE, "Texture data is already NULL");
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

void Texture::Init(int width, int height, unsigned char* data, GLenum textureTarget, GLfloat filter)
{
	ASSERT(data != NULL);
	ASSERT(width > 0);
	ASSERT(height > 0);
	if (data == NULL)
	{
		stdlog(Utility::Error, LOGPLACE, "Cannot initialize texture. Passed texture data is NULL");
		return;
	}
	if ((width <= 0) || (height <= 0))
	{
		stdlog(Utility::Error, LOGPLACE, "Cannot initialize texture. Passed texture size is incorrect (width=%d; height=%d)", width, height);
		return;
	}
	textureData = new TextureData(textureTarget);
	glBindTexture(textureTarget, textureData->GetTextureID());
	glTexParameterf(textureTarget, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameterf(textureTarget, GL_TEXTURE_MAG_FILTER, filter);
	glTexImage2D(textureTarget, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

void Texture::Bind(unsigned int unit /* = 0 */) const
{
	ASSERT(textureData != NULL);
	ASSERT((unit >= 0) && (unit < 32));
	if (textureData == NULL)
	{
		stdlog(Utility::Emergency, LOGPLACE, "Cannot bind the texture. Texture data is NULL");
		return;
	}

	if (lastBindTexture != textureData)
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(textureData->GetTextureTarget(), textureData->GetTextureID());
		lastBindTexture = textureData;
	}
}