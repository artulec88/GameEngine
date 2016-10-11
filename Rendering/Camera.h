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
		RENDERING_API virtual ~Camera(void);

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
		RENDERING_API virtual const Math::Vector3D& GetPos() const { return m_pos; }
		RENDERING_API virtual const Math::Quaternion& GetRot() const { return m_rot; }
		RENDERING_API virtual void SetPos(const Math::Vector3D& position) { m_pos = position; }
		RENDERING_API virtual void SetPos(Math::Real x, Math::Real y, Math::Real z) { m_pos.Set(x, y, z); }
		RENDERING_API virtual void SetPosX(Math::Real x) { m_pos.SetX(x); }
		RENDERING_API virtual void SetPosY(Math::Real y) { m_pos.SetY(y); }
		RENDERING_API virtual void SetPosZ(Math::Real z) { m_pos.SetZ(z); }
		RENDERING_API virtual void IncreasePos(Math::Real x, Math::Real y, Math::Real z) { m_pos.Increase(x, y, z); }
		RENDERING_API virtual void IncreasePos(const Math::Vector3D& translation) { m_pos.Increase(translation); }
		RENDERING_API virtual void IncreasePosX(Math::Real x) { m_pos.IncreaseX(x); }
		RENDERING_API virtual void IncreasePosY(Math::Real y) { m_pos.IncreaseY(y); }
		RENDERING_API virtual void IncreasePosZ(Math::Real z) { m_pos.IncreaseZ(z); }
		RENDERING_API virtual void SetRot(const Math::Quaternion& rotation) { m_rot = rotation; }
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		/// <summary> The camera position. </summary>
		Math::Vector3D m_pos;

		/// <summary> The camera rotation. </summary>
		Math::Quaternion m_rot;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class Camera */

} /* end namespace Rendering */

#endif /* __RENDERING_CAMERA_H__ */