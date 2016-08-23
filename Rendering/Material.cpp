#include "StdAfx.h"
#include "Material.h"

Rendering::Material::Material(Texture* diffuseTexture,
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
		ERROR_LOG_RENDERING("The material is given the NULL diffuse texture");
	}
	m_mappedValues.SetTexture("diffuse", diffuseTexture);
	m_mappedValues.SetReal("specularIntensity", specularIntensity);
	m_mappedValues.SetReal("specularPower", specularPower);
	if (normalMap == NULL)
	{
		DEBUG_LOG_RENDERING("The material is not using any normal maps");
		normalMap = new Texture("defaultNormalMap.jpg" /* TODO: Replace with texture from the TextureFactory */);
		//INFO_LOG_RENDERING("Adding default normal map (\"", defaultNormalMapStr, "\") to the material");
	}
	m_mappedValues.SetTexture("normalMap", normalMap);

	if (displacementMap == NULL)
	{
		DEBUG_LOG_RENDERING("The material is not using any displacement maps");
		displacementMap = new Texture("defaultDisplacementMap.jpg" /* TODO: Replace with texture from the TextureFactory */);
		//INFO_LOG_RENDERING("Adding displacement map to the material");
	}
	m_mappedValues.SetTexture("displacementMap", displacementMap);

	Math::Real baseBias = displacementScale / static_cast<Math::Real>(2.0f); /* TODO: Don't use hardcoded values! Ever! */
	m_mappedValues.SetReal("displacementScale", displacementScale);
	m_mappedValues.SetReal("displacementBias", -baseBias + baseBias * displacementOffset);
}

Rendering::Material::~Material()
{
}

Rendering::Material::Material(Texture* texture, const std::string& textureName)
{
	if (texture == NULL)
	{
		ERROR_LOG_RENDERING("The material is given the NULL main texture");
	}
	m_mappedValues.SetTexture(textureName, texture);
}

void Rendering::Material::SetAdditionalTexture(Texture* texture, const std::string& textureName)
{
	CHECK_CONDITION_RETURN_VOID_RENDERING(texture != NULL, Utility::Logging::WARNING, "Cannot set the additional texture for material. The texture is NULL.");
	m_hasMultipleTextures = true;
	m_mappedValues.SetTexture(textureName, texture);
}

#ifdef ANT_TWEAK_BAR_ENABLED
void Rendering::Material::InitializeTweakBar(TwBar* tweakBar, const char* groupName)
{
	m_mappedValues.InitializeTweakBar(tweakBar, groupName);
}
#endif