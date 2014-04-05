#include "StdAfx.h"
#include "ForwardPointShader.h"
#include "PointLight.h"
#include "Utility\Log.h"

using namespace Rendering;
using namespace Utility;

/* static */ ForwardPointShader* ForwardPointShader::GetInstance()
{
	static ForwardPointShader shader;
	return &shader;
}

ForwardPointShader::ForwardPointShader(void) :
	Shader()
{
	AddVertexShaderFromFile("..\\Shaders\\ForwardPoint.vshader");
	AddFragmentShaderFromFile("..\\Shaders\\ForwardPoint.fshader");

	// Must use it to bind attributes, but only if earlier versions of the GL.
	//glBindAttribLocation(program, 0, "...");
	//glBindAttribLocation(program, 1, "...");

	if (! Compile())
	{
		stdlog(Error, LOGPLACE, "Error while compiling ForwardPointShader shader");
	}
	AddUniform("model");
	AddUniform("MVP");
	AddUniform("specularIntensity");
	AddUniform("specularPower");
	AddUniform("eyePos");
	AddUniform("pointLight.base.color");
	AddUniform("pointLight.base.intensity");
	AddUniform("pointLight.attenuation.constant");
	AddUniform("pointLight.attenuation.linear");
	AddUniform("pointLight.attenuation.exponent");
	AddUniform("pointLight.position");
	AddUniform("pointLight.range");
}


ForwardPointShader::~ForwardPointShader(void)
{
}

void ForwardPointShader::UpdateUniforms(const Transform& transform, const Material& material, Renderer* renderer)
{
	Texture* diffuseTexture = material.GetDiffuseTexture();
	if (diffuseTexture != NULL)
	{
		diffuseTexture->Bind();
	}
	else
	{
		stdlog(Warning, LOGPLACE, "Diffuse texture is not available");
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
	SetUniform("eyePos", renderer->GetCurrentCamera().GetTransform().GetTransformedPos());

	PointLight* pointLight = dynamic_cast<PointLight*>(renderer->GetCurrentLight());
	if (pointLight == NULL)
	{
		stdlog(Error, LOGPLACE, "Point light is NULL");
		exit(EXIT_FAILURE);
	}

	SetUniform("pointLight.base.color", pointLight->GetColor());
	SetUniformf("pointLight.base.intensity", pointLight->GetIntensity());
	SetUniformf("pointLight.attenuation.constant", pointLight->GetAttenuation().GetConstant());
	SetUniformf("pointLight.attenuation.linear", pointLight->GetAttenuation().GetLinear());
	SetUniformf("pointLight.attenuation.exponent", pointLight->GetAttenuation().GetExponent());
	SetUniform("pointLight.position", pointLight->GetTransform().GetTransformedPos());
	SetUniformf("pointLight.range", pointLight->GetRange());
}