#include "StdAfx.h"
#include "Material.h"

using namespace Rendering;

Material::Material(Texture* diffuseTexture,
	Math::Real specularIntensity /* = 2 */,
	Math::Real specularPower /* = 32 */,
	Texture* normalMap /* = NULL */)
{
	if (diffuseTexture == NULL)
	{
		stdlog(Utility::Error, LOGPLACE, "The material is given the NULL diffuse texture");
	}
	AddTexture("diffuse", diffuseTexture);
	AddReal("specularIntensity", specularIntensity);
	AddReal("specularPower", specularPower);
	if (normalMap != NULL)
	{
		stdlog(Utility::Info, LOGPLACE, "Adding normal map to the material");
		AddTexture("normalMap", normalMap);
	}
	else /* (normalMap == NULL) */
	{
		stdlog(Utility::Debug, LOGPLACE, "The material is not using any normal maps");
	}
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