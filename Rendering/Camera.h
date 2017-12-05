#ifndef __RENDERING_CAMERA_H__
#define __RENDERING_CAMERA_H__

#include "Rendering.h"
#include "Math/Transform.h"
#include "CameraBuilder.h"

namespace rendering
{
	/// <summary>
	/// The class representing the camera.
	/// </summary>
	class Camera
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary>
		/// The default constructor of the camera object.
		/// </summary>
		RENDERING_API Camera(const CameraBuilder& cameraBuilder);

		/// <summary>
		/// The destructor of the camera object.
		/// </summary>
		RENDERING_API ~Camera();

		/// <summary> The camera copy constructor. </summary>
		/// <param name="camera"> The reference to the camera to copy construct from. </param>
		RENDERING_API Camera(const Camera& camera) = default;

		/// <summary> The camera move constructor. </summary>
		/// <param name="camera"> The r-value reference to the camera to move construct from. </param>
		RENDERING_API Camera(Camera&& camera) = default;
		
		/// <summary> The camera copy assignment operator. </summary>
		/// <param name="camera"> The reference to the camera to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned camera. </returns>
		Camera& operator=(const Camera& camera) = default;

		/// <summary> The camera move assignment operator. </summary>
		/// <param name="camera"> The r-value reference to the camera to move assign from. </param>
		/// <returns> The reference to the newly move-assigned camera. </returns>
		Camera& operator=(Camera&& camera) = default;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		/// <summary> Returns the reference to the position of the camera. </summary>
		/// <returns> The reference to the position of the camera. </returns>
		RENDERING_API math::Vector3D& GetPos() { return m_pos; }

		/// <summary> Returns the constant reference to the position of the camera. </summary>
		/// <returns> The constant reference to the position of the camera. </returns>
		RENDERING_API const math::Vector3D& GetPos() const { return m_pos; }

		/// <summary> Returns the reference to the rotation of the camera. </summary>
		/// <returns> The reference to the rotation of the camera. </returns>
		RENDERING_API math::Quaternion& GetRot() { return m_rot; }

		/// <summary> Returns the constant reference to the rotation of the camera. </summary>
		/// <returns> The constant reference to the rotation of the camera. </returns>
		RENDERING_API const math::Quaternion& GetRot() const { return m_rot; }

		/// <summary> Returns the sensitivity of the camera. </summary>
		/// <returns> The sensitivity of the camera. </returns>
		RENDERING_API math::Real GetSensitivity() const { return m_sensitivity; }

		/// <summary> Sets the sensitivity of the camera. </summary>
		/// <param name="sensitivity"> The new sensitivity of the camera. </param>
		//RENDERING_API void SetSensitivity(const math::Real sensitivity) { m_sensitivity = sensitivity; }

		RENDERING_API void Rotate(const math::Vector3D& axis, const math::Angle& angle);
		RENDERING_API void Rotate(const math::Quaternion& rotation);

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

		friend std::ostream& operator<<(std::ostream& out, const Camera& camera)
		{
			out << "Camera: " << std::endl << camera.GetPos() << std::endl << "Projection matrix: " <<
				std::endl << camera.m_projection << std::endl << "Active: " << (camera.m_isActive ? "Yes" : "No");
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

		/// <summary> The camera position. </summary>
		math::Vector3D m_pos;

		/// <summary> The camera rotation. </summary>
		math::Quaternion m_rot;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class Camera */

} /* end namespace rendering */

#endif /* __RENDERING_CAMERA_H__ */