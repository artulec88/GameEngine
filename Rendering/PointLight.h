#pragma once

#include "Rendering.h"
#include "Attenuation.h"
#include "BaseLight.h"
#include "Color.h"

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
	PointLight(const Color& color = Color(REAL_ZERO, REAL_ZERO, REAL_ZERO, REAL_ONE), Math::Real intensity = REAL_ZERO,
		const Attenuation& attenuation = Attenuation(REAL_ZERO, REAL_ZERO, REAL_ONE));
	virtual ~PointLight(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	Attenuation GetAttenuation() const { return m_attenuation; };
	Math::Real GetRange() const { return m_range; };
	//virtual void InitializeShaders();
	virtual bool IsEnabled() const;

	virtual void Update(Math::Real deltaTime);
protected:
	void CalculateRange();
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	Attenuation m_attenuation;
	Math::Real m_range;
/* ==================== Non-static member variables end ==================== */
}; /* end class PointLight */

} /* end namespace Rendering */