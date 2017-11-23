#ifndef __RENDERING_MAPPED_VALUES_H__
#define __RENDERING_MAPPED_VALUES_H__

#include <map>
#include <string>

#include "Rendering.h"
#include "Color.h"

#include "Math/Vector.h"

#include "Utility/ILogger.h"

#include "Texture.h"
#ifdef ANT_TWEAK_BAR_ENABLED
#include "AntTweakBar/include/AntTweakBar.h"
#include "AntTweakBarTypes.h"
#endif

namespace rendering
{
	/// <summary>
	/// A utility class serving as a container for different kinds of objects.
	/// </summary>
	class MappedValues
	{
		// TODO: It would probably be a good idea to define pointers to values, vectors, textures instead of copies. This way, it should probably improve performance. For example,
		// the rendering engine sets (only once!) the ambient light color using the call SetVector4D("ambientLightColor", m_ambientLightColor.GetValuesPtr());. Whenever the m_ambientLightColor is changed
		// the same change can be seen in the MappedValues container, so there is no need to call SetVector4D("ambientLightColor") again.

		//using StrToColorMap = std::map<std::string, Color>;
		using StrToRealMap = std::map<std::string, math::Real>;
		using StrToVec2DMap = std::map<std::string, math::Vector2D>;
		using StrToVec3DMap = std::map<std::string, math::Vector3D>;
		using StrToVec4DMap = std::map<std::string, math::Vector4D>;
		using StrToTextureMap = std::map<std::string, const Texture*>;

		/* ==================== Static variables and functions begin ==================== */
		static constexpr math::Real DEFAULT_VALUE = REAL_ZERO;
		static constexpr math::Vector2D DEFAULT_VECTOR2D{ REAL_ZERO, REAL_ZERO };
		static constexpr math::Vector3D DEFAULT_VECTOR3D{ REAL_ZERO, REAL_ZERO, REAL_ZERO };
		static constexpr math::Vector4D DEFAULT_VECTOR4D{ REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ZERO };
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Mapped values container constructor. </summary>
		MappedValues();

		/// <summary> Mapped values container destructor. </summary>
		~MappedValues();

		/// <summary>
		/// Mapped values container copy constructor.
		/// </summary>
		/// <param name="mappedValues"> The reference to mapped values container to copy construct from. </param>
		MappedValues(const MappedValues& mappedValues) = delete;

		/// <summary>
		/// Mapped values container move constructor.
		/// </summary>
		/// <param name="mappedValues"> The r-value reference to mapped values container to move construct from. </param>
		MappedValues(MappedValues&& mappedValues) = delete;

		/// <summary>
		/// Mapped values container copy assignment operator.
		/// </summary>
		/// <param name="mappedValues"> The reference to mapped values container to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned mapped values container. </returns>
		MappedValues& operator=(const MappedValues& mappedValues) = delete;

		/// <summary>
		/// Mapped values container move assignment operator.
		/// </summary>
		/// <param name="mappedValues"> The r-value reference to mapped values container to move assign from. </param>
		/// <returns> The reference to the newly move-assigned mapped values container. </returns>
		MappedValues& operator=(MappedValues&& mappedValues) = delete;
		/* ==================== Constructors and destructors end ==================== */


		/* ==================== Non-static member functions begin ==================== */
		void SetReal(const std::string& name, math::Real value)
		{
			if (realMap.find(name) == realMap.end())
			{
				DEBUG_LOG_RENDERING("The real value with name \"", name, "\" is not found in the map. Creating a new real value with this name.");
				realMap.insert(std::pair<std::string, math::Real>(name, value));
			}
			else
			{
				DELOCUST_LOG_RENDERING("Modifying the real value with name \"", name, "\" from \"", realMap[name], "\" to \"", value, "\"");
				realMap[name] = value;
			}
		}

		void SetVector2D(const std::string& name, const math::Vector2D& vec)
		{
			if (vec2DMap.find(name) == vec2DMap.end())
			{
				DEBUG_LOG_RENDERING("Vector2D with name \"", name, "\" cannot be found in the map. Creating a new vector with this name.");
				vec2DMap.insert(std::pair<std::string, math::Vector2D>(name, vec));
			}
			else
			{
				DELOCUST_LOG_RENDERING("Modifying the Vector2D \"", name, "\" with values \"", vec2DMap[name], "\" to vector \"", vec, "\"");
				vec2DMap[name] = vec;
			}
		}

		void SetVector3D(const std::string& name, const math::Vector3D& vec)
		{
			if (vec3DMap.find(name) == vec3DMap.end())
			{
				DEBUG_LOG_RENDERING("Vector3D with name \"", name, "\" cannot be found in the map. Creating a new vector with this name.");
				vec3DMap.insert(std::pair<std::string, math::Vector3D>(name, vec));
			}
			else
			{
				DELOCUST_LOG_RENDERING("Modifying the Vector3D \"", name, "\" with values \"", vec3DMap[name], "\" to vector \"", vec, "\"");
				vec3DMap[name] = vec;
			}
		}

