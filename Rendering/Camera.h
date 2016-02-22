#ifndef __RENDERING_CAMERA_H__
#define __RENDERING_CAMERA_H__

#include "Rendering.h"
#include "GameComponent.h"
#include "Math\Transform.h"
#include "IInputableKeyboard.h"
#include "IInputableMouse.h"
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
/* ==================== Static variables and functions end ==================== */

/* ==================== Constructors and destructors begin ==================== */
public:
	/// <summary>
	/// The constructor of the camera base object.
	/// </summary>
	/// <param name="projectionMatrix">The projection matrix of the camera.</param>
	/// <param name="sensitivity">The value representing how fast the camera reacts to player input.</param>
	RENDERING_API CameraBase(const Math::Matrix4D& projectionMatrix, Math::Real sensitivity);

	/// <summary>
	/// The constructor of the camera base object.
	/// </summary>
	/// <param name="fov">The field of view of the camera.</param>
	/// <param name="aspectRatio">The aspect ratio of the camera.</param>
	/// <param name="zNearPlane">The near plane of the camera.</param>
	/// <param name="zFarPlane">The far plane of the camera.</param>
	/// <param name="sensitivity">The value representing how fast the camera reacts to player input.</param>
	RENDERING_API CameraBase(const Math::Angle& fov, Math::Real aspectRatio, Math::Real zNearPlane, Math::Real zFarPlane, Math::Real sensitivity);

	/// <summary>
	/// The destructor of the camera object.
	/// </summary>
	RENDERING_API virtual ~CameraBase(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	void Activate() { m_isActive = true; }
	void Deactivate() { m_isActive = false; }
	inline bool IsActive() const { return m_isActive; }
	inline void SetProjection(const Math::Matrix4D& projection) { m_projection = projection; }
	Math::Matrix4D GetViewProjection() const;
	virtual Math::Transform& GetTransform() = 0;
	virtual const Math::Transform& GetTransform() const = 0;
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
	Camera(const Math::Matrix4D& projectionMatrix, const Math::Transform& transform, Math::Real sensitivity);
	Camera(const Math::Angle& FoV, Math::Real aspectRatio, Math::Real zNearPlane, Math::Real zFarPlane, const Math::Transform& transform, Math::Real sensitivity);
	virtual ~Camera(void);
/* ==================== Constructors and destructors end ==================== */

/* ==================== Non-static member functions begin ==================== */
public:
	virtual Math::Transform& GetTransform() { return m_transform; }
	virtual const Math::Transform& GetTransform() const { return m_transform; }
/* ==================== Non-static member functions end ==================== */

/* ==================== Non-static member variables begin ==================== */
protected:
	Math::Transform m_transform;
/* ==================== Non-static member variables end ==================== */
}; /* end class Camera */

} /* end namespace Rendering */

#endif /* __RENDERING_CAMERA_H__ */