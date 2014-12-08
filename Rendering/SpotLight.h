#pragma once

#include "PointLight.h"
#include "Attenuation.h"
#include "Transform.h"

#include "Math\Vector.h"

namespace Rendering
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
	SpotLight(const Math::Vector3D& color = Math::Vector3D(REAL_ZERO, REAL_ZERO, REAL_ZERO), Math::Real intensity = REAL_ZERO,
		const Attenuation& attenuation = Attenuation(), Math::Real cutoff = REAL_ZERO);
	virtual ~SpotLight(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	//Math::Vector3D GetDirection() const { return GetTransform().GetTransformedRot().GetForward(); }
	Math::Real GetCutoff() const { return this->cutoff; };
	virtual bool IsEnabled() const;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	Math::Real cutoff;
/* ==================== Non-static member variables end ==================== */
}; /* end class SpotLight */

} /* end namespace Rendering */