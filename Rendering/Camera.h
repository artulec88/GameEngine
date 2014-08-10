#pragma once

#include "Rendering.h"
#include "GameComponent.h"
#include "Math\Vector.h"
#include "Math\Matrix.h"
#include "Math\Angle.h"
#include <string>

namespace Rendering
{

class CoreEngine;

class RENDERING_API Camera : public GameComponent
{
/* ==================== Static variables begin ==================== */
public:
	static const Math::Vector3D xAxis;
	static const Math::Vector3D yAxis;
	static const Math::Real sensitivity;
/* ==================== Static variables end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	Math::Matrix4D projection;
/* ==================== Non-static member variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	//Camera();
	Camera(const Math::Matrix4D& projectionMatrix);
	Camera(const Math::Angle& FoV, Math::Real aspectRatio, Math::Real zNearPlane, Math::Real zFarPlane);
	~Camera(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	inline void SetProjection(const Math::Matrix4D& projection) { this->projection = projection; }
	Math::Matrix4D GetViewProjection() const;

	void Move(const Math::Vector3D& dir, Math::Real amount);

	virtual void Input(Math::Real delta);
	//virtual void Update(Math::Real delta);
	//virtual void Render(Shader* shader, Renderer* renderer);
	virtual void AddToEngine(CoreEngine* coreEngine);


	//Math::Vector3D GetLeft() const;
	//Math::Vector3D GetRight() const;
	//void Input(Math::Real delta);
	//void Deactivate();
	//void Activate();
	std::string ToString() const;
/* ==================== Non-static member functions end ==================== */
}; /* end class Camera */

} /* end namespace Rendering */