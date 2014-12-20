#include "StdAfx.h"
#include "TextureData.h"
#include "Utility\ILogger.h"

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
		LOG(Utility::Warning, LOGPLACE, "Cannot initialize texture. Passed texture data is NULL");
		//return;
	}
	if (filters == NULL)
	{
		LOG(Utility::Warning, LOGPLACE, "The filter array is NULL.");
	}
	
	glGenTextures(texturesCount, textureID);
	for (int i = 0; i < texturesCount; ++i)
	{
		//ASSERT(data[i] != NULL)
		if (data[i] == NULL)
		{
			LOG(Utility::Warning, LOGPLACE, "The texture data array is invalid. Texture data[%d] is NULL.", i);
			//return;
		}
		glBindTexture(textureTarget, textureID[i]);
		glTexParameterf(textureTarget, GL_TEXTURE_MIN_FILTER, filters[i]);
		glTexParameterf(textureTarget, GL_TEXTURE_MAG_FILTER, filters[i]);

		if (clampEnabled)
		{
			glTexParameterf(textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glTexImage2D(textureTarget, 0, internalFormat[i], width, height, 0, format[i], GL_UNSIGNED_BYTE, data[i]);

		//if(filters[i] == GL_NEAREST_MIPMAP_NEAREST ||
		//	filters[i] == GL_NEAREST_MIPMAP_LINEAR ||
		//	filters[i] == GL_LINEAR_MIPMAP_NEAREST ||
		//	filters[i] == GL_LINEAR_MIPMAP_LINEAR)
		//{
		//	glGenerateMipmap(m_textureTarget);
		//	GLfloat maxAnisotropy;
		//	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
		//	glTexParameterf(m_textureTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, Clamp(0.0f, 8.0f, maxAnisotropy));
		//}
		//else
		//{
		//	glTexParameteri(m_textureTarget, GL_TEXTURE_BASE_LEVEL, 0);
		//	glTexParameteri(m_textureTarget, GL_TEXTURE_MAX_LEVEL, 0);
		//}
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