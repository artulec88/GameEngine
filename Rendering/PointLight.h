#pragma once

#include "Rendering.h"
#include "Attenuation.h"
#include "BaseLight.h"
#include "Math\Vector.h"

namespace Rendering
{

class RENDERING_API PointLight : public BaseLight
{
/* ==================== Static variables and functions begin ==================== */
public:
	static bool pointLightsEnabled;
public:
	static bool* GetPointLightsEnabled() { return &pointLightsEnabled; }
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	PointLight(const Math::Vector3D& color = Math::Vector3D(REAL_ZERO, REAL_ZERO, REAL_ZERO), Math::Real intensity = REAL_ZERO,
		const Attenuation& attenuation = Attenuation());
	virtual ~PointLight(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	Attenuation GetAttenuation() const { return this->attenuation; };
	Math::Real GetRange() const { return this->range; };
	virtual bool IsEnabled() const;
protected:
	void CalculateRange();
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	Attenuation attenuation;
	Math::Real range;
/* ==================== Non-static member variables end ==================== */
}; /* end class PointLight */

} /* end namespace Rendering */