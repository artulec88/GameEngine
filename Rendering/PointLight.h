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
	PointLight(const Math::Vector3D& color = Math::Vector3D(0.0, 0.0, 0.0), Math::Real intensity = 0.0,
		const Attenuation& attenuation = Attenuation(0.0, 0.0, 0.0));
	virtual ~PointLight(void);
protected:
	PointLight(Shader* shader, const Math::Vector3D& color = Math::Vector3D(0.0, 0.0, 0.0), Math::Real intensity = 0.0,
		const Attenuation& attenuation = Attenuation(0.0, 0.0, 0.0));
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	Attenuation GetAttenuation() const { return this->attenuation; };
	//Math::Vector3D GetPosition() const { return this->position; };
	//void SetPosition(const Math::Vector3D& position);
	Math::Real GetRange() const { return this->range; };
	//void SetRange(Math::Real range);
	virtual bool IsEnabled() const;
protected:
	void CalculateRange();
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	Attenuation attenuation;
	//Math::Vector3D position;
	Math::Real range;
/* ==================== Non-static member variables end ==================== */
}; /* end class PointLight */

} /* end namespace Rendering */