#pragma once

#include "Rendering.h"
#include "Attenuation.h"
#include "BaseLight.h"
#include "Color.h"

namespace Rendering { namespace Lighting
{

class PointLight : public BaseLight
{
/* ==================== Static variables and functions begin ==================== */
public:
	static bool pointLightsEnabled;
public:
	static bool* ArePointLightsEnabled() { return &pointLightsEnabled; }
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	RENDERING_API PointLight(Math::Transform& transform, const Color& color, Math::Real intensity, const Attenuation& attenuation);
	RENDERING_API virtual ~PointLight(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	Attenuation GetAttenuation() const { return m_attenuation; };
	Math::Real GetRange() const { return m_range; };
	virtual bool IsEnabled() const;
protected:
	void CalculateRange();
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	Attenuation m_attenuation;
	Math::Real m_range;
/* ==================== Non-static member variables end ==================== */
}; /* end class PointLight */

} /* end namespace Lighting */

} /* end namespace Rendering */