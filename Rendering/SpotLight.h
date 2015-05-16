#pragma once

#include "PointLight.h"
#include "Attenuation.h"
#include "Transform.h"

#include "Math\Vector.h"
#include "Math\Angle.h"

namespace Rendering { namespace Lighting
{

class RENDERING_API SpotLight : public PointLight
{
/* ==================== Static variables and functions begin ==================== */
public:
	static bool spotLightsEnabled;
public:
	static bool* GetSpotLightsEnabled() { return &spotLightsEnabled; }
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	SpotLight(const Color& color = Color(REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ONE), Math::Real intensity = REAL_ZERO,
		const Attenuation& attenuation = Attenuation(REAL_ZERO, REAL_ZERO, REAL_ONE), const Math::Angle& viewAngle = Math::Angle(170.0f),
		int shadowMapSizeAsPowerOf2 = 0, Math::Real shadowSoftness = REAL_ONE,
		Math::Real lightBleedingReductionAmount = static_cast<Math::Real>(0.2f),
		Math::Real minVariance = static_cast<Math::Real>(0.00002f) );
	virtual ~SpotLight(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	//Math::Vector3D GetDirection() const { return GetTransform().GetTransformedRot().GetForward(); }
	Math::Real GetCutoff() const { return m_cutoff; };
	//virtual void InitializeShaders();
	virtual bool IsEnabled() const;

	virtual LightType GetLightType() const { return SPOT; }
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	/**
	 * Stores a value of a cosine (from range between -1 and +1).
	 * If the value of the cosine between a light direction and spot light direction vectors is
	 * smaller than cutoff then the point is not in the spot light cone.
	 * If cutoff = 0 then FoV = 180 degrees.
	 * If cutoff = 1 then FoV = 0 degrees.
	 */
	Math::Real m_cutoff;
/* ==================== Non-static member variables end ==================== */
}; /* end class SpotLight */

} /* end namespace Lighting */

} /* end namespace Rendering */