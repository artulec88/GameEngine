#pragma once

#include "Rendering.h"
#include "BaseLight.h"
#include "Math\Transform.h"
#include "Math\Angle.h"

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
	RENDERING_API DirectionalLight(Math::Transform& transform, const Color& color, Math::Real intensity, Math::Real halfShadowArea,
		int shadowMapSizeAsPowerOf2, Math::Real shadowSoftness, Math::Real lightBleedingReductionAmount, Math::Real minVariance);
	RENDERING_API virtual ~DirectionalLight(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	//Math::Vector3D GetDirection() const { return GetTransform().GetTransformedRot().GetForward(); }

	//virtual void InitializeShaders();
	virtual bool IsEnabled() const;
	
	virtual ShadowCameraTransform CalcShadowCameraTransform(const Math::Vector3D& cameraPos, const Math::Quaternion& cameraRot);
	virtual Math::Transform& GetTransform() { return m_transform; }
	virtual const Math::Transform& GetTransform() const { return m_transform; }

	void SetMaxIntensity(Math::Real maxIntensity) { m_maxIntensity = maxIntensity; }
	void SetSunlightDaytimeColor(const Color& sunlightDaytimeColor) { m_sunlightDaytimeColor = sunlightDaytimeColor; }
	void SetSunNearHorizonColor(const Color& sunNearHorizonColor) { m_sunNearHorizonColor = sunNearHorizonColor; }
	void SetSunlightNighttimeColor(const Color& sunlightNighttimeColor) { m_sunlightNighttimeColor = sunlightNighttimeColor; }

	std::string ToString() const;
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
