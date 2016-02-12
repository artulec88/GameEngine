#ifndef __RENDERING_CAMERA_H__
#define __RENDERING_CAMERA_H__

#include "Rendering.h"
#include "GameComponent.h"
#include "Transform.h"
#include "IInputableKeyboard.h"
#include "IUpdateable.h"
#include "Math\Vector.h"
#include "Math\Matrix.h"
#include "Math\Angle.h"
#include <string>

namespace Rendering
{
	class GameNode;

/// <summary>
/// The base class representing the camera.
/// </summary>
class CameraBase
{
/* ==================== Static variables and functions begin ==================== */
private:
	static const Math::Real DEFAULT_CAMERA_SENSITIVITY;
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	/// <summary>
	/// The constructor of the camera base object.
	/// </summary>
	/// <param name="projectionMatrix">The projection matrix of the camera.</param>
	/// <param name="sensitivity">The value representing how fast the camera reacts to player input.</param>
	CameraBase(const Math::Matrix4D& projectionMatrix, Math::Real sensitivity = DEFAULT_CAMERA_SENSITIVITY);

	/// <summary>
	/// The constructor of the camera base object.
	/// </summary>
	/// <param name="fov">The field of view of the camera.</param>
	/// <param name="aspectRatio">The aspect ratio of the camera.</param>
	/// <param name="zNearPlane">The near plane of the camera.</param>
	/// <param name="zFarPlane">The far plane of the camera.</param>
	/// <param name="sensitivity">The value representing how fast the camera reacts to player input.</param>
	CameraBase(const Math::Angle& fov, Math::Real aspectRatio, Math::Real zNearPlane, Math::Real zFarPlane, Math::Real sensitivity = DEFAULT_CAMERA_SENSITIVITY);

	/// <summary>
	/// The destructor of the camera object.
	/// </summary>
	virtual ~CameraBase(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	void Activate() { m_isActive = true; }
	void Deactivate() { m_isActive = false; }
	inline bool IsActive() const { return m_isActive; }
	inline void SetProjection(const Math::Matrix4D& projection) { m_projection = projection; }
	Math::Matrix4D GetViewProjection() const;
	virtual Transform& GetTransform() = 0;
	virtual const Transform& GetTransform() const = 0;
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	Math::Matrix4D m_projection;
	/// <summary>
	/// The camera sensitivity. The amount representing how fast the camera reacts to player's input.
	/// </summary>
	Math::Real m_sensitivity;
	/// <summary>
	/// The <code>bool</code> value indicating whether the camera is a currently active camera (<code>true</code>) or not (<code>false</code>).
	/// </summary>
	bool m_isActive;
#ifdef ANT_TWEAK_BAR_ENABLED
	Math::Angle m_prevFov, m_fov;
	Math::Real m_prevAspectRatio, m_aspectRatio;
	Math::Real m_prevNearPlane, m_nearPlane, m_prevFarPlane, m_farPlane;
#endif
/* ==================== Non-static member variables end ==================== */
}; /* end class CameraBase */

class Camera : public CameraBase
{
/* ==================== Static variables and functions begin ==================== */
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	Camera(const Math::Matrix4D& projectionMatrix, const Transform& transform, Math::Real sensitivity);
	Camera(const Math::Angle& FoV, Math::Real aspectRatio, Math::Real zNearPlane, Math::Real zFarPlane, const Transform& transform, Math::Real sensitivity);
	virtual ~Camera(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual Transform& GetTransform() { return m_transform; }
	virtual const Transform& GetTransform() const { return m_transform; }
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	Rendering::Transform m_transform;
/* ==================== Non-static member variables end ==================== */
}; /* end class Camera */

class CameraComponent : public CameraBase, public GameComponent, public IUpdateable
{
/* ==================== Static variables and functions begin ==================== */
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	RENDERING_API CameraComponent(const Math::Matrix4D& projectionMatrix, Math::Real sensitivity);
	RENDERING_API CameraComponent(const Math::Angle& FoV, Math::Real aspectRatio, Math::Real zNearPlane, Math::Real zFarPlane, Math::Real sensitivity);
	RENDERING_API virtual ~CameraComponent(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual void Update(Math::Real deltaTime);
	virtual Transform& GetTransform() { return GameComponent::GetTransform(); }
	virtual const Transform& GetTransform() const { return GameComponent::GetTransform(); }
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
/* ==================== Non-static member variables end ==================== */
}; /* end class CameraComponent */

class CameraMoveComponent : public CameraComponent, public Input::IInputableKeyboard
{
/* ==================== Static variables and functions begin ==================== */
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	RENDERING_API CameraMoveComponent(const Math::Matrix4D& projectionMatrix, Math::Real sensitivity);
	RENDERING_API CameraMoveComponent(const Math::Angle& FoV, Math::Real aspectRatio, Math::Real zNearPlane, Math::Real zFarPlane, Math::Real sensitivity);
	RENDERING_API virtual ~CameraMoveComponent(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual void KeyEvent(int key, int scancode, int action, int mods);
	virtual void Update(Math::Real deltaTime);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	bool m_forward, m_backward, m_left, m_right, m_up, m_down;
	Math::Vector3D m_velocity;
	Math::Real m_maxSpeed;
/* ==================== Non-static member variables end ==================== */
}; /* end class CameraMoveComponent */

class CameraFollowComponent : public CameraComponent
{
/* ==================== Static variables and functions begin ==================== */
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	RENDERING_API CameraFollowComponent(const Math::Matrix4D& projectionMatrix, Math::Real sensitivity, GameNode* entityToFollow);
	RENDERING_API CameraFollowComponent(const Math::Angle& FoV, Math::Real aspectRatio, Math::Real zNearPlane, Math::Real zFarPlane, Math::Real sensitivity, GameNode* entityToFollow);
	RENDERING_API virtual ~CameraFollowComponent(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual void Update(Math::Real delta);
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
private:
	GameNode* m_gameEntityToFollow;
/* ==================== Non-static member variables end ==================== */
}; /* end class CameraFollowComponent */

} /* end namespace Rendering */

#endif /* __RENDERING_CAMERA_H__ */