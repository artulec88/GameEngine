#pragma once

#include <map>
#include <string>

#include "Rendering.h"
#include "Math\Vector.h"

#include "Utility\ILogger.h"

#include "Texture.h"
#ifdef ANT_TWEAK_BAR_ENABLED
#include "AntTweakBar\include\AntTweakBar.h"
#include "AntTweakBarTypes.h"
#endif

namespace Rendering
{

class MappedValues
{
/* ==================== Constructors and destructors begin ==================== */
public:
	RENDERING_API MappedValues(void) :
		defaultTexture(new Texture("defaultTexture.png", GL_TEXTURE_2D, GL_NEAREST, GL_RGBA, GL_RGBA, false, GL_NONE)),
		defaultVector3D(Math::Vector3D(REAL_ZERO, REAL_ZERO, REAL_ZERO)) {}
	RENDERING_API virtual ~MappedValues(void)
	{
		std::map<std::string, Texture*>::iterator itr = textureMap.begin();
		while (itr != textureMap.end())
		{
			if (itr->second != NULL) // if texture is not NULL
			{
				SAFE_DELETE(itr->second);
			}
			++itr;
		}
	}
/* ==================== Constructors and destructors end ==================== */


/* ==================== Non-static member functions begin ==================== */
public:
	RENDERING_API inline void SetVector3D(const std::string& name, const Math::Vector3D& vec)
	{
		if (vec3DMap.find(name) == vec3DMap.end())
		{
			DEBUG_LOG("Vector3D with name \"%s\" is not found in the map. Creating a new vector with this name.", name.c_str());
			vec3DMap.insert(std::pair<std::string, Math::Vector3D>(name, vec));
		}
		else
		{
			DELOCUST_LOG("Modifying the Vector3D \"%s\" with values \"%s\" to vector \"%s\"", name.c_str(), vec3DMap[name].ToString().c_str(), vec.ToString().c_str());
			vec3DMap[name] = vec;
		}
	}

	RENDERING_API inline void SetReal(const std::string& name, const Math::Real value)
	{
		if (realMap.find(name) == realMap.end())
		{
			DEBUG_LOG("The real value with name \"%s\" is not found in the map. Creating a new real value with this name.", name.c_str());
			realMap.insert(std::pair<std::string, Math::Real>(name, value));
		}
		else
		{
			DELOCUST_LOG("Modifying the real value with name \"%s\" from \"%.2f\" to \"%.2f\"", name.c_str(), realMap[name], value);
			realMap[name] = value;
		}
	}
	
	RENDERING_API inline void SetTexture(const std::string& textureName, Texture* texture)
	{
		//if (texture == NULL)
		//{
		//	WARNING_LOG("Adding NULL texture with name \"%s\" to the map of textures", textureName.c_str());
		//	//return;
		//}
		std::map<std::string, Texture*>::iterator textureItr = textureMap.find(textureName);
		if (textureItr == textureMap.end())
		{
			DEBUG_LOG("The texture with name \"%s\" is not found in the map. Creating a new texture with this name.", textureName.c_str());
			textureMap.insert(std::pair<std::string, Texture*>(textureName, texture));
		}
		else
		{
			DELOCUST_LOG("Modifying the texture with name \"%s\".", textureName.c_str());
			textureMap[textureName] = texture;
		}
		//textureMap[textureName] = texture;
	}

	RENDERING_API inline const Math::Vector3D GetVec3D(const std::string& name) const
	{
		std::map<std::string, Math::Vector3D>::const_iterator itr = vec3DMap.find(name);
		if (itr == vec3DMap.end()) // vector not found
		{
			DEBUG_LOG("Vector with name \"%s\" has not been found. Returning default vector instead.", name.c_str());
			return defaultVector3D;
		}
		return itr->second;
	}

	RENDERING_API inline Math::Real GetReal(const std::string& name) const
	{
		std::map<std::string, Math::Real>::const_iterator itr = realMap.find(name);
		if (itr == realMap.end()) // number not found
		{
			DEBUG_LOG("Real number with name \"%s\" has not been found", name.c_str());
			return REAL_ZERO;
		}
		return itr->second;
	}

	RENDERING_API inline Texture* GetTexture(const std::string& textureName) const
	{
		std::map<std::string, Texture*>::const_iterator itr = textureMap.find(textureName);
		if (itr == textureMap.end()) // texture not found
		{
			DEBUG_LOG("Texture with name \"%s\" has not been found. Returning default texture instead.", textureName.c_str());
			return defaultTexture;
		}
		return itr->second;
	}

#ifdef ANT_TWEAK_BAR_ENABLED
	RENDERING_API virtual void InitializeTweakBar(TwBar* tweakBar, const char* groupName);
#endif
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	typedef std::map<std::string, Math::Vector3D> StrToVec3DMap;
	typedef std::map<std::string, Math::Real> StrToRealMap;
	typedef std::map<std::string, Texture*> StrToTextureMap;
	StrToVec3DMap vec3DMap;
	StrToRealMap realMap;
	StrToTextureMap textureMap;
	Texture* defaultTexture;
	Math::Vector3D defaultVector3D;
/* ==================== Non-static member variables end ==================== */
}; /* end class MappedValues */

} /* end namespace Rendering */