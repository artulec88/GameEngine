#include "StdAfx.h"
#include "Texture.h"
#include "stb_image.h"
#include "Utility\Utility.h"
#include "Utility\Log.h"

using namespace Rendering;

Texture::Texture(const std::string& fileName, GLenum textureTarget /* = GL_TEXTURE_2D */, GLfloat filter /* = GL_LINEAR */) :
	m_textureID(0),
	m_textureTarget(textureTarget),
	m_filter(filter)
{
	LoadFromFile(fileName);
}

Texture::~Texture(void)
{
	glDeleteTextures(1, &m_textureID);
}

void Texture::LoadFromFile(const std::string& fileName)
{
	// TODO: Check whether the fileName is a full path or just a fileName. Act accordingly.
	std::string name = fileName;
	const char *tmp = strrchr(fileName.c_str(), '\\');
	if (tmp != NULL)
	{
		name.assign(tmp + 1);
	}
	stdlog(Utility::Info, LOGPLACE, "Loading texture from file \"%s\"", name.c_str());
	std::string extension = name.substr(name.find_last_of(".") + 1);
	stdlog(Utility::Delocust, LOGPLACE, "Extension is = \"%s\"", extension.c_str());

	//if (extension != "obj")
	//{
	//	stdlog(Utility::Error, LOGPLACE, "File format \"%s\" is not supported for mesh data", extension.c_str());
	//	return;
	//}

	int x, y, bytesPerPixel;
	unsigned char* data = stbi_load(fileName.c_str(), &x, &y, &bytesPerPixel, 4);

	if (data == NULL)
	{
		stdlog(Utility::Error, LOGPLACE, "Unable to load texture from the file \"%s\"", name.c_str());
		return;
	}

	this->Init(x, y, data);
	stbi_image_free(data);
}

void Texture::Init(int width, int height, unsigned char* data)
{
	if(width > 0 && height > 0 && data != 0)
	{
		glGenTextures(1, &m_textureID);
		glBindTexture(m_textureTarget, m_textureID);
		glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, m_filter);
		glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, m_filter);
		glTexImage2D(m_textureTarget, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		m_textureID = 0;
	}
}

void Texture::Bind(unsigned int unit /* = 0 */) const
{
	ASSERT((unit >= 0) && (unit < 32));
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(m_textureTarget, m_textureID);
}