#include "StdAfx.h"
#include "TextRenderer.h"
#include "Material.h"
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
	m_textShader(NULL)
{
	//m_fontTexture = new Texture("..\\Textures\\font1.bmp");
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

	m_windowWidth = static_cast<Math::Real>(CoreEngine::GetCoreEngine()->GetWindowWidth());
	m_windowHeight = static_cast<Math::Real>(CoreEngine::GetCoreEngine()->GetWindowHeight());
	//m_fontMaterial->SetReal("screenWidth", m_windowWidth);
	//m_fontMaterial->SetReal("screenHeight", m_windowHeight);
	m_projection = Math::Matrix4D::OrtographicProjection(REAL_ZERO, m_windowWidth, REAL_ZERO, m_windowHeight, -REAL_ONE, REAL_ONE);
}


TextRenderer::~TextRenderer(void)
{
	LOG(Utility::Delocust, LOGPLACE, "Text renderer destruction started");
	SAFE_DELETE(m_fontMaterial);
	SAFE_DELETE(m_textShader);

	glDeleteVertexArrays(1, &m_vaoID);
	glDeleteBuffers(1, &m_vertexBuffer);
	glDeleteBuffers(1, &m_texCoordBuffer);

	LOG(Utility::Debug, LOGPLACE, "Text renderer destruction finished");
}

void TextRenderer::DrawString(Text::Alignment alignment, Math::Real y, const std::string& str, Renderer* renderer) const
{
	DrawString(alignment, y, str, renderer, m_defaultFontSize, m_defaultFontColor);
}

void TextRenderer::DrawString(Text::Alignment alignment, Math::Real y, const std::string& str, Renderer* renderer, Math::Real fontSize) const
{
	DrawString(alignment, y, str, renderer, fontSize, m_defaultFontColor);
}

void TextRenderer::DrawString(Text::Alignment alignment, Math::Real y, const std::string& str, Renderer* renderer, const Math::Vector3D& fontColor) const
{
	DrawString(alignment, y, str, renderer, m_defaultFontSize, fontColor);
}

void TextRenderer::DrawString(Text::Alignment alignment, Math::Real y, const std::string& str, Renderer* renderer, Math::Real fontSize, const Math::Vector3D& fontColor) const
{
	Math::Real x = REAL_ZERO;
	switch (alignment)
	{
	case Text::LEFT:
		x = REAL_ZERO;
		break;
	case Text::RIGHT:
		x = m_windowWidth - str.size() * fontSize;
		break;
	case Text::CENTER:
		x = (m_windowWidth - str.size() * fontSize) / 2;
		break;
	default:
		LOG(Utility::Warning, LOGPLACE, "Incorrect alignment type used.");
	}
	DrawString(x, y, str, renderer, fontSize, fontColor);
}

void TextRenderer::DrawString(Math::Real x, Math::Real y, const std::string& str, Renderer* renderer) const
{
	DrawString(x, y, str, renderer, m_defaultFontSize, m_defaultFontColor);
}

void TextRenderer::DrawString(Math::Real x, Math::Real y, const std::string& str, Renderer* renderer, Math::Real fontSize) const
{
	DrawString(x, y, str, renderer, fontSize, m_defaultFontColor);
}

void TextRenderer::DrawString(Math::Real x, Math::Real y, const std::string& str, Renderer* renderer, const Math::Vector3D& fontColor) const
{
	DrawString(x, y, str, renderer, m_defaultFontSize, fontColor);
}

void TextRenderer::DrawString(Math::Real x, Math::Real y, const std::string& str, Renderer* renderer, Math::Real fontSize, const Math::Vector3D& fontColor) const
{
	LOG(Utility::Delocust, LOGPLACE, "Started drawing string \"%s\"", str.c_str());

	Rendering::CheckErrorCode("TextRenderer::DrawString", "Started drawing a string");

	std::vector<Math::Vector2D> vertices;
	std::vector<Math::Vector2D> uvs;
	for (std::string::size_type i = 0; i < str.size(); ++i)
	{
		Math::Vector2D upLeftVec(x + i * fontSize, y + fontSize);
		Math::Vector2D upRightVec(x + i * fontSize + fontSize, y + fontSize);
		Math::Vector2D downRightVec(x + i * fontSize + fontSize, y);
		Math::Vector2D downLeftVec(x + i * fontSize, y);
		//LOG(Utility::Critical, LOGPLACE, "str = \"%s\" upRightVec = %s", str.c_str(), upRightVec.ToString().c_str());

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
		//LOG(Utility::Info, LOGPLACE, "character=\"%c\"\t ascii value=%d, xUV = %.2f, yUV = %.2f", str[i], ch, xUV, yUV);

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
	m_textShader->UpdateUniforms(Transform() /* TODO: Create something better here */, *m_fontMaterial, renderer);
	//textShader->SetUniformMatrix("MVP", Math::Matrix4D::Translation(x, y, REAL_ZERO) * projection);
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
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
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
	//exit(EXIT_FAILURE);
}