#ifndef __RENDERING_SHADER_FACTORY_H__
#define __RENDERING_SHADER_FACTORY_H__

#include "Rendering.h"
#include "Shader.h"
#include "ShaderIDs.h"

#include "Utility\ILogger.h"

#include <map>

namespace Rendering
{
	class ShaderFactory
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary>The shader factory constructor.</summary>
		RENDERING_API ShaderFactory(const std::string& shadersDirectory);

		/// <summary>The destructor.</summary>
		RENDERING_API ~ShaderFactory(void);

		/// <summary> The shader factory copy constructor. </summary>
		ShaderFactory(const ShaderFactory& shaderFactory) = delete;
		/// <summary> The shader factory move constructor. </summary>
		ShaderFactory(ShaderFactory&& shaderFactory) = delete;
		/// <summary> The shader factory copy assignment operator. </summary>
		ShaderFactory& operator=(ShaderFactory& shaderFactory) = delete;
		/// <summary> The shader factory move assignment operator. </summary>
		ShaderFactory& operator=(ShaderFactory&& shaderFactory) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		RENDERING_API void CreateShader(int shaderID, const std::string& shaderFileName);
		RENDERING_API inline const Shader* GetShader(int shaderID) const
		{
			CHECK_CONDITION_EXIT_RENDERING(m_shaderType2ShaderMap.find(shaderID) != m_shaderType2ShaderMap.end(), Utility::Logging::EMERGENCY,
				"No shader has been created for the specified type of shader (", shaderID, ").");
			return &m_shaderType2ShaderMap.find(shaderID)->second;
		}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		const std::string& m_shadersDirectory;

		/// <summary>
		/// The map storing all shader by their unique ID.
		/// </summary>
		std::map<int, Rendering::Shader> m_shaderType2ShaderMap;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class ShaderFactory */

} /* end namespace Rendering */

#endif /* __RENDERING_SHADER_FACTORY_H__ */