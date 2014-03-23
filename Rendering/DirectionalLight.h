#pragma once
#include "BaseLight.h"

namespace Rendering
{

class DirectionalLight : public BaseLight
{
/* ==================== Non-static member variables begin ==================== */
private:
	Math::Vector3D direction;
/* ==================== Non-static member variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	DirectionalLight(const Math::Vector3D& color = Math::Vector3D(1.0, 1.0, 1.0), Math::Real intensity = 0.0,
		const Math::Vector3D& direction = Math::Vector3D(0.0, 0.0, 0.0));
	virtual ~DirectionalLight(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	Math::Vector3D GetDirection() const;
	void SetDirection(const Math::Vector3D& direction);
/* ==================== Non-static member functions end ==================== */
}; /* end class DirectionalLight */

} /* end namespace Rendering */