#ifndef __RENDERING_SHADER_FACTORY_H__
#define __RENDERING_SHADER_FACTORY_H__

#include "Rendering.h"
#include "Shader.h"

#include "Utility/ILogger.h"

#include <map>

namespace rendering
{
	class ShaderFactory
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary>The shader factory constructor.</summary>
		/// <param name="shadersDirectory"> Shaders directory. </param>
		RENDERING_API explicit ShaderFactory(const std::string& shadersDirectory);

		/// <summary>The destructor.</summary>
		RENDERING_API ~ShaderFactory();

		/// <summary> The shader factory copy constructor. </summary>
		/// <param name="shaderFactory"> The reference to shader factory to copy construct from. </param>
		ShaderFactory(const ShaderFactory& shaderFactory) = delete;

		/// <summary> The shader factory move constructor. </summary>
		/// <param name="shaderFactory"> The r-value reference to shader factory to move construct from. </param>
		ShaderFactory(ShaderFactory&& shaderFactory) = delete;

		/// <summary> The shader factory copy assignment operator. </summary>
		/// <param name="shaderFactory"> The reference to shader factory to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned shader factory. </returns>
		ShaderFactory& operator=(ShaderFactory& shaderFactory) = delete;

		/// <summary> The shader factory move assignment operator. </summary>
		/// <param name="shaderFactory"> The r-value reference to shader factory to move assign from. </param>
		/// <returns> The reference to the newly move-assigned shader factory. </returns>
		ShaderFactory& operator=(ShaderFactory&& shaderFactory) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		RENDERING_API const Shader* CreateShader(int shaderId, const std::string& shaderFileName);
		RENDERING_API const Shader* GetShader(const int shaderId) const
		{
			CHECK_CONDITION_EXIT_RENDERING(m_shaderType2ShaderMap.find(shaderId) != m_shaderType2ShaderMap.end(), Utility::Logging::EMERGENCY,
				"No shader has been created for the specified type of shader (", shaderId, ").");
			return &m_shaderType2ShaderMap.find(shaderId)->second;
		}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		const std::string& m_shadersDirectory;

		/// <summary>
		/// The map storing all shader by their unique ID.
		/// </summary>
		std::map<int, Shader> m_shaderType2ShaderMap;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class ShaderFactory */

} /* end namespace rendering */

#endif /* __RENDERING_SHADER_FACTORY_H__ */