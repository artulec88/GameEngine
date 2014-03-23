#include "StdAfx.h"
#include "PhongShader.h"
#include "Texture.h"
#include "Transform.h"
#include "Utility\Log.h"

using namespace Rendering;
using namespace Utility;

/* static */ Math::Vector3D PhongShader::ambientLight = Math::Vector3D(0.1, 0.1, 0.1);
/* static */ DirectionalLight PhongShader::directionalLight(Math::Vector3D(1.0, 1.0, 1.0), 0.0, Math::Vector3D(0.0, 0.0, 0.0));

/* static */ Math::Vector3D PhongShader::GetAmbientLight()
{
	return PhongShader::ambientLight;
}

/* static */ void PhongShader::SetAmbientLight(const Math::Vector3D& ambientLight)
{
	PhongShader::ambientLight = ambientLight;
}

/* static */ DirectionalLight PhongShader::GetDirectionalLight()
{
	return PhongShader::directionalLight;
}

/* static */ void PhongShader::SetDirectionalLight(const DirectionalLight& directionalLight)
{
	PhongShader::directionalLight = directionalLight;
}

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
	AddUniform("projectedTransform");
	AddUniform("baseColor");
	AddUniform("ambientLight");
	AddUniform("specularIntensity");
	AddUniform("specularPower");
	AddUniform("eyePos");
	AddUniform("directionalLight.base.color");
	AddUniform("directionalLight.base.intensity");
	AddUniform("directionalLight.direction");
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

	SetUniform("transform", worldMatrix);
	SetUniform("projectedTransform", projectedMatrix);
	SetUniform("baseColor", material.color);
	SetUniform("ambientLight", PhongShader::GetAmbientLight());
	SetUniformLight("directionalLight", directionalLight);

	SetUniformf("specularIntensity", material.GetSpecularIntensity());
	SetUniformf("specularPower", material.GetSpecularPower());

	SetUniform("eyePos", Transform::GetCamera().GetPos());
}

void PhongShader::SetUniformLight(const std::string& uniform, const DirectionalLight& directionalLight)
{
	SetUniform(uniform + ".base.color", directionalLight.GetColor());
	SetUniformf(uniform + ".base.intensity", directionalLight.GetIntensity());
	SetUniform(uniform + ".direction", directionalLight.GetDirection());
}