#include "stdafx.h"
#include "LightFactory.h"

#include "Utility\BuilderDirector.h"

engine::LightFactory::LightFactory() :
	m_directionalLights(),
	m_pointLights(),
	m_spotLights()
{
}


engine::LightFactory::~LightFactory()
{
}

const rendering::lighting::BaseLight* engine::LightFactory::CreateLight(LightTypes::LightType lightType, int lightID)
{
	switch (lightType)
	{
	case LightTypes::DIRECTIONAL:
		return AddDirectionalLight(lightID);
	case LightTypes::POINT:
		return AddPointLight(lightID);
	case LightTypes::SPOT:
		return AddSpotLight(lightID);
	default:
		ERROR_LOG_ENGINE("Cannot create the light. Incorrect light type (", lightType, ") has been specified.");
	}
	return NULL;
}

const rendering::lighting::DirectionalLight* engine::LightFactory::AddDirectionalLight(int lightID /* TODO: Add parameter that declares the file name (XML?) that contains information about the light */)
{
	// TODO: This code needs to be fixed.


	//Engine::DirectionalLightBuilder directionalLightBuilder(m_shaderFactory);
	//Utility::BuilderDirector<Rendering::Lighting::DirectionalLight> lightBuilderDirector(directionalLightBuilder);
	//lightBuilderDirector.Construct();
	//if (directionalLightBuilder.Get() != NULL)
	//{
	//	INFO_LOG_RENDERING("Directional light with intensity = ", directionalLightBuilder.Get()->GetIntensity(), " is being added to directional / spot lights vector");
	//	m_directionalLights[lightID] = *directionalLightBuilder.Get();
	//}

	return NULL;
}

const rendering::lighting::PointLight* engine::LightFactory::AddPointLight(int lightID /* TODO: Add parameter that declares the file name (XML?) that contains information about the light */)
{
	return NULL;
}

const rendering::lighting::SpotLight* engine::LightFactory::AddSpotLight(int lightID /* TODO: Add parameter that declares the file name (XML?) that contains information about the light */)
{
	return NULL;
}