#include "StdAfx.h"
#include "TextRenderer.h"
#include "Material.h"
#include "Transform.h"
#include "Renderer.h"
#include "CoreEngine.h"
#include "Utility\IConfig.h"
#include "Utility\ILogger.h"

using namespace Rendering;

const int vertexTempID = 0;
const int uvTempID = 1;

TextRenderer::TextRenderer(Texture* fontTexture, Math::Real defaultFontSize /* = 32.0f */) :
	m_fontMaterial(NULL),
	m_defaultFontSize(defaultFontSize),
	m_defaultFontColor(GET_CONFIG_VALUE("defaultTextColorRed", REAL_ONE), GET_CONFIG_VALUE("defaultTextColorGreen", REAL_ZERO), GET_CONFIG_VALUE("defaultTextColorBlue", REAL_ZERO)),
	m_textShader(NULL),
	m_windowWidth(static_cast<Math::Real>(CoreEngine::GetCoreEngine()->GetWindowWidth())),
	m_windowHeight(static_cast<Math::Real>(CoreEngine::GetCoreEngine()->GetWindowHeight())),
	m_transform(), /* TODO: Fix transform. */
	m_projection(REAL_ZERO, m_windowWidth, REAL_ZERO, m_windowHeight, -REAL_ONE, REAL_ONE)
{
	if (fontTexture == NULL)
	{
		WARNING_LOG("The text renderer will not work properly. The specified font texture is NULL");
	}
	//m_fontTexture = new Texture("font1.bmp");
	m_fontMaterial = new Material(fontTexture);
	m_textShader = new Shader((GET_CONFIG_VALUE_STR("textShader", "text-shader")));

	//Vertex vertices[] = {
	//	Vertex(Math::Vector3D(REAL_ZERO, REAL_ZERO, REAL_ZERO), Math::Vector2D(REAL_ZERO, REAL_ZERO)),
	//	Vertex(Math::Vector3D(fontSize, REAL_ZERO, REAL_ZERO), Math::Vector2D(REAL_ZERO, REAL_ZERO)),
	//	Vertex(Math::Vector3D(fontSize, fontSize, REAL_ZERO), Math::Vector2D(REAL_ZERO, REAL_ZERO)),
	//	Vertex(Math::Vector3D(REAL_ZERO, fontSize, REAL_ZERO), Math::Vector2D(REAL_ZERO, REAL_ZERO))
	//};
 //   float vertices [] = {
 //       0.0f, 0.0f,
 //       fontSize, 0.0f, 
 //       fontSize, fontSize,
 //       0.0f, fontSize
 //   };


	//glGenVertexArrays(1, &vaoID);
	//glBindVertexArray(vaoID);

	glGenBuffers(1, &m_vertexBuffer);
	//glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8, &vertices[0], GL_STATIC_DRAW);
	//glEnableVertexAttribArray(vertexTempID);
	//glVertexAttribPointer(vertexTempID, 2, GL_FLOAT, GL_FALSE, 0, 0);
	Rendering::CheckErrorCode("TextRenderer::TextRenderer", "Generating and binding vertex buffer");

    //Just initialize with something for now, the tex coords are updated
    //for each character printed
    //float texCoords [] = {
    //    0.0f, 0.0f,
    //    0.0f, 0.0f, 
    //    0.0f, 0.0f,
    //    0.0f, 0.0f
    //};
	glGenBuffers(1, &m_texCoordBuffer);
	//glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8, &texCoords[0], GL_DYNAMIC_DRAW);
	//glEnableVertexAttribArray(uvTempID);
	//glVertexAttribPointer(uvTempID, 2, GL_FLOAT, GL_FALSE, 0, 0);
	Rendering::CheckErrorCode("TextRenderer::TextRenderer", "Generating and binding texture coordinates buffer");
	//m_fontMaterial->SetReal("screenWidth", m_windowWidth);
	//m_fontMaterial->SetReal("screenHeight", m_windowHeight);
}


