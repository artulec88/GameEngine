#pragma once

#include "Rendering.h"
#include "Math\Vector.h"
#include <string>

namespace Rendering
{

class RENDERING_API Camera
{
/* ==================== Static variables begin ==================== */
private:
	static const Math::Vector3D yAxis;
/* ==================== Static variables end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	Math::Vector3D pos;
	Math::Vector3D forward;
	Math::Vector3D up;
/* ==================== Non-static member variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	Camera();
	Camera(const Math::Vector3D& pos, const Math::Vector3D& forward, const Math::Vector3D& up);
	~Camera(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	Math::Vector3D GetPos() const { return this->pos; };
	Math::Vector3D GetForward() const { return this->forward; };
	Math::Vector3D GetUp() const { return this->up; };
	void SetPos(const Math::Vector3D& pos);
	void SetForward(const Math::Vector3D& forward);
	void SetUp(const Math::Vector3D& up);

	void Move(const Math::Vector3D& dir, Math::Real amount);
	void RotateX(Math::Real angleInDegrees);
	void RotateY(Math::Real angleInDegrees);

	Math::Vector3D GetLeft() const;
	Math::Vector3D GetRight() const;
	void Input(int key, Math::Real delta);
	std::string ToString() const;
/* ==================== Non-static member functions end ==================== */
}; /* end class Camera */

} /* end namespace Rendering */