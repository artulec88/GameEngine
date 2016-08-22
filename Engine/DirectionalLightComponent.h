#ifndef __ENGINE_DIRECTIONAL_LIGHT_COMPONENT_H__
#define __ENGINE_DIRECTIONAL_LIGHT_COMPONENT_H__

#include "Engine.h"
#include "GameComponent.h"

#include "Rendering\DirectionalLight.h"
#include "Rendering\Color.h"

#define SIMULATE_SUN_BEHAVIOR // TODO: Move all #define's to one place

#ifdef SIMULATE_SUN_BEHAVIOR
#include "IUpdateable.h"
#endif

#include "Math\Angle.h"

namespace Engine
{
	class DirectionalLightComponent : public GameComponent
#ifdef SIMULATE_SUN_BEHAVIOR
		, public IUpdateable
#endif
	{
		/* ==================== Static variables and functions begin ==================== */
	private:
		static const Math::Real TROPIC_OF_CANCER_SINUS;
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		DirectionalLightComponent(Rendering::Lighting::DirectionalLight* directionalLight, Math::Real maxIntensity, const Rendering::Color& sunDaytimeColor,
			const Rendering::Color& sunNearHorizonColor, const Rendering::Color& sunNighttimeColor, const Math::Angle& latitude, const Math::Angle& longitude,
			const Math::Angle& firstElevationLevel, const Math::Angle& secondElevationLevel, const Math::Angle& thirdElevationLevel);
		virtual ~DirectionalLightComponent();
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		//virtual Math::Transform& GetTransform() { return GameComponent::GetTransform(); }
		//virtual const Math::Transform& GetTransform() const { return GameComponent::GetTransform(); }

#ifdef SIMULATE_SUN_BEHAVIOR
		virtual void Update(Math::Real delta);
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
		Rendering::Lighting::DirectionalLight* m_directionalLight;

		///<summary>The maximum intensity of the directional light.</summary>
		CONST_IF_TWEAK_BAR_DISABLED Math::Real m_maxIntensity;

		///<summary>The daytime color of the directional light.</summary>
		CONST_IF_TWEAK_BAR_DISABLED Rendering::Color m_sunDaytimeColor;

		///<summary>The near-horizon color of the directional light.</summary>
		CONST_IF_TWEAK_BAR_DISABLED Rendering::Color m_sunNearHorizonColor;

		///<summary>The nighttime color of the directional light.</summary>
		CONST_IF_TWEAK_BAR_DISABLED Rendering::Color m_sunNighttimeColor;

		CONST_IF_TWEAK_BAR_DISABLED Math::Angle m_latitude;
		CONST_IF_TWEAK_BAR_DISABLED Math::Angle m_longitude;

		Math::Angle m_sunElevation;
		Math::Angle m_sunAzimuth;

		CONST_IF_TWEAK_BAR_DISABLED Math::Angle m_firstElevationLevel;
		CONST_IF_TWEAK_BAR_DISABLED Math::Angle m_secondElevationLevel;
		CONST_IF_TWEAK_BAR_DISABLED Math::Angle m_thirdElevationLevel;
		/* ==================== Non-static member variables end ==================== */
	}; // end class DirectionalLightComponent
} // end namespace Engine

#endif // __ENGINE_DIRECTIONAL_LIGHT_COMPONENT_H__

