#include "StdAfx.h"
#include "BasicShader.h"
#include "Texture.h"
#include "Utility\Log.h"

using namespace Rendering;
using namespace Utility;

BasicShader::BasicShader(void) :
	Shader()
{
	// TODO: I ended up making BasicShader a singleton. That way, you don't load the same shader program onto the graphics card multiple times
	AddVertexShaderFromFile("C:\\Users\\Artur\\Documents\\Visual Studio 2010\\Projects\\GameEngine\\Shaders\\TextureVertexShader.vshader");
	AddFragmentShaderFromFile("C:\\Users\\Artur\\Documents\\Visual Studio 2010\\Projects\\GameEngine\\Shaders\\TextureFragmentShader.fshader");
	if (! Compile())
	{
		stdlog(Error, LOGPLACE, "Error while compiling shader");
	}
	AddUniform("transform");
	AddUniform("color");
}


BasicShader::~BasicShader(void)
{
}


// derived from Shader class
void BasicShader::UpdateUniforms(const Math::Matrix4D& worldMatrix, const Math::Matrix4D& projectedMatrix, const Material& material)
{
	if (material.texture != NULL)
	{
		material.texture->Bind();
	}
	else
	{
		static unsigned char whitePixel[] = {0xFF, 0xFF, 0xFF, 0xFF};
		static Texture WHITE(1, 1, whitePixel);
		WHITE.Bind();
	}

	SetUniform("transform", projectedMatrix);
	SetUniform("color", material.color);
}