#pragma once

#include "Rendering.h"
#include "BaseLight.h"
#include "Transform.h"

namespace Rendering
{

class RENDERING_API DirectionalLight : public BaseLight
{
/* ==================== Non-static member variables begin ==================== */
private:
/* ==================== Non-static member variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	DirectionalLight(const Math::Vector3D& color = Math::Vector3D(1.0, 1.0, 1.0), Math::Real intensity = 0.0);
	virtual ~DirectionalLight(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	Math::Vector3D GetDirection() const { return GetTransform().GetTransformedRot().GetForward(); }
/* ==================== Non-static member functions end ==================== */
}; /* end class DirectionalLight */

} /* end namespace Rendering */