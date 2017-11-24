#ifndef __ENGINE_DIRECTIONAL_LIGHT_COMPONENT_H__
#define __ENGINE_DIRECTIONAL_LIGHT_COMPONENT_H__

#include "Engine.h"
#include "GameComponent.h"

#include "Rendering/DirectionalLight.h"
#include "Rendering\Color.h"

#define SIMULATE_SUN_BEHAVIOR // TODO: Move all #define's to one place

#ifdef SIMULATE_SUN_BEHAVIOR
#include "IUpdateable.h"
#endif

#include "Math/Angle.h"

namespace engine
{
	class DirectionalLightComponent : public GameComponent
#ifdef SIMULATE_SUN_BEHAVIOR
		, public IUpdateable
#endif
	{
		/* ==================== Static variables and functions begin ==================== */
	private:
		static constexpr math::Real TROPIC_OF_CANCER_SINUS = 0.39794863131f;
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		DirectionalLightComponent(rendering::lighting::DirectionalLight* directionalLight, math::Real maxIntensity, const rendering::Color& sunDaytimeColor,
			const rendering::Color& sunNearHorizonColor, const rendering::Color& sunNighttimeColor, const math::Angle& latitude, const math::Angle& longitude,
			const math::Angle& firstElevationLevel, const math::Angle& secondElevationLevel, const math::Angle& thirdElevationLevel);
		virtual ~DirectionalLightComponent();
		DirectionalLightComponent(const DirectionalLightComponent& directionalLightComponent) = delete;
		DirectionalLightComponent(DirectionalLightComponent&& directionalLightComponent);
		DirectionalLightComponent& operator=(const DirectionalLightComponent& directionalLightComponent) = delete;
		DirectionalLightComponent& operator=(DirectionalLightComponent&& directionalLightComponent);
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		//virtual math::Transform& GetTransform() { return GameComponent::GetTransform(); }
		//virtual const math::Transform& GetTransform() const { return GameComponent::GetTransform(); }

#ifdef SIMULATE_SUN_BEHAVIOR
		virtual void Update(math::Real delta);
#endif
	private:
		/// <summary>
		/// Calculates the elevation and azimuth angles for the directional light (sun).
		/// </summary>
		/// <remarks>
		/// See http://pveducation.org/pvcdrom/properties-of-sunlight/sun-position-calculator for details.
		/// </remarks>
		void CalculateSunElevationAndAzimuth();
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		rendering::lighting::DirectionalLight* m_directionalLight;

		///<summary>The maximum intensity of the directional light.</summary>
		CONST_IF_TWEAK_BAR_DISABLED math::Real m_maxIntensity;

		///<summary>The daytime color of the directional light.</summary>
		CONST_IF_TWEAK_BAR_DISABLED rendering::Color m_sunDaytimeColor;

		///<summary>The near-horizon color of the directional light.</summary>
		CONST_IF_TWEAK_BAR_DISABLED rendering::Color m_sunNearHorizonColor;

		///<summary>The nighttime color of the directional light.</summary>
		CONST_IF_TWEAK_BAR_DISABLED rendering::Color m_sunNighttimeColor;

		CONST_IF_TWEAK_BAR_DISABLED math::Angle m_latitude;
		CONST_IF_TWEAK_BAR_DISABLED math::Angle m_longitude;

		math::Angle m_sunElevation;
		math::Angle m_sunAzimuth;

		CONST_IF_TWEAK_BAR_DISABLED math::Angle m_firstElevationLevel;
		CONST_IF_TWEAK_BAR_DISABLED math::Angle m_secondElevationLevel;
		CONST_IF_TWEAK_BAR_DISABLED math::Angle m_thirdElevationLevel;
		/* ==================== Non-static member variables end ==================== */
	}; // end class DirectionalLightComponent
} // end namespace Engine

#endif // __ENGINE_DIRECTIONAL_LIGHT_COMPONENT_H__

