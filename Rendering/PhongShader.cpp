#include "StdAfx.h"
#include "PhongShader.h"
#include "Texture.h"
#include "Utility\Log.h"

using namespace Rendering;
using namespace Utility;

PhongShader::PhongShader(void) :
	Shader()
{
	// TODO: I ended up making BasicShader a singleton. That way, you don't load the same shader program onto the graphics card multiple times
	AddVertexShaderFromFile("C:\\Users\\Artur\\Documents\\Visual Studio 2010\\Projects\\GameEngine\\Shaders\\PhongVertexShader.vshader");
	AddFragmentShaderFromFile("C:\\Users\\Artur\\Documents\\Visual Studio 2010\\Projects\\GameEngine\\Shaders\\PhongFragmentShader.fshader");
	if (! Compile())
	{
		stdlog(Error, LOGPLACE, "Error while compiling shader");
	}
	AddUniform("transform");
	AddUniform("color");
}


PhongShader::~PhongShader(void)
{
}


// derived from Shader class
void PhongShader::UpdateUniforms(const Math::Matrix4D& worldMatrix, const Math::Matrix4D& projectedMatrix, const Material& material)
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