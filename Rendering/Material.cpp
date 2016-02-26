#include "StdAfx.h"
#include "Material.h"

using namespace Rendering;

Material::Material(Texture* diffuseTexture,
	Math::Real specularIntensity /* = REAL_ONE */,
	Math::Real specularPower /* = 8.0f */,
	Texture* normalMap /* = NULL */,
	Texture* displacementMap /* = NULL */,
	Math::Real displacementScale /* = REAL_ZERO */,
	Math::Real displacementOffset /* = REAL_ZERO */) :
		m_mappedValues(),
		m_hasMultipleTextures(false)
{
	if (diffuseTexture == NULL)
	{
		ERROR_LOG("The material is given the NULL diffuse texture");
	}
	m_mappedValues.SetTexture("diffuse", diffuseTexture);
	m_mappedValues.SetReal("specularIntensity", specularIntensity);
	m_mappedValues.SetReal("specularPower", specularPower);
	if (normalMap == NULL)
	{
		DEBUG_LOG("The material is not using any normal maps");
		//const std::string defaultNormalMapStr = GET_CONFIG_VALUE_STR("defaultNormalMap", "defaultNormalMap.jpg");
		const std::string defaultNormalMapStr = "defaultNormalMap.jpg";
		normalMap = new Texture(defaultNormalMapStr);
		//INFO_LOG("Adding default normal map (\"%s\") to the material", defaultNormalMapStr.c_str());
	}
	m_mappedValues.SetTexture("normalMap", normalMap);

	if (displacementMap == NULL)
	{
		DEBUG_LOG("The material is not using any displacement maps");
		displacementMap = new Texture("defaultDisplacementMap.jpg");
		//INFO_LOG("Adding displacement map to the material");
	}
	m_mappedValues.SetTexture("displacementMap", displacementMap);

	Math::Real baseBias = displacementScale / static_cast<Math::Real>(2.0f); /* TODO: Don't use hardcoded values! Ever! */
	m_mappedValues.SetReal("displacementScale", displacementScale);
	m_mappedValues.SetReal("displacementBias", -baseBias + baseBias * displacementOffset);
}

Material::Material(Texture* texture, const std::string& textureName)
{
	if (texture == NULL)
	{
		ERROR_LOG("The material is given the NULL main texture");
	}
	m_mappedValues.SetTexture(textureName, texture);
}

void Material::SetAdditionalTexture(Texture* texture, const std::string& textureName)
{
	CHECK_CONDITION_RETURN_VOID(texture != NULL, Utility::Warning, "Cannot set the additional texture for material. The texture is NULL.");
	m_hasMultipleTextures = true;
	m_mappedValues.SetTexture(textureName, texture);
}

#ifdef ANT_TWEAK_BAR_ENABLED
void Material::InitializeTweakBar(TwBar* tweakBar, const char* groupName)
{
	m_mappedValues.InitializeTweakBar(tweakBar, groupName);
}
#endif