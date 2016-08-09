#ifndef __RENDERING_SHADER_FACTORY_H__
#define __RENDERING_SHADER_FACTORY_H__

#include "Rendering.h"
#include "Shader.h"

#include "Utility\ILogger.h"

#include <map>

namespace Rendering
{
	namespace ShaderTypes
	{
		enum ShaderType
		{
			AMBIENT = 0,
			AMBIENT_TERRAIN,
			AMBIENT_FOG_LINEAR_PLANE_BASED,
			AMBIENT_FOG_EXPONENTIAL_PLANE_BASED,
			AMBIENT_FOG_LINEAR_RANGE_BASED,
			AMBIENT_FOG_EXPONENTIAL_RANGE_BASED,
			AMBIENT_TERRAIN_FOG_LINEAR_PLANE_BASED,
			AMBIENT_TERRAIN_FOG_EXPONENTIAL_PLANE_BASED,
			AMBIENT_TERRAIN_FOG_LINEAR_RANGE_BASED,
			AMBIENT_TERRAIN_FOG_EXPONENTIAL_RANGE_BASED,
			SHADOW_MAP,
			SHADOW_MAP_CUBE,
			FILTER_NULL,
			FILTER_GAUSSIAN_BLUR,
			FILTER_FXAA,
			SKYBOX,
			SKYBOX_PROCEDURAL,
			TEXT_SIMPLE,
			TEXT,
			WATER,
			WATER_NO_DIRECTIONAL_LIGHT,
			BILLBOARD,
			PARTICLES,
			GUI,
			DIRECTIONAL_LIGHT,
			DIRECTIONAL_LIGHT_TERRAIN,
			DIRECTIONAL_LIGHT_NO_SHADOWS,
			DIRECTIONAL_LIGHT_TERRAIN_NO_SHADOWS,
			POINT_LIGHT,
			POINT_LIGHT_TERRAIN,
			POINT_LIGHT_NO_SHADOWS,
			POINT_LIGHT_TERRAIN_NO_SHADOWS,
			SPOT_LIGHT,
			SPOT_LIGHT_TERRAIN,
			SPOT_LIGHT_NO_SHADOWS,
			SPOT_LIGHT_TERRAIN_NO_SHADOWS,
			// TODO: Add more shader types
			COUNT
		}; /* end enum ShaderType */
	} /* end namespace ShaderTypes */

	class ShaderFactory
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary>The shader factory constructor.</summary>
		ShaderFactory();

		/// <summary>The destructor.</summary>
		~ShaderFactory(void);

		ShaderFactory(const ShaderFactory& shaderFactory) = delete; // Copy constructor
		ShaderFactory(ShaderFactory&& shaderFactory) = delete; // Move constructor
		void operator=(ShaderFactory& shaderFactory) = delete; // Copy assignment operator
		void operator=(ShaderFactory&& shaderFactory) = delete; // Move assignment operator
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		//void CreateShader(ShaderTypes::ShaderType shaderType, const std::string& shaderFileName);
		RENDERING_API inline const Shader& GetShader(ShaderTypes::ShaderType shaderType) const
		{
			std::map<ShaderTypes::ShaderType, Shader>::const_iterator shaderItr = m_shaderType2ShaderMap.find(shaderType);
			if (shaderItr == m_shaderType2ShaderMap.end())
			{
				ERROR_LOG_RENDERING("No shader has been created for the specified type of shader (", shaderType, ").");
			}
			return shaderItr->second;
		}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		/// <summary>
		/// The vector storing all shader by their types.
		/// </summary>
		std::map<ShaderTypes::ShaderType, Shader> m_shaderType2ShaderMap;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class ShaderFactory */

} /* end namespace Rendering */

#endif /* __RENDERING_SHADER_FACTORY_H__ */