TextRenderer::~TextRenderer(void)
{
	DELOCUST_LOG("Text renderer destruction started");
	SAFE_DELETE(m_fontMaterial);
	SAFE_DELETE(m_textShader);

	glDeleteVertexArrays(1, &m_vaoID);
	glDeleteBuffers(1, &m_vertexBuffer);
	glDeleteBuffers(1, &m_texCoordBuffer);

	DEBUG_LOG("Text renderer destruction finished");
}

void TextRenderer::DrawString(Text::Alignment alignment, int y, const std::string& str, Renderer* renderer) const
{
	DrawString(alignment, y, str, renderer, m_defaultFontSize, m_defaultFontColor);
}

void TextRenderer::DrawString(Text::Alignment alignment, int y, const std::string& str, Renderer* renderer, Math::Real fontSize) const
{
	DrawString(alignment, y, str, renderer, fontSize, m_defaultFontColor);
}

void TextRenderer::DrawString(Text::Alignment alignment, int y, const std::string& str, Renderer* renderer, const Math::Vector3D& fontColor) const
{
	DrawString(alignment, y, str, renderer, m_defaultFontSize, fontColor);
}

void TextRenderer::DrawString(Text::Alignment alignment, int y, const std::string& str, Renderer* renderer, Math::Real fontSize, const Math::Vector3D& fontColor) const
{
	int x = 0;
	switch (alignment)
	{
	case Text::LEFT:
		x = 0;
		break;
	case Text::RIGHT:
		x = static_cast<int>(m_windowWidth - str.size() * fontSize);
		break;
	case Text::CENTER:
		x = static_cast<int>(m_windowWidth - str.size() * fontSize) / 2;
		DEBUG_LOG("Drawing string \"%s\": x = %d, window width = %.2f", str.c_str(), x, m_windowWidth);
		break;
	default:
		WARNING_LOG("Incorrect alignment type used (%d). The text will start at default x=%.1f value", alignment, x);
	}
	DrawString(x, y, str, renderer, fontSize, fontColor);
}

void TextRenderer::DrawString(int x, int y, const std::string& str, Renderer* renderer) const
{
	DrawString(x, y, str, renderer, m_defaultFontSize, m_defaultFontColor);
}

void TextRenderer::DrawString(int x, int y, const std::string& str, Renderer* renderer, Math::Real fontSize) const
{
	DrawString(x, y, str, renderer, fontSize, m_defaultFontColor);
}

void TextRenderer::DrawString(int x, int y, const std::string& str, Renderer* renderer, const Math::Vector3D& fontColor) const
{
	DrawString(x, y, str, renderer, m_defaultFontSize, fontColor);
}

