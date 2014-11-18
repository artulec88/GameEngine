#pragma once

#include <map>
#include <string>

#include "Rendering.h"
#include "Math\Vector.h"

#include "Utility\ILogger.h"

#include "Texture.h"

namespace Rendering
{

class RENDERING_API MappedValues
{
/* ==================== Non-static member variables begin ==================== */
private:
	std::map<std::string, Math::Vector3D> vec3DMap;
	std::map<std::string, Math::Real> realMap;
	std::map<std::string, Texture*> textureMap;
/* ==================== Non-static member variables end ==================== */


/* ==================== Constructors and destructors begin ==================== */
public:
	MappedValues(void) { }
	virtual ~MappedValues(void)
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
/* ==================== Constructors and destructors end ==================== */


/* ==================== Non-static member functions begin ==================== */
public:
	inline void SetVector3D(const std::string& name, const Math::Vector3D& vec)
	{
		if (vec3DMap.find(name) == vec3DMap.end())
		{
			LOG(Utility::Debug, LOGPLACE, "Vector3D with name \"%s\" is not found in the map. Creating a new vector with this name.", name.c_str());
			vec3DMap.insert(std::pair<std::string, Math::Vector3D>(name, vec));
		}
		else
		{
#ifdef _DEBUG
			LOG(Utility::Delocust, LOGPLACE, "Modifying the Vector3D \"%s\" with values \"%s\" to vector \"%s\"", name.c_str(), vec3DMap[name].ToString().c_str(), vec.ToString().c_str());
#endif
			vec3DMap[name] = vec;
		}
	}

	inline void SetReal(const std::string& name, const Math::Real value)
	{
		if (realMap.find(name) == realMap.end())
		{
			LOG(Utility::Debug, LOGPLACE, "The real value with name \"%s\" is not found in the map. Creating a new real value with this name.", name.c_str());
			realMap.insert(std::pair<std::string, Math::Real>(name, value));
		}
		else
		{
#ifdef _DEBUG
			LOG(Utility::Delocust, LOGPLACE, "Modifying the real value with name \"%s\" from \"%.2f\" to \"%.2f\"", name.c_str(), realMap[name], value);
#endif
			realMap[name] = value;
		}
	}
	
	inline void SetTexture(const std::string& textureName, Texture* texture)
	{
		if (texture == NULL)
		{
			LOG(Utility::Warning, LOGPLACE, "Adding NULL texture with name \"%s\" to the map of textures", textureName.c_str());
		}
		if (textureMap.find(textureName) == textureMap.end())
		{
			LOG(Utility::Debug, LOGPLACE, "The texture with name \"%s\" is not found in the map. Creating a new texture with this name.", textureName.c_str());
			textureMap.insert(std::pair<std::string, Texture*>(textureName, texture));
		}
		else
		{
#ifdef _DEBUG
			LOG(Utility::Delocust, LOGPLACE, "Modifying the texture with name \"%s\".", textureName.c_str());
#endif
			textureMap[textureName] = texture;
		}
	}

	inline Math::Vector3D GetVec3D(const std::string& name) const
	{
		std::map<std::string, Math::Vector3D>::const_iterator itr = vec3DMap.find(name);
		if (itr == vec3DMap.end()) // vector not found
		{
			LOG(Utility::Warning, LOGPLACE, "Vector with name \"%s\" has not been found", name.c_str());
			return Math::Vector3D(0, 0, 0);
		}
		return itr->second;
	}

	inline Math::Real GetReal(const std::string& name) const
	{
		std::map<std::string, Math::Real>::const_iterator itr = realMap.find(name);
		if (itr == realMap.end()) // number not found
		{
			LOG(Utility::Warning, LOGPLACE, "Real number with name \"%s\" has not been found", name.c_str());
			return 0;
		}
		return itr->second;
	}

	inline Texture* GetTexture(const std::string& textureName) const
	{
		std::map<std::string, Texture*>::const_iterator itr = textureMap.find(textureName);
		if (itr == textureMap.end()) // texture not found
		{
			LOG(Utility::Warning, LOGPLACE, "Texture with name \"%s\" has not been found", textureName.c_str());
			return NULL;
		}
		return itr->second;
	}
/* ==================== Non-static member functions end ==================== */
}; /* end class MappedValues */

} /* end namespace Rendering */