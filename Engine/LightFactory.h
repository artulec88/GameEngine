#ifndef __ENGINE_LIGHT_FACTORY_H__
#define __ENGINE_LIGHT_FACTORY_H__

#include "Engine.h"
#include "Rendering\DirectionalLight.h"
#include "Rendering\PointLight.h"
#include "Rendering\SpotLight.h"

#include "Utility\ILogger.h"

#include <map>

namespace Engine
{
	namespace LightTypes
	{
		enum LightType
		{
			//AMBIENT = 0,
			DIRECTIONAL = 0,
			POINT,
			SPOT,
			COUNT
		}; /* end enum LightType */
	} /* end namespace LightTypes */

	class LightFactory
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary>The shader factory constructor.</summary>
		LightFactory();

		/// <summary>The destructor.</summary>
		~LightFactory(void);

		/// <summary>
		/// Light factory copy constructor.
		/// </summary>
		LightFactory(const LightFactory& lightFactory) = delete;
		/// <summary>
		/// Light factory move constructor.
		/// </summary>
		LightFactory(LightFactory&& lightFactory) = delete;
		/// <summary>
		/// Light factory copy assignment operator.
		/// </summary>
		void operator=(LightFactory& lightFactory) = delete;
		/// <summary>
		/// Light factory move assignment operator.
		/// </summary>
		void operator=(LightFactory&& lightFactory) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		const Rendering::Lighting::BaseLight* CreateLight(LightTypes::LightType lightType, int lightID /* TODO: Add parameter that declares the file name (XML?) that contains information about the light */);
		ENGINE_API inline const Rendering::Lighting::BaseLight& GetLight(LightTypes::LightType lightType, int lightId) const
		{
			// TODO: Check if lightId is a correct value.
			switch (lightType)
			{
			case LightTypes::DIRECTIONAL:
				return m_directionalLights[lightId];
			case LightTypes::POINT:
				return m_pointLights[lightId];
			case LightTypes::SPOT:
				return m_spotLights[lightId];
			default:
				ERROR_LOG_ENGINE("Cannot return the light. Incorrect light type (", lightType, ") has been specified.");
			}
		}
	private:
		const Rendering::Lighting::DirectionalLight* AddDirectionalLight(int lightID /* TODO: Add parameter that declares the file name (XML?) that contains information about the light */);
		const Rendering::Lighting::PointLight* AddPointLight(int lightID /* TODO: Add parameter that declares the file name (XML?) that contains information about the light */);
		const Rendering::Lighting::SpotLight* AddSpotLight(int lightID /* TODO: Add parameter that declares the file name (XML?) that contains information about the light */);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		/// <summary>
		/// The vector storing all directional lights at their unique IDs.
		/// </summary>
		std::vector<Rendering::Lighting::DirectionalLight> m_directionalLights;

		/// <summary>
		/// The vector storing all point lights at their unique IDs.
		/// </summary>
		std::vector<Rendering::Lighting::PointLight> m_pointLights;

		/// <summary>
		/// The vector storing all spot lights at their unique IDs.
		/// </summary>
		std::vector<Rendering::Lighting::SpotLight> m_spotLights;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class LightFactory */

} /* end namespace Engine */

#endif /* __ENGINE_LIGHT_FACTORY_H__ */