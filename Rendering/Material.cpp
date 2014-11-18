#include "StdAfx.h"
#include "Material.h"

using namespace Rendering;

Material::Material(Texture* diffuseTexture,
	Math::Real specularIntensity /* = 2 */,
	Math::Real specularPower /* = 32 */,
	Texture* normalMap /* = NULL */,
	Texture* displacementMap /* = NULL */,
	Math::Real displacementScale /* = 0.0f */,
	Math::Real displacementOffset /* = 0.0f */)
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