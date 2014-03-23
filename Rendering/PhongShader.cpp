#include "StdAfx.h"
#include "PhongShader.h"
#include "Texture.h"
#include "Transform.h"
#include "Utility\Log.h"
#include <sstream>

using namespace Rendering;
using namespace Utility;

/* static */ Math::Vector3D PhongShader::ambientLight = Math::Vector3D(0.1, 0.1, 0.1);
/* static */ DirectionalLight PhongShader::directionalLight(Math::Vector3D(1.0, 1.0, 1.0), 0.0, Math::Vector3D(0.0, 0.0, 0.0));
/* static */ int PhongShader::pointLightCount = 0;
/* static */ PointLight* PhongShader::pointLights = NULL;

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

/* static */ void PhongShader::SetPointLights(PointLight* pointLights, int arraySize)
{
	ASSERT(arraySize > 0);
	if (arraySize > PhongShader::MAX_POINT_LIGHTS)
	{
		stdlog(Error, LOGPLACE, "Passed in too many point lights. Max number allowed is %d, whereas you passed in %d", MAX_POINT_LIGHTS, arraySize);
		exit(EXIT_FAILURE);
	}

	//for (int i = 0; i < MAX_POINT_LIGHTS; ++i)
	//{
	//	PhongShader::pointLights[i] = pointLights[i];
	//}
	PhongShader::pointLightCount = arraySize;
	PhongShader::pointLights = pointLights;

	ASSERT(PhongShader::pointLights != NULL);
	if (PhongShader::pointLights == NULL)
	{
		stdlog(Error, LOGPLACE, "PhongShader::pointLights have not been successfully initialized");
		exit(EXIT_FAILURE);
	}
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

	if (PhongShader::pointLights == NULL)
	{
		stdlog(Info, LOGPLACE, "No point lights available.");
		return;
	}
	// TODO: Shouldn't MAX_POINT_LIGHTS be changed to PhongShader::pointLightCount?
	for (int i = 0; i < MAX_POINT_LIGHTS; ++i)
	{
		std::stringstream ss("");
		ss << "pointLights[" << i << "].";
		std::string pointLightUniformName = ss.str();
		AddUniform(pointLightUniformName + "base.color");
		AddUniform(pointLightUniformName + "base.intensity");
		AddUniform(pointLightUniformName + "attenuation.constant");
		AddUniform(pointLightUniformName + "attenuation.linear");
		AddUniform(pointLightUniformName + "attenuation.exponent");
		AddUniform(pointLightUniformName + "position");
	}
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
	SetUniform("directionalLight.base.color", directionalLight.GetColor());
	SetUniformf("directionalLight.base.intensity", directionalLight.GetIntensity());
	SetUniform("directionalLight.direction", directionalLight.GetDirection());
	
	if (PhongShader::pointLights != NULL)
	{
		// TODO: Shouldn't MAX_POINT_LIGHTS be changed to PhongShader::pointLightCount?
		for(int i = 0; i < MAX_POINT_LIGHTS; ++i)
		{
			std::stringstream ss("");
			ss << "pointLights[" << i << "]";
			std::string pointLightName = ss.str();
			//std::string pointLightName = "pointLights[" + std::to_string(i) + "]";

			Attenuation attenuation = PhongShader::pointLights[i].GetAttenuation();

			SetUniform(pointLightName + ".base.color", PhongShader::pointLights[i].GetColor());
			SetUniformf(pointLightName + ".base.intensity", PhongShader::pointLights[i].GetIntensity());
			SetUniformf(pointLightName + ".attenuation.constant", attenuation.GetConstant());
			SetUniformf(pointLightName + ".attenuation.linear", attenuation.GetLinear());
			SetUniformf(pointLightName + ".attenuation.exponent", attenuation.GetExponent());
			SetUniform(pointLightName + ".position", PhongShader::pointLights[i].GetPosition());
		}
	}
	else
	{
		stdlog(Warning, LOGPLACE, "Point lights are unavailable");
	}

	SetUniformf("specularIntensity", material.GetSpecularIntensity());
	SetUniformf("specularPower", material.GetSpecularPower());

	SetUniform("eyePos", Transform::GetCamera().GetPos());
}