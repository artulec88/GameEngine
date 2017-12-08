#include "stdafx.h"
#include "TextureBuilder.h"
#include "Texture.h"

rendering::TextureBuilder::TextureBuilder(const std::string& fileName) :
	m_defaultTarget(GL_TEXTURE_2D),
	m_defaultFilter(GL_LINEAR_MIPMAP_LINEAR),
	m_defaultInternalFormat(GL_RGBA),
	m_defaultFormat(GL_RGBA),
	m_defaultWrapping(GL_REPEAT),
	m_defaultAttachment(GL_NONE),
	m_fileName(fileName),
	m_target(m_defaultTarget),
	m_filter(m_defaultFilter),
	m_internalFormat(m_defaultInternalFormat),
	m_format(m_defaultFormat),
	m_wrapping(m_defaultWrapping),
	m_attachment(m_defaultAttachment)
{
}


rendering::TextureBuilder::~TextureBuilder()
{
}

rendering::Texture rendering::TextureBuilder::Build()
{
	return Texture(*this);
}
