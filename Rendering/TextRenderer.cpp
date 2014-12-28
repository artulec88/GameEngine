#include "StdAfx.h"
#include "TextRenderer.h"
#include "Texture.h"
#include "Utility\IConfig.h"
#include "Utility\ILogger.h"

using namespace Rendering;

const int vertexTempID = 0;
const int uvTempID = 1;

TextRenderer::TextRenderer(Texture* fontTexture, Math::Real fontSize /* = 32.0f */) :
	fontTexture(fontTexture),
	fontSize(fontSize),
	textShader(NULL)
{
	//fontTexture = new Texture("..\\Textures\\font1.bmp");
	textShader = new Shader((GET_CONFIG_VALUE_STR("textShader", "text-shader")));

	//Vertex vertices[] = {
	//	Vertex(Math::Vector3D(REAL_ZERO, REAL_ZERO, REAL_ZERO), Math::Vector2D(REAL_ZERO, REAL_ZERO)),
	//	Vertex(Math::Vector3D(fontSize, REAL_ZERO, REAL_ZERO), Math::Vector2D(REAL_ZERO, REAL_ZERO)),
	//	Vertex(Math::Vector3D(fontSize, fontSize, REAL_ZERO), Math::Vector2D(REAL_ZERO, REAL_ZERO)),
	//	Vertex(Math::Vector3D(REAL_ZERO, fontSize, REAL_ZERO), Math::Vector2D(REAL_ZERO, REAL_ZERO))
	//};
    float vertices [] = {
        0.0f, 0.0f,
        fontSize, 0.0f, 
        fontSize, fontSize,
        0.0f, fontSize
    };


	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8, &vertices[0], GL_STATIC_DRAW);
	//glEnableVertexAttribArray(vertexTempID);
	//glVertexAttribPointer(vertexTempID, 2, GL_FLOAT, GL_FALSE, 0, 0);
	Rendering::CheckErrorCode("TextRenderer::TextRenderer", "Generating and binding vertex buffer");

    //Just initialize with something for now, the tex coords are updated
    //for each character printed
    float texCoords [] = {
        0.0f, 0.0f,
        0.0f, 0.0f, 
        0.0f, 0.0f,
        0.0f, 0.0f
    };
	glGenBuffers(1, &texCoordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8, &texCoords[0], GL_DYNAMIC_DRAW);
	//glEnableVertexAttribArray(uvTempID);
	//glVertexAttribPointer(uvTempID, 2, GL_FLOAT, GL_FALSE, 0, 0);
	Rendering::CheckErrorCode("TextRenderer::TextRenderer", "Generating and binding texture coordinates buffer");

	projection = Math::Matrix4D::OrtographicProjection(REAL_ZERO, 1024 /* screen width */, REAL_ZERO, 600 /* screen height */, -REAL_ONE, REAL_ONE);
}


TextRenderer::~TextRenderer(void)
{
	LOG(Utility::Delocust, LOGPLACE, "Text renderer destruction started");
	SAFE_DELETE(fontTexture);
	SAFE_DELETE(textShader);

	glDeleteVertexArrays(1, &vaoID);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &texCoordBuffer);

	LOG(Utility::Debug, LOGPLACE, "Text renderer destruction finished");
}

