#ifndef __RENDERING_CAMERA_H__
#define __RENDERING_CAMERA_H__

#include "Rendering.h"
#include "BaseCamera.h"
#include "Math\Transform.h"
#include "Math\Vector.h"
#include "Math\Matrix.h"
#include "Math\Angle.h"
#include <string>

namespace Rendering
{
	/// <summary>
	/// The class representing the camera.
	/// </summary>
	class Camera /* : public BaseCamera */
	{
		/* ==================== Static variables and functions begin ==================== */
	private:
		static constexpr Math::Real DEFAULT_SENSITIVITY = 0.05f;
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary>
		/// The default constructor of the camera object.
		/// </summary>
		RENDERING_API Camera();

		/// <summary>
		/// The constructor of the camera object.
		/// </summary>
		/// <param name="position">The position of the camera in the world.</param>
		/// <param name="rotation">The rotation of the camera in the world.</param>
		/// <param name="projectionMatrix">The projection matrix of the camera.</param>
		/// <param name="sensitivity">The value representing how fast the camera reacts to player input.</param>
		RENDERING_API Camera(const Math::Vector3D& position, const Math::Quaternion& rotation, const Math::Matrix4D& projectionMatrix, Math::Real sensitivity);
		/// <summary>
		/// The constructor of the camera object.
		/// </summary>
		/// <param name="position">The position of the camera in the world.</param>
		/// <param name="rotation">The rotation of the camera in the world.</param>
		/// <param name="fov">The field of view of the camera.</param>
		/// <param name="aspectRatio">The aspect ratio of the camera.</param>
		/// <param name="zNearPlane">The near plane of the camera.</param>
		/// <param name="zFarPlane">The far plane of the camera.</param>
		/// <param name="sensitivity">The value representing how fast the camera reacts to player input.</param>
		RENDERING_API Camera(const Math::Vector3D& position, const Math::Quaternion& rotation, const Math::Angle& FoV, Math::Real aspectRatio,
			Math::Real zNearPlane, Math::Real zFarPlane, Math::Real sensitivity);

		/// <summary>
		/// The destructor of the camera object.
		/// </summary>
		RENDERING_API ~Camera(void);

		/// <summary> The camera copy constructor. </summary>
		RENDERING_API Camera(const Camera& camera) = default;

		/// <summary> The camera move constructor. </summary>
		RENDERING_API Camera(Camera&& camera) = default;
		
		/// <summary> The camera copy assignment operator. </summary>
		Camera& operator=(const Camera& camera) = default;

		/// <summary> The camera move assignment operator. </summary>
		Camera& operator=(Camera&& camera) = default;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		RENDERING_API const Math::Vector3D& GetPos() const { return m_pos; }
		RENDERING_API const Math::Quaternion& GetRot() const { return m_rot; }
		RENDERING_API Math::Vector3D& GetPos() { return m_pos; }
		RENDERING_API Math::Quaternion& GetRot() { return m_rot; }
		RENDERING_API void SetPos(const Math::Vector3D& position) { m_pos = position; }
		RENDERING_API void SetRot(const Math::Quaternion& rotation) { m_rot = rotation; }

		RENDERING_API Math::Real GetSensitivity() const { return m_sensitivity; }
		RENDERING_API void SetSensitivity(Math::Real sensitivity) { m_sensitivity = sensitivity; }
		RENDERING_API inline void Activate() { m_isActive = true; }
		RENDERING_API inline void Deactivate() { m_isActive = false; }
		RENDERING_API inline bool IsActive() const { return m_isActive; }
		RENDERING_API inline const Math::Matrix4D& GetProjection() const { return m_projection; }
		RENDERING_API inline void SetProjection(const Math::Matrix4D& projection) { m_projection = projection; }
		RENDERING_API Math::Matrix4D GetViewMatrix() const;
		RENDERING_API Math::Matrix4D GetViewProjection() const;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		/// <summary> The camera position. </summary>
		Math::Vector3D m_pos;

		/// <summary> The camera rotation. </summary>
		Math::Quaternion m_rot;

		/// <summary> The projection of the camera. </summary>
		Math::Matrix4D m_projection;

		// TODO: Create a mutable matrix "m_viewMatrix" which would store the GetViewMatrix() result and save it for the next GetViewMatrix() function call.
		///* mutable */ Math::Matrix4D m_viewMatrix;

		/// <summary>
		/// The camera sensitivity. The amount representing how fast the camera reacts to player's input.
		/// </summary>
		Math::Real m_sensitivity;
		/// <summary>
		/// The <code>bool</code> value indicating whether the camera is currently active (<code>true</code>) or not (<code>false</code>).
		/// </summary>
		bool m_isActive;
#ifdef ANT_TWEAK_BAR_ENABLED
		Math::Angle m_prevFov, m_fov;
		Math::Real m_prevAspectRatio, m_aspectRatio;
		Math::Real m_prevNearPlane, m_nearPlane, m_prevFarPlane, m_farPlane;
#endif
		/* ==================== Non-static member variables end ==================== */
	}; /* end class Camera */

} /* end namespace Rendering */

#endif /* __RENDERING_CAMERA_H__ */