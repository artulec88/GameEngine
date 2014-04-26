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
		stdlog(Utility::Error, LOGPLACE, "The material is given the NULL diffuse texture");
	}
	AddTexture("diffuse", diffuseTexture);
	AddReal("specularIntensity", specularIntensity);
	AddReal("specularPower", specularPower);
	if (normalMap == NULL)
	{
		stdlog(Utility::Debug, LOGPLACE, "The material is not using any normal maps");
		normalMap = new Texture("..\\Textures\\defaultNormalMap.jpg");
		//stdlog(Utility::Info, LOGPLACE, "Adding normal map to the material");
	}
	ASSERT(normalMap != NULL);
	AddTexture("normalMap", normalMap);

	if (displacementMap == NULL)
	{
		stdlog(Utility::Debug, LOGPLACE, "The material is not using any displacement maps");
		displacementMap = new Texture("..\\Textures\\defaultDisplacementMap.jpg");
		//stdlog(Utility::Info, LOGPLACE, "Adding displacement map to the material");
	}
	ASSERT(displacementMap != NULL);
	AddTexture("displacementMap", displacementMap);

	Math::Real baseBias = displacementScale / static_cast<Math::Real>(2.0f);
	AddReal("displacementScale", displacementScale);
	AddReal("displacementBias", -baseBias + baseBias * displacementOffset);
}


Material::~Material(void)
{
	// TODO: Deallocate textures
	std::map<std::string, Texture*>::iterator itr = textureMap.begin();
	while (itr != textureMap.end())
	{
		if (itr->second != NULL) // if texture is not NULL
		{
			delete itr->second;
			itr->second = NULL;
		}
		++itr;
	}
}

void Material::AddTexture(const std::string& textureName, Texture* texture)
{
	if (texture == NULL)
	{
		stdlog(Utility::Warning, LOGPLACE, "Adding NULL texture with name \"%s\" to the map of textures", textureName.c_str());
	}
	textureMap.insert(std::pair<std::string, Texture*>(textureName, texture));
}

Texture* Material::GetTexture(const std::string& textureName) const
{
	std::map<std::string, Texture*>::const_iterator itr = textureMap.find(textureName);
	if (itr == textureMap.end()) // texture not found
	{
		stdlog(Utility::Warning, LOGPLACE, "Texture with name \"%s\" has not been found", textureName.c_str());
		return NULL;
	}
	return itr->second;
}