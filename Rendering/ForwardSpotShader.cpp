#include "StdAfx.h"
#include "ForwardSpotShader.h"
#include "SpotLight.h"
#include "Utility\Log.h"

using namespace Rendering;
using namespace Utility;

/* static */ ForwardSpotShader* ForwardSpotShader::GetInstance()
{
	static ForwardSpotShader shader;
	return &shader;
}

ForwardSpotShader::ForwardSpotShader(void) :
	Shader()
{
	AddVertexShaderFromFile("C:\\Users\\Artur\\Documents\\Visual Studio 2010\\Projects\\GameEngine\\Shaders\\ForwardSpot.vshader");
	AddFragmentShaderFromFile("C:\\Users\\Artur\\Documents\\Visual Studio 2010\\Projects\\GameEngine\\Shaders\\ForwardSpot.fshader");

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
	AddUniform("spotLight.pointLight.base.color");
	AddUniform("spotLight.pointLight.base.intensity");
	AddUniform("spotLight.pointLight.attenuation.constant");
	AddUniform("spotLight.pointLight.attenuation.linear");
	AddUniform("spotLight.pointLight.attenuation.exponent");
	AddUniform("spotLight.pointLight.position");
	AddUniform("spotLight.pointLight.range");
	AddUniform("spotLight.direction");
	AddUniform("spotLight.cutoff");
}


ForwardSpotShader::~ForwardSpotShader(void)
{
}

void ForwardSpotShader::UpdateUniforms(const Transform& transform, const Material& material, Renderer* renderer)
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

	SpotLight* spotLight = dynamic_cast<SpotLight*>(renderer->GetCurrentLight());
	if (spotLight == NULL)
	{
		stdlog(Error, LOGPLACE, "Spot light is NULL");
		exit(EXIT_FAILURE);
	}

	AddUniform("spotLight.pointLight.base.color");
	AddUniform("spotLight.pointLight.base.intensity");
	AddUniform("spotLight.pointLight.attenuation.constant");
	AddUniform("spotLight.pointLight.attenuation.linear");
	AddUniform("spotLight.pointLight.attenuation.exponent");
	AddUniform("spotLight.pointLight.position");
	AddUniform("spotLight.pointLight.range");
	AddUniform("spotLight.direction");
	AddUniform("spotLight.cutoff");

	SetUniform("spotLight.pointLight.base.color", spotLight->GetColor());
	SetUniformf("spotLight.pointLight.base.intensity", spotLight->GetIntensity());
	SetUniformf("spotLight.pointLight.attenuation.constant", spotLight->GetAttenuation().GetConstant());
	SetUniformf("spotLight.pointLight.attenuation.linear", spotLight->GetAttenuation().GetLinear());
	SetUniformf("spotLight.pointLight.attenuation.exponent", spotLight->GetAttenuation().GetExponent());
	SetUniform("spotLight.pointLight.position", spotLight->GetTransform().GetPos());
	SetUniformf("spotLight.pointLight.range", spotLight->GetRange());
	SetUniform("spotLight.direction", spotLight->GetTransform().GetRot().GetForward());
	SetUniformf("spotLight.cutoff", spotLight->GetCutoff());
}