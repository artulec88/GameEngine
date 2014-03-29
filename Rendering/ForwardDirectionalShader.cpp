#include "StdAfx.h"
#include "ForwardDirectionalShader.h"
#include "DirectionalLight.h"
#include "Utility\Log.h"

using namespace Rendering;
using namespace Utility;

/* static */ ForwardDirectionalShader* ForwardDirectionalShader::GetInstance()
{
	static ForwardDirectionalShader shader;
	return &shader;
}

ForwardDirectionalShader::ForwardDirectionalShader(void) :
	Shader()
{
	AddVertexShaderFromFile("C:\\Users\\Artur\\Documents\\Visual Studio 2010\\Projects\\GameEngine\\Shaders\\ForwardDirectional.vshader");
	AddFragmentShaderFromFile("C:\\Users\\Artur\\Documents\\Visual Studio 2010\\Projects\\GameEngine\\Shaders\\ForwardDirectional.fshader");

	// Must use it to bind attributes, but only if earlier versions of the GL.
	//glBindAttribLocation(program, 0, "...");
	//glBindAttribLocation(program, 1, "...");

	if (! Compile())
	{
		stdlog(Error, LOGPLACE, "Error while compiling ForwardDirectionalShader shader");
	}
	AddUniform("model");
	AddUniform("MVP");
	AddUniform("specularIntensity");
	AddUniform("specularPower");
	AddUniform("eyePos");
	AddUniform("directionalLight.base.color");
	AddUniform("directionalLight.base.intensity");
	AddUniform("directionalLight.direction");
}


ForwardDirectionalShader::~ForwardDirectionalShader(void)
{
}

void ForwardDirectionalShader::UpdateUniforms(const Transform& transform, const Material& material, Renderer* renderer)
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

	Math::Matrix4D worldMatrix = transform.GetTransformation();
	Math::Matrix4D projectedMatrix = renderer->GetCurrentCamera().GetViewProjection() * worldMatrix;

	SetUniform("model", worldMatrix);
	SetUniform("MVP", projectedMatrix);
	SetUniformf("specularIntensity", material.GetSpecularIntensity());
	SetUniformf("specularPower", material.GetSpecularPower());
	SetUniform("eyePos", renderer->GetCurrentCamera().GetTransform().GetPos());

	DirectionalLight* directionalLight = dynamic_cast<DirectionalLight*>(renderer->GetCurrentLight());
	if (directionalLight == NULL)
	{
		stdlog(Error, LOGPLACE, "Directional light is NULL");
		exit(EXIT_FAILURE);
	}

	SetUniform("directionalLight.base.color", directionalLight->GetColor());
	SetUniformf("directionalLight.base.intensity", directionalLight->GetIntensity());
	SetUniform("directionalLight.direction", directionalLight->GetDirection());
}