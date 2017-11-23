#ifndef __RENDERING_BASE_CAMERA_H__
#define __RENDERING_BASE_CAMERA_H__

#include "Rendering.h"

#include "Math/Transform.h"
#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Math/Angle.h"

namespace rendering
{
	/// <summary>
	/// The base class representing the camera.
	/// </summary>
	class BaseCamera
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary>
		/// The constructor of the base camera object.
		/// </summary>
		/// <param name="projectionMatrix">The projection matrix of the camera.</param>
		/// <param name="sensitivity">The value representing how fast the camera reacts to player input.</param>
		RENDERING_API BaseCamera(const math::Matrix4D& projectionMatrix, math::Real sensitivity);

		/// <summary>
		/// The constructor of the base camera object.
		/// </summary>
		/// <param name="fov">The field of view of the camera.</param>
		/// <param name="aspectRatio">The aspect ratio of the camera.</param>
		/// <param name="zNearPlane">The near plane of the camera.</param>
		/// <param name="zFarPlane">The far plane of the camera.</param>
		/// <param name="sensitivity">The value representing how fast the camera reacts to player input.</param>
		RENDERING_API BaseCamera(const math::Angle& fieldOfView, math::Real aspectRatio, math::Real zNearPlane, math::Real zFarPlane, math::Real sensitivity);

		/// <summary>
		/// The destructor of the camera object.
		/// </summary>
		RENDERING_API virtual ~BaseCamera();

		/// <summary> The base camera copy constructor. </summary>
		/// <param name="baseCamera"> The reference to the base camera to copy construct from. </param>
		BaseCamera(const BaseCamera& baseCamera) = default;

		/// <summary> The base camera move constructor. </summary>
		/// <param name="baseCamera"> The r-value reference to the base camera to move construct from. </param>
		BaseCamera(BaseCamera&& baseCamera) = default;

		/// <summary> The base camera copy assignment operator. </summary>
		/// <param name="baseCamera"> The reference to the base camera to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned base camera. </returns>
		BaseCamera& operator=(const BaseCamera& baseCamera) = default;

		/// <summary> The base camera move assignment operator. </summary>
		/// <param name="baseCamera"> The r-value reference to the base camera to move assign from. </param>
		/// <returns> The reference to the newly move-assigned base camera. </returns>
		BaseCamera& operator=(BaseCamera&& baseCamera) = default;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		/// <summary> Returns the reference to the transform of the camera. </summary>
		/// <returns> The reference to the transform of the camera. </returns>
		RENDERING_API virtual math::Transform& GetTransform() = 0;

		/// <summary> Returns the constant reference to the transform of the camera. </summary>
		/// <returns> The constant reference to the transform of the camera. </returns>
		RENDERING_API virtual const math::Transform& GetTransform() const = 0;

		/// <summary> Returns the sensitivity of the camera. </summary>
		/// <returns> The sensitivity of the camera. </returns>
		RENDERING_API math::Real GetSensitivity() const { return m_sensitivity; }

		/// <summary> Sets the sensitivity of the camera. </summary>
		/// <param name="sensitivity"> The new sensitivity of the camera. </param>
		RENDERING_API void SetSensitivity(const math::Real sensitivity) { m_sensitivity = sensitivity; }

		/// <summary> Activates the camera. </summary>
		RENDERING_API void Activate() { m_isActive = true; }

		/// <summary> Deactivates the camera. </summary>
		RENDERING_API void Deactivate() { m_isActive = false; }

		/// <summary> Checks whether the camera is active (<code>true</code> is returned) or not (<code>false</code> is returned). </summary>
		/// <returns> <code>true</code> when the camera is currently active and <code>false</code> otherwise. </returns>
		RENDERING_API bool IsActive() const { return m_isActive; }

		RENDERING_API const math::Matrix4D& GetProjection() const { return m_projection; }
		RENDERING_API void SetProjection(const math::Matrix4D& projection) { m_projection = projection; }
		RENDERING_API math::Matrix4D GetViewMatrix() const;
		RENDERING_API math::Matrix4D GetViewProjection() const;

		friend std::ostream& operator<<(std::ostream& out, const BaseCamera& camera)
		{
			out << "Camera: " << std::endl << camera.GetTransform() << std::endl << "Projection matrix: " <<
				std::endl << camera.m_projection << "Sensitivity: " << camera.m_sensitivity << std::endl << "Active: " <<
				(camera.m_isActive ? "Yes" : "No");
			return out;
		}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		/// <summary>
		/// The projection matrix of the camera. It describes the mapping of a pinhole camera (see https://en.wikipedia.org/wiki/Pinhole_camera)
		/// from 3D points in the world to 2D points in an image. Let <code>x</code> be a representation of a 3D point in homogeneous coordinates (4D vector)
		/// and let <code>y</code> be a representation of the image of this point in the pinhole camera (3D vector). Then the following relation holds:
		/// <code>y~Cx</code> where <code>C</code> is the camera projection matrix and the <code>~</code> sign implies that the left and right hand sides
		/// are equal up to a non-zero scalar multiplication.
		/// </summary>
		/// <remarks>
		/// See https://en.wikipedia.org/wiki/Camera_matrix, https://en.wikipedia.org/wiki/3D_projection.
		/// </remarks>
		math::Matrix4D m_projection;

		/// <summary>
		/// The camera sensitivity. The amount representing how fast the camera reacts to player's input.
		/// </summary>
		math::Real m_sensitivity;

		/// <summary>
		/// The <code>bool</code> value indicating whether the camera is a currently active camera (<code>true</code>) or not (<code>false</code>).
		/// </summary>
		bool m_isActive;
#ifdef ANT_TWEAK_BAR_ENABLED
		math::Angle m_prevFov, m_fov;
		math::Real m_prevAspectRatio, m_aspectRatio;
		math::Real m_prevNearPlane, m_nearPlane, m_prevFarPlane, m_farPlane;
#endif
		/* ==================== Non-static member variables end ==================== */
	}; /* end class BaseCamera */

} /* end namespace rendering */

#endif /* __RENDERING_CAMERA_H__ */