		void SetVector4D(const std::string& name, const math::Vector4D& vec)
		{
			if (m_vec4DMap.find(name) == m_vec4DMap.end())
			{
				DEBUG_LOG_RENDERING("Vector 4D with name \"", name, "\" cannot be found in the map. Creating a new vector with this name.");
				m_vec4DMap.insert(std::pair<std::string, math::Vector4D>(name, vec));
			}
			else
			{
				DELOCUST_LOG_RENDERING("Modifying the Vector4D \"", name, "\" with values \"", m_vec4DMap[name], "\" to vector \"", vec, "\"");
				m_vec4DMap[name] = vec;
			}
		}

		//void SetColor(const std::string& name, const Color& color)
		//{
		//	if (m_vec4DMap.find(name) == m_vec4DMap.end())
		//	{
		//		DEBUG_LOG_RENDERING("Color with name \"", name, "\" cannot be found in the map. Creating a new color with this name.");
		//		m_vec4DMap.insert(std::pair<std::string, math::Vector4D>(name, color.GetValues()));
		//	}
		//	else
		//	{
		//		DELOCUST_LOG_RENDERING("Modifying the color \"", name, "\" with values \"", m_vec4DMap[name], "\" to color \"", color, "\"");
		//		m_vec4DMap[name] = color.GetValues();
		//	}
		//}

		void SetTexture(const std::string& textureName, const Texture* texture)
		{
			//if (texture == nullptr)
			//{
			//	WARNING_LOG_RENDERING("Adding NULL texture with name \"", textureName, "\" to the map of textures");
			//	//return;
			//}
			const auto textureItr = textureMap.find(textureName);
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

		void SetMultitexture(const std::string& textureName, const Texture* texture, unsigned int textureIndex)
		{
			const std::string multitextureName = textureName + " " + std::to_string(textureIndex);
			const auto textureItr = textureMap.find(multitextureName);
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

		const math::Real& GetReal(const std::string& name) const
		{
			const auto itr = realMap.find(name);
			if (itr == realMap.end()) // number not found
			{
				WARNING_LOG_RENDERING("Real number with name \"", name, "\" has not been found");
				return DEFAULT_VALUE;
			}
			return itr->second;
		}

		const math::Vector2D& GetVec2D(const std::string& name) const
		{
			// TODO: Return a reference instead of value.
			const auto itr = vec2DMap.find(name);
			if (itr == vec2DMap.end()) // vector not found
			{
				WARNING_LOG_RENDERING("Vector2D with name \"", name, "\" has not been found. Returning default vector instead.");
				return DEFAULT_VECTOR2D;
			}
			return itr->second;
		}

		const math::Vector3D& GetVec3D(const std::string& name) const
		{
			// TODO: Return a reference instead of value.
			const auto itr = vec3DMap.find(name);
			if (itr == vec3DMap.end()) // vector not found
			{
				WARNING_LOG_RENDERING("Vector3D with name \"", name, "\" has not been found. Returning default vector instead.");
				return DEFAULT_VECTOR3D;
			}
			return itr->second;
		}

		const math::Vector4D& GetVec4D(const std::string& name) const
		{
			// TODO: Return a reference instead of value.
			const auto itr = m_vec4DMap.find(name);
			if (itr == m_vec4DMap.end()) // vector not found
			{
				WARNING_LOG_RENDERING("Vector4D with name \"", name, "\" has not been found. Returning default vector instead.");
				return DEFAULT_VECTOR4D;
			}
			return itr->second;
		}

		const Texture* GetTexture(const std::string& textureName) const
		{
			const auto itr = textureMap.find(textureName);
			if (itr == textureMap.end()) // texture not found
			{
				WARNING_LOG_RENDERING("Texture with name \"", textureName, "\" has not been found. Returning default texture instead.");
				return textureMap.begin()->second; // TODO: Temporary. Instead we should return some default texture.
			}
			return itr->second;
		}

		const Texture* GetTexture(const std::string& textureName, unsigned int* multitextureIndex) const
		{
			const auto itr = textureMap.find(textureName);
			if (itr == textureMap.end()) // texture not found
			{
				// The texture with the specified name has not been found. However, there is still hope- we should look for it in the multitextures.
				for (auto textureItr = textureMap.begin(); textureItr != textureMap.end(); ++textureItr)
				{
					const std::string& textureNameItr = textureItr->first;
					const auto spacePos = textureNameItr.find(" ");
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
				return textureMap.begin()->second; // TODO: Temporary. Instead we should return some default texture.
			}
			return itr->second;
		}

#ifdef ANT_TWEAK_BAR_ENABLED
		void InitializeTweakBar(TwBar* tweakBar, const char* groupName);
#endif

		//bool Compare(const MappedValues& mappedValues)
		//{
		//	for (StrToRealMap::const_iterator realItr = realMap.begin(); realItr != realMap.end(); ++realItr)
		//	{
		//		math::Real rValue = mappedValues.GetReal(realItr->first);
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
		//StrToColorMap colorMap;
		StrToRealMap realMap;
		StrToVec2DMap vec2DMap;
		StrToVec3DMap vec3DMap;
		StrToVec4DMap m_vec4DMap;
		StrToTextureMap textureMap;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class MappedValues */

} /* end namespace rendering */

#endif /* __RENDERING_MAPPED_VALUES_H__ */