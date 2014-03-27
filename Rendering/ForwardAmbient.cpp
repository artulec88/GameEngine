#include "StdAfx.h"
#include "ForwardAmbient.h"

#include "Utility\Log.h"

using namespace Rendering;
using namespace Utility;

/* static */ ForwardAmbientShader* ForwardAmbientShader::GetInstance()
{
	static ForwardAmbientShader shader;
	return &shader;
}

ForwardAmbientShader::ForwardAmbientShader(void) :
	Shader()
{
	// TODO: I ended up making BasicShader a singleton. That way, you don't load the same shader program onto the graphics card multiple times
	AddVertexShaderFromFile("C:\\Users\\Artur\\Documents\\Visual Studio 2010\\Projects\\GameEngine\\Shaders\\ForwardAmbient.vshader");
	AddFragmentShaderFromFile("C:\\Users\\Artur\\Documents\\Visual Studio 2010\\Projects\\GameEngine\\Shaders\\ForwardAmbient.fshader");

	// Must use it to bind attributes, but only if earlier versions of the GL.
	//glBindAttribLocation(program, 0, "");
	//glBindAttribLocation(program, 1, "");

	if (! Compile())
	{
		stdlog(Error, LOGPLACE, "Error while compiling ForwardAmbientShader shader");
	}
	AddUniform("MVP");
	AddUniform("ambientIntensity");
}


ForwardAmbientShader::~ForwardAmbientShader(void)
{
}

void ForwardAmbientShader::UpdateUniforms(const Transform& transform, const Material& material, Renderer* renderer)
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

	Math::Matrix4D cameraViewProjection = renderer->GetCurrentCamera().GetViewProjection();

	Math::Matrix4D projectedMatrix = cameraViewProjection * transform.GetTransformation();

	SetUniform("MVP", projectedMatrix);
	SetUniform("ambientIntensity", renderer->GetAmbientLight());
}