void TextRenderer::DrawString(Math::Real x, Math::Real y, const std::string& str)
{
	LOG(Utility::Debug, LOGPLACE, "Started drawing string \"%s\"", str.c_str());

	Rendering::CheckErrorCode("TextRenderer::DrawString", "Started drawing a string");

	glCullFace(GL_FRONT);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_ONE, GL_ONE); // the existing color will be blended with the new color with both wages equal to 1
	//glDepthMask(GL_FALSE); // Disable writing to the depth buffer (Z-buffer). We are after the ambient rendering pass, so we do not need to write to Z-buffer anymore
	//glDepthFunc(GL_EQUAL); // CRITICAL FOR PERFORMANCE SAKE! This will allow calculating the light only for the pixel which will be seen in the final rendered image

	textShader->Bind();

	//Updating uniforms
	fontTexture->Bind(25);
	textShader->SetUniformi("R_fontTexture", 25);


	glEnableVertexAttribArray(vertexTempID);
	glEnableVertexAttribArray(uvTempID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(vertexTempID, 2, GL_FLOAT, GL_FALSE, 0, 0);
	//glVertexAttribPointer(uvTempID, 2, GL_FLOAT, GL_FALSE, 0, 0);

	GLint test[1];
	glGetIntegerv(GL_DEPTH_TEST, test);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	if (textShader == NULL)
	{
		LOG(Utility::Error, LOGPLACE, "Cannot draw a string. Text shader is NULL.");
		return;
	}
	Math::Real texCoords[8];
	
	Rendering::CheckErrorCode("TextRenderer::DrawString", "Binding a font texture");

	Math::Matrix4D translate = Math::Matrix4D::Translation(x, y, REAL_ZERO);
	Math::Matrix4D MV = Math::Matrix4D::Identity() * translate; // = translate

	glBindVertexArray(vaoID);
	/* int offset = 0; */
	for (std::string::size_type i = 0; i < str.size(); ++i /*, offset += sizeof(Math::Real) * 8 */)
	{
		LOG(Utility::Delocust, LOGPLACE, "MV = \"\n%s\"", MV.ToString());
		Math::Matrix4D MVP = projection * MV;
		const Math::Real oneOverSixteen = REAL_ONE / 16.0f;
		int ch = static_cast<int>(str[i]);
		Math::Real xPos = static_cast<Math::Real>(ch % 16) * oneOverSixteen;
		Math::Real yPos = static_cast<Math::Real>(ch / 16) * oneOverSixteen;

		//LOG(Utility::Info, LOGPLACE, "xPos = %.2f\tyPos = %.2f", xPos, yPos);
		texCoords[0] = xPos;
		texCoords[1] = REAL_ONE - yPos - oneOverSixteen;

		texCoords[2] = xPos + oneOverSixteen;
        texCoords[3] = REAL_ONE - yPos - oneOverSixteen;

        texCoords[4] = xPos + oneOverSixteen;
        texCoords[5] = REAL_ONE - yPos - 0.001f;

        texCoords[6] = xPos;
        texCoords[7] = REAL_ONE - yPos - 0.001f;
		//for (int j = 0; j < 8; ++j)
		//{
		//	LOG(Utility::Info, LOGPLACE, "texCoords[%d] = %.2f", j, texCoords[j]);
		//}

		glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0 /* offset */, sizeof(Math::Real) * 8, &texCoords[0]);

		//glGetUniformLocation(_program, uniform.c_str())
		textShader->SetUniformMatrix("MVP", MVP);
		//LOG(Utility::Info, LOGPLACE, "MVP = \"\n%s\"", MVP.ToString());
		//glUniformMatrix4fv(0, 1, GL_FALSE, &MVP[0][0]);
		Rendering::CheckErrorCode("TextRenderer::DrawString", "Updating MVP uniform");

		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		Rendering::CheckErrorCode("TextRenderer::DrawString", "The actual draw command");

		//Math::Matrix4D oldMV = MV;
		//Math::Matrix4D temp = Math::Matrix4D::Identity();
		//temp[3][0] = fontSize * 0.8f;
		//MV = temp * oldMV;
		MV[3][0] += fontSize * 0.8f;

		Rendering::CheckErrorCode("TextRenderer::DrawString", "Finished iteration step of the DrawString function");
	}
	glBindVertexArray(0);
	textShader->Unbind();

	Rendering::CheckErrorCode("TextRenderer::DrawString", "Finished the DrawString function");

	//if (test[0]) { glEnable(GL_DEPTH_TEST); }
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	
	glCullFace(GL_BACK);

	//glDepthMask(GL_TRUE);
	//glDepthFunc(GL_LESS);
	//glDisable(GL_BLEND);

	glDisableVertexAttribArray(vertexTempID);
	glDisableVertexAttribArray(uvTempID);
	//exit(EXIT_FAILURE);
}