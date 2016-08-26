//#ifndef __RENDERING_BASE_CAMERA_H__
//#define __RENDERING_BASE_CAMERA_H__
//
//#include "Rendering.h"
//#include "Math\Transform.h"
//#include "Math\Vector.h"
//#include "Math\Matrix.h"
//#include "Math\Angle.h"
//#include <string>
//
//namespace Rendering
//{
//	/// <summary>
//	/// The base class representing the camera.
//	/// </summary>
//	class BaseCamera
//	{
//		/* ==================== Static variables and functions begin ==================== */
//		/* ==================== Static variables and functions end ==================== */
//
//		/* ==================== Constructors and destructors begin ==================== */
//	public:
//		/// <summary>
//		/// The constructor of the base camera object.
//		/// </summary>
//		/// <param name="projectionMatrix">The projection matrix of the camera.</param>
//		/// <param name="sensitivity">The value representing how fast the camera reacts to player input.</param>
//		RENDERING_API BaseCamera(const Math::Matrix4D& projectionMatrix, Math::Real sensitivity);
//		/// <summary>
//		/// The constructor of the base camera object.
//		/// </summary>
//		/// <param name="fov">The field of view of the camera.</param>
//		/// <param name="aspectRatio">The aspect ratio of the camera.</param>
//		/// <param name="zNearPlane">The near plane of the camera.</param>
//		/// <param name="zFarPlane">The far plane of the camera.</param>
//		/// <param name="sensitivity">The value representing how fast the camera reacts to player input.</param>
//		RENDERING_API BaseCamera(const Math::Angle& FoV, Math::Real aspectRatio, Math::Real zNearPlane, Math::Real zFarPlane, Math::Real sensitivity);
//
//		/// <summary>
//		/// The destructor of the camera object.
//		/// </summary>
//		RENDERING_API virtual ~BaseCamera(void);
//
//		BaseCamera(const BaseCamera& baseCamera) = delete; // Copy constructor
//		BaseCamera(BaseCamera&& baseCamera) = delete; // Move constructor
//		BaseCamera& operator=(const BaseCamera& baseCamera) = delete; // Copy assignment operator
//		BaseCamera& operator=(BaseCamera&& baseCamera) = delete; // Move assignment operator
//		/* ==================== Constructors and destructors end ==================== */
//
//		/* ==================== Non-static member functions begin ==================== */
//	public:
//		virtual const Math::Vector3D& GetPos() const = 0;
//		virtual const Math::Quaternion& GetRot() const = 0;
//		virtual Math::Vector3D& GetPos() = 0;
//		virtual Math::Quaternion& GetRot() = 0;
//		virtual void SetPos(const Math::Vector3D& position) = 0;
//		virtual void SetRot(const Math::Quaternion& rotation) = 0;
//		Math::Real GetSensitivity() const { return m_sensitivity; }
//		void Activate() { m_isActive = true; }
//		void Deactivate() { m_isActive = false; }
//		inline bool IsActive() const { return m_isActive; }
//		RENDERING_API inline const Math::Matrix4D& GetProjection() const { return m_projection; }
//		inline void SetProjection(const Math::Matrix4D& projection) { m_projection = projection; }
//		RENDERING_API Math::Matrix4D GetViewMatrix() const;
//		Math::Matrix4D GetViewProjection() const;
//		/* ==================== Non-static member functions end ==================== */
//
//		/* ==================== Non-static member variables begin ==================== */
//	protected:
//		Math::Matrix4D m_projection;
//
//		/// <summary>
//		/// The camera sensitivity. The amount representing how fast the camera reacts to player's input.
//		/// </summary>
//		Math::Real m_sensitivity;
//		/// <summary>
//		/// The <code>bool</code> value indicating whether the camera is a currently active camera (<code>true</code>) or not (<code>false</code>).
//		/// </summary>
//		bool m_isActive;
//#ifdef ANT_TWEAK_BAR_ENABLED
//		Math::Angle m_prevFov, m_fov;
//		Math::Real m_prevAspectRatio, m_aspectRatio;
//		Math::Real m_prevNearPlane, m_nearPlane, m_prevFarPlane, m_farPlane;
//#endif
//		/* ==================== Non-static member variables end ==================== */
//	}; /* end class Camera */
//
//} /* end namespace Rendering */
//
//#endif /* __RENDERING_CAMERA_H__ */