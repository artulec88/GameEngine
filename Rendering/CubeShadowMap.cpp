#include "StdAfx.h"
#include "CubeShadowMap.h"
#include "Utility\ILogger.h"
#include "Math\Math.h"

Rendering::CubeShadowMap::CubeShadowMap(void) :
	m_fbo(0),
	m_renderBuffer(0),
	m_shadowMap(0),
	m_depth(0)
{
}


Rendering::CubeShadowMap::~CubeShadowMap(void)
{
	if (m_fbo != 0)
	{
		glDeleteFramebuffers(1, &m_fbo);
	}

	if (m_renderBuffer != 0)
	{
		glDeleteRenderbuffers(1, &m_renderBuffer);
	}

	if (m_shadowMap != 0)
	{
		glDeleteTextures(1, &m_shadowMap);
	}

	if (m_depth != 0)
	{
		glDeleteTextures(1, &m_depth);
	}
}

void Rendering::CubeShadowMap::Init(unsigned int windowWidth, unsigned int windowHeight)
{
	// See http://stackoverflow.com/questions/12879969/hardware-support-for-non-power-of-two-textures

	unsigned int windowWidthPowerOfTwo = Math::RoundUpPow2(windowWidth);
	unsigned int windowHeightPowerOfTwo = Math::RoundUpPow2(windowHeight);
	unsigned int cubeMapSize = (windowWidthPowerOfTwo > windowHeightPowerOfTwo) ? windowWidthPowerOfTwo : windowHeightPowerOfTwo;

	INFO_LOG_RENDERING("Initializing cube shadow map with width=", windowWidth, " and height=", windowHeight, " (cubeMapSize=", cubeMapSize, ")");
	const int NUMBER_OF_CUBE_MAP_FACES = 6;

	// Create the FBO
	glGenFramebuffers(1, &m_fbo);

	// Create the depth buffer
	glGenTextures(1, &m_depth);
	glBindTexture(GL_TEXTURE_2D, m_depth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, cubeMapSize, cubeMapSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Create the cube map
	glGenTextures(1, &m_shadowMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_shadowMap);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	for (unsigned int i = 0; i < NUMBER_OF_CUBE_MAP_FACES; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_R32F, cubeMapSize, cubeMapSize, 0, GL_RED, GL_FLOAT, NULL);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth, 0);
	//DEBUG_LOG_RENDERING("The texture does not have any depth attachment. Creating the render buffer is started.");
	//glGenRenderbuffers(1, &m_renderBuffer);
	//glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, windowWidth, windowHeight);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_renderBuffer);

	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, m_shadowMap, 0);
	//for (unsigned int i = 0; i < NUMBER_OF_CUBE_MAP_FACES; ++i)
	//{
	//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_shadowMap, 0);
	//}
	//glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_shadowMap, 0);

	// Disable writes to the color buffer
	glDrawBuffer(GL_NONE);

	// Disable reads from the color buffer
	glReadBuffer(GL_NONE);

	Rendering::CheckFramebufferStatus();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Rendering::CheckErrorCode(__FUNCTION__, "Initializing cube shadow map");
}

void Rendering::CubeShadowMap::BindForWriting(GLenum cubeFace) const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, cubeFace, m_shadowMap, 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	Rendering::CheckFramebufferStatus();
}


void Rendering::CubeShadowMap::BindForReading(unsigned int textureUnit) const
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_shadowMap);
	Rendering::CheckErrorCode(__FUNCTION__, "Binding for reading");
}