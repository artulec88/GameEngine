#pragma once

#include <map>
#include <string>

#include "Rendering.h"
#include "Color.h"

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
		m_defaultValue(REAL_ZERO),
		//m_defaultColor(REAL_ONE, REAL_ONE, REAL_ONE, REAL_ONE),
		// TODO: There is no need to create new default texture for each instance of MappedValues. Instead, use Flyweight pattern to minimize dynamic memory allocation.
		m_defaultTexture(new Texture("defaultTexture.png", GL_TEXTURE_2D, GL_NEAREST, GL_RGBA, GL_RGBA, false, GL_NONE)),
		m_defaultVector2D(REAL_ZERO, REAL_ZERO),
		m_defaultVector3D(REAL_ZERO, REAL_ZERO, REAL_ZERO),
		m_defaultVector4D(REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ZERO)
	{
	}
	RENDERING_API virtual ~MappedValues(void)
	{
		std::map<std::string, const Texture*>::iterator itr = textureMap.begin();
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
	//RENDERING_API inline void SetColor(const std::string& colorName, const Color& color)
	//{
	//	if (colorMap.find(colorName) == colorMap.end())
	//	{
	//		DEBUG_LOG_RENDERING("Color with name \"",, colorName, "\" cannot be found in the map. Creating a new color with this name.");
	//		colorMap.insert(std::pair<std::string, Color>(colorName, color));
	//	}
	//	else
	//	{
	//		DELOCUST_LOG_RENDERING("Replacing the color \"", colorName, "\" with values \"", colorMap[colorName].ToString(), "\" to color \"", color.ToString(), "\"");
	//		colorMap[colorName] = color;
	//	}
	//}

	RENDERING_API inline void SetVector2D(const std::string& name, const Math::Vector2D& vec)
	{
		if (vec2DMap.find(name) == vec2DMap.end())
		{
			DEBUG_LOG_RENDERING("Vector2D with name \"", name, "\" cannot be found in the map. Creating a new vector with this name.");
			vec2DMap.insert(std::pair<std::string, Math::Vector2D>(name, vec));
		}
		else
		{
			DELOCUST_LOG_RENDERING("Modifying the Vector2D \"", name, "\" with values \"", vec2DMap[name].ToString(), "\" to vector \"", vec.ToString(), "\"");
			vec2DMap[name] = vec;
		}
	}

	RENDERING_API inline void SetVector3D(const std::string& name, const Math::Vector3D& vec)
	{
		if (vec3DMap.find(name) == vec3DMap.end())
		{
			DEBUG_LOG_RENDERING("Vector3D with name \"", name, "\" cannot be found in the map. Creating a new vector with this name.");
			vec3DMap.insert(std::pair<std::string, Math::Vector3D>(name, vec));
		}
		else
		{
			DELOCUST_LOG_RENDERING("Modifying the Vector3D \"", name, "\" with values \"", vec3DMap[name].ToString(), "\" to vector \"", vec.ToString(), "\"");
			vec3DMap[name] = vec;
		}
	}

	RENDERING_API inline void SetVector4D(const std::string& name, const Math::Vector4D& vec)
	{
		if (m_vec4DMap.find(name) == m_vec4DMap.end())
		{
			DEBUG_LOG_RENDERING("Vector 4D with name \"", name, "\" cannot be found in the map. Creating a new vector with this name.");
			m_vec4DMap.insert(std::pair<std::string, Math::Vector4D>(name, vec));
		}
		else
		{
			DELOCUST_LOG_RENDERING("Modifying the Vector4D \"", name, "\" with values \"", m_vec4DMap[name].ToString(), "\" to vector \"", vec.ToString(), "\"");
			m_vec4DMap[name] = vec;
		}
	}

	RENDERING_API inline void SetReal(const std::string& name, const Math::Real value)
	{
		if (realMap.find(name) == realMap.end())
		{
			DEBUG_LOG_RENDERING("The real value with name \"", name, "\" is not found in the map. Creating a new real value with this name.");
			realMap.insert(std::pair<std::string, Math::Real>(name, value));
		}
		else
		{
			DELOCUST_LOG_RENDERING("Modifying the real value with name \"", name, "\" from \"", realMap[name], "\" to \"", value, "\"");
			realMap[name] = value;
		}
	}
	
	RENDERING_API inline void SetTexture(const std::string& textureName, const Texture* texture)
	{
		//if (texture == NULL)
		//{
		//	WARNING_LOG_RENDERING("Adding NULL texture with name \"", textureName, "\" to the map of textures");
		//	//return;
		//}
		std::map<std::string, const Texture*>::iterator textureItr = textureMap.find(textureName);
		if (textureItr == textureMap.end())
		{
			DEBUG_LOG_RENDERING("The texture with name \"", textureName, "\" is not found in the map. Creating a new texture with this name.");
			textureMap.insert(std::pair<std::string, const Texture*>(textureName, texture));
		}
		else
		{
			DELOCUST_LOG_RENDERING("Modifying the texture with name \"", textureName, "\".");
			textureMap[textureName] = texture;
		}
		//textureMap[textureName] = texture;
	}

	RENDERING_API inline void SetMultitexture(const std::string& textureName, const Texture* texture, unsigned int textureIndex)
	{
		const std::string multitextureName = textureName + " " + std::to_string(textureIndex);
		std::map<std::string, const Texture*>::iterator textureItr = textureMap.find(multitextureName);
		if (textureItr == textureMap.end())
		{
			DEBUG_LOG_RENDERING("The multitexture with name \"", multitextureName, "\" is not found in the map. Creating a new texture with this name.");
			textureMap.insert(std::pair<std::string, const Texture*>(multitextureName, texture));
		}
		else
		{
			DELOCUST_LOG_RENDERING("Modifying the multitexture under index ", textureIndex, " with name \"", textureName, "\".");
			textureMap[multitextureName] = texture;
		}
	}

	//RENDERING_API inline const Color& GetColor(const std::string& colorName) const
	//{
	//	// TODO: Return a reference instead of value.
	//	std::map<std::string, Color>::const_iterator itr = colorMap.find(colorName);
	//	if (itr == colorMap.end()) // color not found
	//	{
	//		WARNING_LOG_RENDERING("Color with name \"", colorName, "\" has not been found. Returning default color instead.");
	//		return m_defaultColor;
	//	}
	//	return itr->second;
	//}

	RENDERING_API inline const Math::Vector2D& GetVec2D(const std::string& name) const
	{
		// TODO: Return a reference instead of value.
		std::map<std::string, Math::Vector2D>::const_iterator itr = vec2DMap.find(name);
		if (itr == vec2DMap.end()) // vector not found
		{
			WARNING_LOG_RENDERING("Vector with name \"", name, "\" has not been found. Returning default vector instead.");
			return m_defaultVector2D;
		}
		return itr->second;
	}

	RENDERING_API inline const Math::Vector3D& GetVec3D(const std::string& name) const
	{
		// TODO: Return a reference instead of value.
		std::map<std::string, Math::Vector3D>::const_iterator itr = vec3DMap.find(name);
		if (itr == vec3DMap.end()) // vector not found
		{
			WARNING_LOG_RENDERING("Vector with name \"", name, "\" has not been found. Returning default vector instead.");
			return m_defaultVector3D;
		}
		return itr->second;
	}

	RENDERING_API inline const Math::Vector4D& GetVec4D(const std::string& name) const
	{
		// TODO: Return a reference instead of value.
		std::map<std::string, Math::Vector4D>::const_iterator itr = m_vec4DMap.find(name);
		if (itr == m_vec4DMap.end()) // vector not found
		{
			WARNING_LOG_RENDERING("Vector with name \"", name, "\" has not been found. Returning default vector instead.");
			return m_defaultVector4D;
		}
		return itr->second;
	}

	RENDERING_API inline const Math::Real& GetReal(const std::string& name) const
	{
		std::map<std::string, Math::Real>::const_iterator itr = realMap.find(name);
		if (itr == realMap.end()) // number not found
		{
			WARNING_LOG_RENDERING("Real number with name \"", name, "\" has not been found");
			return m_defaultValue;
		}
		return itr->second;
	}

	RENDERING_API inline const Texture* GetTexture(const std::string& textureName) const
	{
		std::map<std::string, const Texture*>::const_iterator itr = textureMap.find(textureName);
		if (itr == textureMap.end()) // texture not found
		{
			WARNING_LOG_RENDERING("Texture with name \"", textureName, "\" has not been found. Returning default texture instead.");
			return m_defaultTexture;
		}
		return itr->second;
	}

	RENDERING_API inline const Texture* GetTexture(const std::string& textureName, unsigned int* multitextureIndex) const
	{
		std::map<std::string, const Texture*>::const_iterator itr = textureMap.find(textureName);
		if (itr == textureMap.end()) // texture not found
		{
			// The texture with the specified name has not been found. However, there is still hope- we should look for it in the multitextures.
			for (std::map<std::string, const Texture*>::const_iterator textureItr = textureMap.begin(); textureItr != textureMap.end(); ++textureItr)
			{
				const std::string& textureNameItr = textureItr->first;
				size_t spacePos = textureNameItr.find(" ");
				if (spacePos != std::string::npos)
				{
					if (textureNameItr.compare(0, textureName.length(), textureName) == 0)
					{
						// multitexture found
						(*multitextureIndex) = std::stoi(textureNameItr.substr(spacePos + 1));
						return textureItr->second;
					}
				}
			}

			WARNING_LOG_RENDERING("Texture with name \"", textureName, "\" has not been found. Returning default texture instead.");
			return m_defaultTexture;
		}
		return itr->second;
	}

#ifdef ANT_TWEAK_BAR_ENABLED
	RENDERING_API virtual void InitializeTweakBar(TwBar* tweakBar, const char* groupName);
#endif

	//bool Compare(const MappedValues& mappedValues)
	//{
	//	for (StrToRealMap::const_iterator realItr = realMap.begin(); realItr != realMap.end(); ++realItr)
	//	{
	//		Math::Real rValue = mappedValues.GetReal(realItr->first);
	//		if ((realItr->second < rValue) || (realItr->second > rValue))
	//		{
	//			return false;
	//		}
	//	}
	//	for (StrToVec3DMap::const_iterator vec3DItr = vec3DMap.begin(); vec3DItr != vec3DMap.end(); ++vec3DItr)
	//	{
	//		if (mappedValues.GetVec3D(vec3DItr->first) != vec3DItr->second)
	//		{
	//			return false;
	//		}
	//	}
	//	for (StrToVec4DMap::const_iterator vec4DItr = m_vec4DMap.begin(); vec4DItr != m_vec4DMap.end(); ++vec4DItr)
	//	{
	//		if (mappedValues.GetVec4D(vec4DItr->first) != vec4DItr->second)
	//		{
	//			return false;
	//		}
	//	}
	//	for (StrToTextureMap::const_iterator textureItr = textureMap.begin(); textureItr != textureMap.end(); ++textureItr)
	//	{
	//		if (mappedValues.GetTexture(textureItr->first) != textureItr->second)
	//		{
	//			return false;
	//		}
	//	}
	//	return true;
	//}
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	//typedef std::map<std::string, Color> StrToColorMap;
	typedef std::map<std::string, Math::Vector2D> StrToVec2DMap;
	typedef std::map<std::string, Math::Vector3D> StrToVec3DMap;
	typedef std::map<std::string, Math::Vector4D> StrToVec4DMap;
	typedef std::map<std::string, Math::Real> StrToRealMap;
	typedef std::map<std::string, const Texture*> StrToTextureMap;
	//StrToColorMap colorMap;
	StrToVec2DMap vec2DMap;
	StrToVec3DMap vec3DMap;
	StrToVec4DMap m_vec4DMap;
	StrToRealMap realMap;
	StrToTextureMap textureMap;

	Math::Real m_defaultValue;
	//Color m_defaultColor;

	// TODO: Default texture, vector3D and vector4D are all the same for all MappedValues instances so
	// there is no need to store as many instances of these as many MappedValues instances there are.
	// We should store them only once in memory. Flyweight design pattern would be a perfect solution here.
	Texture* m_defaultTexture;
	Math::Vector2D m_defaultVector2D;
	Math::Vector3D m_defaultVector3D;
	Math::Vector4D m_defaultVector4D;
/* ==================== Non-static member variables end ==================== */
}; /* end class MappedValues */

} /* end namespace Rendering */