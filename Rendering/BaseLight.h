#pragma once

#include "Math\Vector.h"

namespace Rendering
{

class BaseLight
{
/* ==================== Non-static member variables begin ==================== */
private:
	Math::Vector3D color;
	Math::Real intensity;
/* ==================== Non-static member variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	BaseLight(void);
	BaseLight(const Math::Vector3D& color = Math::Vector3D(0.0, 0.0, 0.0), Math::Real intensity = 0.0);
	virtual ~BaseLight(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	Math::Vector3D GetColor() const;
	void SetColor(const Math::Vector3D& color);
	Math::Real GetIntensity() const;
	void SetIntensity(Math::Real intensity);
/* ==================== Non-static member functions end ==================== */
}; /* end class BaseLight */

} /* end namespace Rendering */