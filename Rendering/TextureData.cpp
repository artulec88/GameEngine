#include "StdAfx.h"
#include "TextureData.h"

using namespace Rendering;

TextureData::TextureData(GLenum textureTarget) :
	textureTarget(textureTarget)
{
	glGenTextures(1, &textureID);
}


TextureData::~TextureData(void)
{
	if (textureID)
	{
		glDeleteTextures(1, &textureID);
	}
}

GLenum TextureData::GetTextureTarget() const
{
	return textureTarget;
}

GLuint TextureData::GetTextureID() const
{
	return textureID;
}