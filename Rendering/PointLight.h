#pragma once

#include "Attenuation.h"
#include "BaseLight.h"
#include "Math\Vector.h"

namespace Rendering
{

class PointLight : public BaseLight
{
/* ==================== Non-static member variables begin ==================== */
private:
	Attenuation attenuation;
	Math::Vector3D position;
	Math::Real range;
/* ==================== Non-static member variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	PointLight(const Math::Vector3D& color = Math::Vector3D(0.0, 0.0, 0.0), Math::Real intensity = 0.0,
		const Attenuation& attenuation = Attenuation(0.0, 0.0, 0.0),
		const Math::Vector3D& position = Math::Vector3D(0.0, 0.0, 0.0),
		Math::Real range = 1.0);
	virtual ~PointLight(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	Attenuation GetAttenuation() const { return this->attenuation; };
	Math::Vector3D GetPosition() const { return this->position; };
	void SetPosition(const Math::Vector3D& position);
	Math::Real GetRange() const { return this->range; };
	void SetRange(Math::Real range);
/* ==================== Non-static member functions end ==================== */
}; /* end class PointLight */

} /* end namespace Rendering */