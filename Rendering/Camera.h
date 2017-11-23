#ifndef __RENDERING_CAMERA_H__
#define __RENDERING_CAMERA_H__

#include "Rendering.h"
#include "BaseCamera.h"
#include "Math/Transform.h"

namespace rendering
{
	/// <summary>
	/// The class representing the camera.
	/// </summary>
	class Camera : public BaseCamera
	{
		/* ==================== Static variables and functions begin ==================== */
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
		RENDERING_API Camera(const math::Vector3D& position, const math::Quaternion& rotation, const math::Matrix4D& projectionMatrix, math::Real sensitivity);
		/// <summary>
		/// The constructor of the camera object.
		/// </summary>
		/// <param name="position">The position of the camera in the world.</param>
		/// <param name="rotation">The rotation of the camera in the world.</param>
		/// <param name="fieldOfView">The field of view of the camera.</param>
		/// <param name="aspectRatio">The aspect ratio of the camera.</param>
		/// <param name="zNearPlane">The near plane of the camera.</param>
		/// <param name="zFarPlane">The far plane of the camera.</param>
		/// <param name="sensitivity">The value representing how fast the camera reacts to player input.</param>
		RENDERING_API Camera(const math::Vector3D& position, const math::Quaternion& rotation, const math::Angle& fieldOfView, math::Real aspectRatio,
			math::Real zNearPlane, math::Real zFarPlane, math::Real sensitivity);

		/// <summary>
		/// The destructor of the camera object.
		/// </summary>
		RENDERING_API virtual ~Camera();

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
		RENDERING_API math::Transform& GetTransform() override { return m_transform; }
		RENDERING_API const math::Transform& GetTransform() const override { return m_transform; }
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		/// <summary> The camera transform. </summary>
		math::Transform m_transform;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class Camera */

} /* end namespace rendering */

#endif /* __RENDERING_CAMERA_H__ */