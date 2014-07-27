#include "StdAfx.h"
#include "TextureData.h"
#include "Utility\Log.h"

using namespace Rendering;

TextureData::TextureData(GLenum textureTarget, int width, int height, int texturesCount /* = 1 */, unsigned char** data, GLfloat* filters, GLenum* attachments) :
	textureTarget(textureTarget),
	texturesCount(texturesCount),
	framebuffer(0),
	renderbuffer(0)
{
	ASSERT(texturesCount > 0);
	if (texturesCount < 1)
	{
		stdlog(Utility::Emergency, LOGPLACE, "Incorrect number of textures specified");
		exit(EXIT_FAILURE);
	}
	textureID = new GLuint[texturesCount];
	this->width = width;
	this->height = height;

	InitTextures(data, filters);
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

void TextureData::InitTextures(unsigned char** data, GLfloat* filters)
{
	//ASSERT(data.size == texturesCount);
	//ASSERT(filter.size == texturesCount);
	ASSERT(data != NULL);
	ASSERT(filter != NULL);
	if (data == NULL)
	{
		stdlog(Utility::Error, LOGPLACE, "Cannot initialize texture. Passed texture data is NULL");
		return;
	}
	if (filters != NULL)
	{
		stdlog(Utility::Error, LOGPLACE, "The filter array is NULL.");
	}
	
	glGenTextures(texturesCount, textureID);
	for (int i = 0; i < texturesCount; ++i)
	{
		ASSERT(data[i] != NULL)
		if (data[i] == NULL)
		{
			stdlog(Utility::Error, LOGPLACE, "The texture data array is invalid. Incorrect array size.");
			return;
		}
		glBindTexture(textureTarget, textureID[i]);
		glTexParameterf(textureTarget, GL_TEXTURE_MIN_FILTER, filters[i]);
		glTexParameterf(textureTarget, GL_TEXTURE_MAG_FILTER, filters[i]);
		glTexImage2D(textureTarget, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data[i]);
	}
}

void TextureData::Bind(int textureIndex)
{
	ASSERT(textureIndex >= 0);
	ASSERT(textureIndex < texturesCount);
	if ((textureIndex < 0) || (textureIndex >= texturesCount))
	{
		stdlog(Utility::Error, LOGPLACE,
			"Cannot bind the texture with textureID=%d. The value is out of range (%d, %d)", textureIndex, 0, texturesCount);
		return;
	}
	glBindTexture(textureTarget, textureID[textureIndex]);
}

void TextureData::InitRenderTargets(GLenum* attachments)
{
	if (attachments == NULL)
	{
		stdlog(Utility::Debug, LOGPLACE, "No attachments used");
		return;
	}

	GLenum* drawBuffers = new GLenum[texturesCount];
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
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
		}
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, attachments[i], textureTarget, textureID[0], 0); // associate framebuffer with texture
	}

	if (framebuffer == 0)
	{
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
	delete [] drawBuffers;
	drawBuffers = NULL;

	ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		stdlog(Utility::Critical, LOGPLACE, "Framebuffer creation failed. The framebuffer status is not GL_FRAMEBUFFER_COMPLETE.");
		exit(-1);
	}
}

void TextureData::BindAsRenderTarget()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
	glViewport(0, 0, width, height);
}