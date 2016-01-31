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
		m_hasMultipleTextures(false)
{
	if (diffuseTexture == NULL)
	{
		ERROR_LOG("The material is given the NULL diffuse texture");
	}
	SetTexture("diffuse", diffuseTexture);
	SetReal("specularIntensity", specularIntensity);
	SetReal("specularPower", specularPower);
	if (normalMap == NULL)
	{
		DEBUG_LOG("The material is not using any normal maps");
		//const std::string defaultNormalMapStr = GET_CONFIG_VALUE_STR("defaultNormalMap", "defaultNormalMap.jpg");
		const std::string defaultNormalMapStr = "defaultNormalMap.jpg";
		normalMap = new Texture(defaultNormalMapStr);
		//INFO_LOG("Adding default normal map (\"%s\") to the material", defaultNormalMapStr.c_str());
	}
	SetTexture("normalMap", normalMap);

	if (displacementMap == NULL)
	{
		DEBUG_LOG("The material is not using any displacement maps");
		displacementMap = new Texture("defaultDisplacementMap.jpg");
		//INFO_LOG("Adding displacement map to the material");
	}
	SetTexture("displacementMap", displacementMap);

	Math::Real baseBias = displacementScale / static_cast<Math::Real>(2.0f); /* TODO: Don't use hardcoded values! Ever! */
	SetReal("displacementScale", displacementScale);
	SetReal("displacementBias", -baseBias + baseBias * displacementOffset);
}

Material::Material(Texture* texture, const std::string& textureName)
{
	if (texture == NULL)
	{
		ERROR_LOG("The material is given the NULL main texture");
	}
	SetTexture(textureName, texture);
}

void Material::SetAdditionalTexture(Texture* texture, const std::string& textureName)
{
	CHECK_CONDITION_RETURN_VOID(texture != NULL, Utility::Warning, "Cannot set the additional texture for material. The texture is NULL.");
	m_hasMultipleTextures = true;
	SetTexture(textureName, texture);
}