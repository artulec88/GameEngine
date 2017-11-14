#include "StdAfx.h"
#include "Material.h"

Rendering::Material::Material(const Texture* diffuseTexture, Math::Real specularIntensity, Math::Real specularPower, const Texture* normalMap, const Texture* displacementMap, Math::Real displacementScale /* = REAL_ZERO */, Math::Real displacementOffset /* = REAL_ZERO */) :
	m_mappedValues(),
	m_hasMultipleTextures(false)
{
	DELOCUST_LOG_RENDERING("Material construction has started.");
	CHECK_CONDITION_EXIT_ALWAYS_RENDERING(diffuseTexture != nullptr, utility::logging::ERR, "The material is given the NULL diffuse texture");
	m_mappedValues.SetTexture("diffuse", diffuseTexture);
	m_mappedValues.SetReal("specularIntensity", specularIntensity);
	m_mappedValues.SetReal("specularPower", specularPower);
	CHECK_CONDITION_EXIT_ALWAYS_RENDERING(normalMap != nullptr, utility::logging::EMERGENCY, "Cannot create a material. No normal map used.");
	m_mappedValues.SetTexture("normalMap", normalMap);
	CHECK_CONDITION_EXIT_ALWAYS_RENDERING(displacementMap != nullptr, utility::logging::EMERGENCY, "Cannot create a material. No displacement map used.");
	m_mappedValues.SetTexture("displacementMap", displacementMap);

	Math::Real baseBias = displacementScale / static_cast<Math::Real>(2.0f); /* TODO: Don't use hardcoded values! Ever! */
	m_mappedValues.SetReal("displacementScale", displacementScale);
	m_mappedValues.SetReal("displacementBias", -baseBias + baseBias * displacementOffset);
	DEBUG_LOG_RENDERING("Material has been created.");
}

Rendering::Material::Material(const Texture* texture, const std::string& textureName)
{
	DEBUG_LOG_RENDERING("Material construction has started.");
	CHECK_CONDITION_EXIT_ALWAYS_RENDERING(texture != nullptr, utility::logging::ERR, "The material is given the NULL main texture");
	m_mappedValues.SetTexture(textureName, texture);
	DEBUG_LOG_RENDERING("Material has been created.");
}

Rendering::Material::~Material()
{
}

void Rendering::Material::SetAdditionalTexture(const Texture* texture, const std::string& textureName)
{
	CHECK_CONDITION_RETURN_VOID_ALWAYS_RENDERING(texture != nullptr, utility::logging::WARNING,
		"Cannot set the additional texture for material. The texture is NULL.");
	m_hasMultipleTextures = true;
	m_mappedValues.SetTexture(textureName, texture);
}

#ifdef ANT_TWEAK_BAR_ENABLED
void Rendering::Material::InitializeTweakBar(TwBar* tweakBar, const char* groupName)
{
	m_mappedValues.InitializeTweakBar(tweakBar, groupName);
}
#endif