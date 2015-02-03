#include "StdAfx.h"
#include "Material.h"

using namespace Rendering;

Material::Material(Texture* diffuseTexture,
	Math::Real specularIntensity /* = REAL_ONE */,
	Math::Real specularPower /* = 8.0f */,
	Texture* normalMap /* = NULL */,
	Texture* displacementMap /* = NULL */,
	Math::Real displacementScale /* = REAL_ZERO */,
	Math::Real displacementOffset /* = REAL_ZERO */)
{
	if (diffuseTexture == NULL)
	{
		LOG(Utility::Error, LOGPLACE, "The material is given the NULL diffuse texture");
	}
	SetTexture("diffuse", diffuseTexture);
	SetReal("specularIntensity", specularIntensity);
	SetReal("specularPower", specularPower);
	if (normalMap == NULL)
	{
		LOG(Utility::Debug, LOGPLACE, "The material is not using any normal maps");
		normalMap = new Texture("..\\Textures\\defaultNormalMap.jpg");
		//LOG(Utility::Info, LOGPLACE, "Adding normal map to the material");
	}
	ASSERT(normalMap != NULL);
	SetTexture("normalMap", normalMap);

	if (displacementMap == NULL)
	{
		LOG(Utility::Debug, LOGPLACE, "The material is not using any displacement maps");
		displacementMap = new Texture("..\\Textures\\defaultDisplacementMap.jpg");
		//LOG(Utility::Info, "Adding displacement map to the material");
	}
	ASSERT(displacementMap != NULL);
	SetTexture("displacementMap", displacementMap);

	Math::Real baseBias = displacementScale / static_cast<Math::Real>(2.0f);
	SetReal("displacementScale", displacementScale);
	SetReal("displacementBias", -baseBias + baseBias * displacementOffset);
}

void Material::SetAdditionalTexture(Texture* texture)
{
	if (texture == NULL)
	{
		LOG(Utility::Warning, LOGPLACE, "Cannot set the additional texture for material. The texture is NULL.");
		return;
	}
	SetTexture("diffuse2", texture);
}