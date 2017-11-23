#ifndef __ENGINE_LIGHT_FACTORY_H__
#define __ENGINE_LIGHT_FACTORY_H__

#include "Engine.h"
#include "Rendering/DirectionalLight.h"
#include "Rendering/PointLight.h"
#include "Rendering/SpotLight.h"

#include "Utility/ILogger.h"

namespace engine
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
		/// <summary>The light factory constructor.</summary>
		LightFactory();

		/// <summary>The light factory destructor.</summary>
		~LightFactory();

		/// <summary> Light factory copy constructor. </summary>
		/// <param name="lightFactory"> The light factory to copy construct from. </param>
		LightFactory(const LightFactory& lightFactory) = delete;

		/// <summary> Light factory move constructor. </summary>
		/// <param name="lightFactory"> The light factory to move construct from. </param>
		LightFactory(LightFactory&& lightFactory) = delete;

		/// <summary> Light factory copy assignment operator. </summary>
		/// <param name="lightFactory"> The light factory to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned light factory. </returns>
		LightFactory& operator=(LightFactory& lightFactory) = delete;

		/// <summary> Light factory move assignment operator. </summary>
		/// <param name="lightFactory"> The light factory to move assign from. </param>
		/// <returns> The reference to the newly move-assigned light factory. </returns>
		LightFactory& operator=(LightFactory&& lightFactory) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		const rendering::lighting::BaseLight* CreateLight(LightTypes::LightType lightType, int lightID /* TODO: Add parameter that declares the file name (XML?) that contains information about the light */);
		ENGINE_API const rendering::lighting::BaseLight& GetLight(LightTypes::LightType lightType, int lightId) const
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
				return m_spotLights[lightId];
			}
		}
	private:
		const rendering::lighting::DirectionalLight* AddDirectionalLight(int lightID /* TODO: Add parameter that declares the file name (XML?) that contains information about the light */);
		const rendering::lighting::PointLight* AddPointLight(int lightID /* TODO: Add parameter that declares the file name (XML?) that contains information about the light */);
		const rendering::lighting::SpotLight* AddSpotLight(int lightID /* TODO: Add parameter that declares the file name (XML?) that contains information about the light */);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		/// <summary>
		/// The vector storing all directional lights at their unique IDs.
		/// </summary>
		std::vector<rendering::lighting::DirectionalLight> m_directionalLights;

		/// <summary>
		/// The vector storing all point lights at their unique IDs.
		/// </summary>
		std::vector<rendering::lighting::PointLight> m_pointLights;

		/// <summary>
		/// The vector storing all spot lights at their unique IDs.
		/// </summary>
		std::vector<rendering::lighting::SpotLight> m_spotLights;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class LightFactory */

} /* end namespace engine */

#endif /* __ENGINE_LIGHT_FACTORY_H__ */