void TextRenderer::DrawString(int x, int y, const std::string& str, Renderer* renderer, Math::Real fontSize, const Math::Vector3D& fontColor) const
{
	DELOCUST_LOG("Started drawing string \"%s\"", str.c_str());

	Rendering::CheckErrorCode("TextRenderer::DrawString", "Started drawing a string");

	std::vector<Math::Vector2D> vertices;
	std::vector<Math::Vector2D> uvs;
	Math::Real yReal = static_cast<Math::Real>(y);
	const Math::Real screenHalfWidth = m_windowWidth / 2;
	const Math::Real screenHalfHeight = m_windowHeight / 2;
	for (std::string::size_type i = 0; i < str.size(); ++i)
	{
		// Our vertices need to be represented in the clipping space, that is why we convert the X and Y components of the vertices
		// below from the screen space coordinates ([0..screenWidth][0..screenHeight]) to clip space coordinates ([-1..1][-1..1]).
		// The conversion is done by first subtracting the half of width / height (for X and Y components respectively) and then dividing the result by the same value.
		Math::Vector2D upLeftVec(((x + i * fontSize) - screenHalfWidth) / screenHalfWidth, ((yReal + fontSize) - screenHalfHeight) / screenHalfHeight);
		Math::Vector2D upRightVec(((x + i * fontSize + fontSize) - screenHalfWidth) / screenHalfWidth, ((yReal + fontSize) - screenHalfHeight) / screenHalfHeight);
		Math::Vector2D downRightVec(((x + i * fontSize + fontSize) - screenHalfWidth) / screenHalfWidth, (static_cast<Math::Real>(yReal) - screenHalfHeight) / screenHalfHeight);
		Math::Vector2D downLeftVec(((x + i * fontSize) - screenHalfWidth) / screenHalfWidth, (yReal - screenHalfHeight) / screenHalfHeight);
		//CRITICAL_LOG("str = \"%s\" upRightVec = %s", str.c_str(), upRightVec.ToString().c_str());

		vertices.push_back(upLeftVec);
		vertices.push_back(downLeftVec);
		vertices.push_back(upRightVec);
		vertices.push_back(downRightVec);
		vertices.push_back(upRightVec);
		vertices.push_back(downLeftVec);

		const Math::Real oneOverSixteen = REAL_ONE / 16.0f;
		int ch = static_cast<int>(str[i]);
		Math::Real xUV = static_cast<Math::Real>(ch % 16) * oneOverSixteen;
		Math::Real yUV = REAL_ONE - ((static_cast<Math::Real>(ch / 16) * oneOverSixteen) + oneOverSixteen);
		//INFO_LOG("character=\"%c\"\t ascii value=%d, xUV = %.2f, yUV = %.2f", str[i], ch, xUV, yUV);

		Math::Vector2D upLeftUV(xUV, REAL_ONE - (yUV + oneOverSixteen ));
		Math::Vector2D upRightUV(xUV + oneOverSixteen, REAL_ONE - (yUV + oneOverSixteen));
		Math::Vector2D downRightUV(xUV + oneOverSixteen, REAL_ONE - yUV);
		Math::Vector2D downLeftUV(xUV, REAL_ONE - yUV);
		uvs.push_back(upLeftUV);
		uvs.push_back(downLeftUV);
		uvs.push_back(upRightUV);
		uvs.push_back(downRightUV);
		uvs.push_back(upRightUV);
		uvs.push_back(downLeftUV);
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Math::Vector2D), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(Math::Vector2D), &uvs[0], GL_STATIC_DRAW);

	m_textShader->Bind();

	//Updating uniforms
	m_fontMaterial->SetVector3D("textColor", fontColor);
	m_fontMaterial->SetReal("screenWidth", m_windowWidth);
	m_fontMaterial->SetReal("screenHeight", m_windowHeight);
	m_textShader->UpdateUniforms(m_transform, *m_fontMaterial, renderer);
	//textShader->SetUniformMatrix("MVP", Math::Matrix4D(x, y, REAL_ZERO) * projection);
	//fontTexture->Bind(25);
	//textShader->SetUniformi("R_fontTexture", 25);	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	if (Rendering::glDepthTestEnabled)
	{
		glDisable(GL_DEPTH_TEST);
	}
	glCullFace(GL_FRONT);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_ONE, GL_ONE); // the existing color will be blended with the new color with both wages equal to 1
	//glDepthMask(GL_FALSE); // Disable writing to the depth buffer (Z-buffer). We are after the ambient rendering pass, so we do not need to write to Z-buffer anymore
	//glDepthFunc(GL_EQUAL); // CRITICAL FOR PERFORMANCE SAKE! This will allow calculating the light only for the pixel which will be seen in the final rendered image
	/**
	 * TODO: We should first save the blend-specific parameters and restore them once glDrawArrays function is finished.
	 * See how it is done for GL_DEPTH_TEST here, in this function.
	 */
	 // Save the current blending state
	if (!Rendering::glBlendEnabled)
	{
		glEnable(GL_BLEND);
	}
	/**
	 * This effectively means:
	 * newColorInFramebuffer = currentAlphaInFramebuffer * current color in framebuffer +
	 * (1 - currentAlphaInFramebuffer) * shader's output color
	 */
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()));
	// Now restore the blending state
	glBlendFunc(Rendering::glBlendSfactor, Rendering::glBlendDfactor);
	if (!Rendering::glBlendEnabled)
	{
		glDisable(GL_BLEND);
	}
	glCullFace(Rendering::glCullFaceMode);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	//glCullFace(GL_BACK);
	if (Rendering::glDepthTestEnabled)
	{
		glEnable(GL_DEPTH_TEST);
	}
	Rendering::CheckErrorCode("TextRenderer::DrawString", "Finishing drawing a string");
}