#pragma once

#include "Rendering.h"
#include "BaseLight.h"
#include "Transform.h"
#include "Math\Angle.h"

#define SIMULATE_SUN_BEHAVIOR

namespace Rendering
{

class RENDERING_API DirectionalLight : public BaseLight
{
/* ==================== Static variables and functions begin ==================== */
private:
	static bool directionalLightsEnabled;
public:
	static bool* GetDirectionalLightsEnabled() { return &directionalLightsEnabled; }
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	DirectionalLight(const Color& color = Color(REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ONE), Math::Real intensity = REAL_ZERO,
		int shadowMapSizeAsPowerOf2 = 0, Math::Real shadowArea = 80.0f, Math::Real shadowSoftness = REAL_ONE,
		Math::Real lightBleedingReductionAmount = static_cast<Math::Real>(0.2f),
		Math::Real minVariance = static_cast<Math::Real>(0.00002f) );
	virtual ~DirectionalLight(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	//Math::Vector3D GetDirection() const { return GetTransform().GetTransformedRot().GetForward(); }
	virtual bool IsEnabled() const;
	virtual ShadowCameraTransform CalcShadowCameraTransform(const Math::Vector3D& cameraPos, const Math::Quaternion& cameraRot);

#ifdef SIMULATE_SUN_BEHAVIOR
	virtual void Update(Math::Real delta);
#endif
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	const Math::Real m_maxIntensity;
	const Color m_sunlightDaytimeColor;
	const Color m_sunNearHorizonColor;
	const Color m_sunlightNighttimeColor;
	Math::Real m_halfShadowArea;
/* ==================== Non-static member variables end ==================== */
}; /* end class DirectionalLight */

} /* end namespace Rendering */