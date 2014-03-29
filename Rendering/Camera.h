#pragma once

#include "Rendering.h"
#include "GameComponent.h"
#include "Math\Vector.h"
#include "Math\Matrix.h"
#include "Math\Angle.h"
#include <string>

namespace Rendering
{

class Renderer;

class RENDERING_API Camera : public GameComponent
{
/* ==================== Static variables begin ==================== */
private:
	static const Math::Vector3D xAxis;
	static const Math::Vector3D yAxis;
public:
	static const Math::Real defaultFoV;
	static const Math::Real defaultAspectRatio;
	static const Math::Real defaultNearPlane;
	static const Math::Real defaultFarPlane;
	static const Math::Vector3D defaultCameraPos;
	static const Math::Vector3D defaultCameraForward;
	static const Math::Vector3D defaultCameraUp;
	//static const Camera defaultCamera;
/* ==================== Static variables end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	//Math::Vector3D pos;
	//Math::Vector3D forward;
	//Math::Vector3D up;
	Math::Matrix4D projection;
/* ==================== Non-static member variables end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	Camera();
	Camera(Math::Real FoV, Math::Real aspectRatio, Math::Real zNearPlane, Math::Real zFarPlane);
	~Camera(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	//Math::Vector3D GetPos() const { return this->pos; };
	//Math::Vector3D GetForward() const { return this->forward; };
	//Math::Vector3D GetUp() const { return this->up; };
	//void SetPos(const Math::Vector3D& pos);
	//void SetForward(const Math::Vector3D& forward);
	//void SetUp(const Math::Vector3D& up);

	Math::Matrix4D GetViewProjection() const;

	void Move(const Math::Vector3D& dir, Math::Real amount);
	//void RotateX(const Math::Angle& angle);
	//void RotateY(const Math::Angle& angle);

	//virtual void Input(Math::Real delta);
	//virtual void Update(Math::Real delta);
	//virtual void Render(Shader* shader, Renderer* renderer);
	virtual void AddToRenderingEngine(Renderer* renderer);


	//Math::Vector3D GetLeft() const;
	//Math::Vector3D GetRight() const;
	void Input(int key, Math::Real delta);
	std::string ToString() const;
/* ==================== Non-static member functions end ==================== */
}; /* end class Camera */

} /* end namespace Rendering */