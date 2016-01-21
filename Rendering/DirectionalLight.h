#pragma once

#include "Rendering.h"
#include "BaseLight.h"
#include "Transform.h"
#include "Math\Angle.h"

#define SIMULATE_SUN_BEHAVIOR // TODO: Move all #define's to one place

namespace Rendering { namespace Lighting
{

/// <summary>
/// The directional light representation.
/// </summary>
class DirectionalLight : public BaseLight
{
/* ==================== Static variables and functions begin ==================== */
private:
	static bool directionalLightsEnabled;
public:
	static bool* GetDirectionalLightsEnabled() { return &directionalLightsEnabled; }
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	DirectionalLight(const Color& color = Color(REAL_ONE, REAL_ONE, REAL_ONE, REAL_ONE), Math::Real intensity = REAL_ONE,
		Math::Real halfShadowArea = REAL_ONE, int shadowMapSizeAsPowerOf2 = 0, Math::Real shadowSoftness = REAL_ONE,
		Math::Real lightBleedingReductionAmount = static_cast<Math::Real>(0.2f),
		Math::Real minVariance = static_cast<Math::Real>(0.00002f));
	virtual ~DirectionalLight(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	//Math::Vector3D GetDirection() const { return GetTransform().GetTransformedRot().GetForward(); }

	//virtual void InitializeShaders();
	virtual bool IsEnabled() const;
	virtual ShadowCameraTransform CalcShadowCameraTransform(const Math::Vector3D& cameraPos, const Math::Quaternion& cameraRot);

	void SetMaxIntensity(Math::Real maxIntensity) { m_maxIntensity = maxIntensity; }
	void SetSunlightDaytimeColor(const Color& sunlightDaytimeColor) { m_sunlightDaytimeColor = sunlightDaytimeColor; }
	void SetSunNearHorizonColor(const Color& sunNearHorizonColor) { m_sunNearHorizonColor = sunNearHorizonColor; }
	void SetSunlightNighttimeColor(const Color& sunlightNighttimeColor) { m_sunlightNighttimeColor = sunlightNighttimeColor; }

#ifdef SIMULATE_SUN_BEHAVIOR
	virtual void Update(Math::Real delta);
#endif
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	///<summary>The maximum intensity of the directional light.</summary>
	Math::Real m_maxIntensity;

	///<summary>The daytime color of the directional light.</summary>
	Color m_sunlightDaytimeColor;

	///<summary>The near-horizon color of the directional light.</summary>
	Color m_sunNearHorizonColor;

	///<summary>The nighttime color of the directional light.</summary>
	Color m_sunlightNighttimeColor;
	Math::Real m_halfShadowArea;
/* ==================== Non-static member variables end ==================== */
}; /* end class DirectionalLight */

} /* end namespace Lighting */

} /* end namespace Rendering */
