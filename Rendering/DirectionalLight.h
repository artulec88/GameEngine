#pragma once

#include "Rendering.h"
#include "BaseLight.h"
#include "Transform.h"

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
	DirectionalLight(const Math::Vector3D& color = Math::Vector3D(REAL_ZERO, REAL_ZERO, REAL_ZERO), Math::Real intensity = REAL_ZERO);
	virtual ~DirectionalLight(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	//Math::Vector3D GetDirection() const { return GetTransform().GetTransformedRot().GetForward(); }
	virtual bool IsEnabled() const;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
/* ==================== Non-static member variables end ==================== */
}; /* end class DirectionalLight */

} /* end namespace Rendering */