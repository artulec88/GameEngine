#pragma once

#include "PointLight.h"
#include "Attenuation.h"
#include "Transform.h"

#include "Math\Vector.h"

namespace Rendering
{

class RENDERING_API SpotLight : public PointLight
{
/* ==================== Non-static member variables begin ==================== */
private:
	Math::Real cutoff;
/* ==================== Non-static member variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	SpotLight(const Math::Vector3D& color = Math::Vector3D(0.0, 0.0, 0.0), Math::Real intensity = 0.0,
		const Attenuation& attenuation = Attenuation(0.0, 0.0, 0.0), Math::Real cutoff = 0.0);
	virtual ~SpotLight(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	Math::Vector3D GetDirection() const { return GetTransform().GetTransformedRot().GetForward(); }
	//void SetDirection(const Math::Vector3D& direction);
	Math::Real GetCutoff() const { return this->cutoff; };
	void SetCutoff(Math::Real cutoff);
/* ==================== Non-static member functions end ==================== */
}; /* end class SpotLight */

} /* end namespace Rendering */