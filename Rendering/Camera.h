#pragma once

#include "Rendering.h"
#include "GameComponent.h"
#include "Transform.h"
#include "Math\Vector.h"
#include "Math\Matrix.h"
#include "Math\Angle.h"
#include <string>

namespace Rendering
{

class CoreEngine;

class RENDERING_API CameraBase
{
/* ==================== Static variables and functions begin ==================== */
public:
	static const Math::Vector3D xAxis;
	static const Math::Vector3D yAxis;
private:
	static Math::Real sensitivity;
public:
	static Math::Real GetSensitivity();
	static void InitializeCameraSensitivity();
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	CameraBase(const Math::Matrix4D& projectionMatrix);
	CameraBase(const Math::Angle& FoV, Math::Real aspectRatio, Math::Real zNearPlane, Math::Real zFarPlane);
	virtual ~CameraBase(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	inline void SetProjection(const Math::Matrix4D& projection) { m_projection = projection; }
	virtual Math::Matrix4D GetViewProjection() const = 0;
	virtual Transform& GetTransform() = 0;
	virtual const Transform& GetTransform() const = 0;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	Math::Matrix4D m_projection;
#ifdef ANT_TWEAK_BAR_ENABLED
	Math::Angle m_prevFov, m_fov;
	Math::Real m_prevAspectRatio, m_aspectRatio;
	Math::Real m_prevNearPlane, m_nearPlane, m_prevFarPlane, m_farPlane;
#endif
/* ==================== Non-static member variables end ==================== */
}; /* end class CameraBase */

class RENDERING_API Camera : public CameraBase
{
/* ==================== Static variables and functions begin ==================== */
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	Camera(const Math::Matrix4D& projectionMatrix, const Transform& transform);
	Camera(const Math::Angle& FoV, Math::Real aspectRatio, Math::Real zNearPlane, Math::Real zFarPlane, const Transform& transform);
	virtual ~Camera(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual Math::Matrix4D GetViewProjection() const;
	virtual Transform& GetTransform() { return m_transform; }
	virtual const Transform& GetTransform() const { return m_transform; }
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	Rendering::Transform m_transform;
/* ==================== Non-static member variables end ==================== */
}; /* end class Camera */

class RENDERING_API CameraComponent : public CameraBase, public GameComponent
{
/* ==================== Static variables and functions begin ==================== */
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	CameraComponent(const Math::Matrix4D& projectionMatrix);
	CameraComponent(const Math::Angle& FoV, Math::Real aspectRatio, Math::Real zNearPlane, Math::Real zFarPlane);
	virtual ~CameraComponent(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual Math::Matrix4D GetViewProjection() const;
#ifdef ANT_TWEAK_BAR_ENABLED
	virtual void Input(Math::Real delta);
#endif
	virtual void AddToEngine(CoreEngine* coreEngine);
	virtual Transform& GetTransform() { return GameComponent::GetTransform(); }
	virtual const Transform& GetTransform() const { return GameComponent::GetTransform(); }
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
/* ==================== Non-static member variables end ==================== */
}; /* end class Camera */

} /* end namespace Rendering */