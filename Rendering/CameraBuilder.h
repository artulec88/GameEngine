#ifndef __RENDERING_CAMERA_BUILDER_H__
#define __RENDERING_CAMERA_BUILDER_H__

#include "Rendering.h"

#include "Math/Quaternion.h"
#include "Math/Angle.h"
#include "Math/Vector.h"
#include "Math/Matrix.h"

#include "Utility/Builder.h"

namespace rendering
{
	class Camera;

	/// <summary>
	/// The camera builder.
	/// </summary>
	class CameraBuilder : public utility::Builder<Camera>
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Camera builder constructor. </summary>
		RENDERING_API CameraBuilder();

		/// <summary> Camera builder destructor. </summary>
		RENDERING_API virtual ~CameraBuilder();

		/// <summary> Camera builder copy constructor. </summary>
		/// <param name="cameraBuilder"> The reference to camera builder to copy construct from. </param>
		CameraBuilder(CameraBuilder& cameraBuilder) = delete;

		/// <summary> Camera builder move constructor. </summary>
		/// <param name="cameraBuilder"> The r-value reference to camera builder to move construct from. </param>
		CameraBuilder(CameraBuilder&& cameraBuilder) = delete;

		/// <summary> Camera builder copy assignment operator. </summary>
		/// <param name="cameraBuilder"> The reference to camera builder to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned camera builder. </returns>
		CameraBuilder& operator=(const CameraBuilder& cameraBuilder) = delete;

		/// <summary> Camera builder move assignment operator. </summary>
		/// <param name="cameraBuilder"> The r=value reference to camera builder to move assign from. </param>
		/// <returns> The reference to the newly move-assigned camera builder. </returns>
		CameraBuilder& operator=(CameraBuilder&& cameraBuilder) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		RENDERING_API void SetDefault() override
		{
			m_pos.x = m_defaultCameraPosX;
			m_pos.y = m_defaultCameraPosY;
			m_pos.z = m_defaultCameraPosZ;
			m_rot = math::Quaternion(math::Matrix4D(m_defaultCameraRotationAngleX, m_defaultCameraRotationAngleY, m_defaultCameraRotationAngleZ));
			m_nearPlane = m_defaultCameraNearPlane;
			m_farPlane = m_defaultCameraFarPlane;
			// m_projectionMatrix is built from other member variables so there is no need to set default value to it.
			m_sensitivity = m_defaultCameraSensitivity;
		}

		RENDERING_API Camera Build() override;

		RENDERING_API CameraBuilder& SetPos(math::Real posX, math::Real posY, math::Real posZ)
		{
			m_pos.x = posX;
			m_pos.y = posY;
			m_pos.z = posZ;
			return *this;
		}
		RENDERING_API CameraBuilder& SetPos(const math::Vector3D& position)
		{
			m_pos = position;
			return *this;
		}
		RENDERING_API CameraBuilder& SetPosX(math::Real posX)
		{
			m_pos.x = posX;
			return *this;
		}
		RENDERING_API CameraBuilder& SetPosY(math::Real posY)
		{
			m_pos.y = posY;
			return *this;
		}
		RENDERING_API CameraBuilder& SetPosZ(math::Real posZ)
		{
			m_pos.z = posZ;
			return *this;
		}
		RENDERING_API CameraBuilder& SetRot(const math::Quaternion& rotation)
		{
			m_rot = rotation;
			return *this;
		}
		RENDERING_API CameraBuilder& SetNearPlane(math::Real nearPlane)
		{
			m_nearPlane = nearPlane;
			BuildProjectionMatrix();
			return *this;
		}
		RENDERING_API CameraBuilder& SetFarPlane(math::Real farPlane)
		{
			m_farPlane = farPlane;
			BuildProjectionMatrix();
			return *this;
		}
		RENDERING_API CameraBuilder& SetSensitivity(math::Real sensitivity)
		{
			m_sensitivity = sensitivity;
			return *this;
		}
		RENDERING_API CameraBuilder& SetProjectionMatrix(const math::Matrix4D& projectionMatrix)
		{
			m_projectionMatrix = projectionMatrix;
			return *this;
		}
		math::Matrix4D GetProjectionMatrix() const { return m_projectionMatrix; }
		math::Real GetSensitivity() const { return m_sensitivity; }
		math::Vector3D GetPos() const { return m_pos; }
		math::Quaternion GetRot() const { return m_rot; }
	protected:
		virtual void BuildProjectionMatrix() = 0;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
		const math::Real m_defaultCameraPosX; // TODO: Make it a static constexpr in the future.
		const math::Real m_defaultCameraPosY; // TODO: Make it a static constexpr in the future.
		const math::Real m_defaultCameraPosZ; // TODO: Make it a static constexpr in the future.
		const math::Angle m_defaultCameraRotationAngleX; // TODO: Make it a static constexpr in the future.
		const math::Angle m_defaultCameraRotationAngleY; // TODO: Make it a static constexpr in the future.
		const math::Angle m_defaultCameraRotationAngleZ; // TODO: Make it a static constexpr in the future.
		const math::Real m_defaultCameraNearPlane; // TODO: Make it a static constexpr in the future.
		const math::Real m_defaultCameraFarPlane; // TODO: Make it a static constexpr in the future.
		const math::Real m_defaultCameraSensitivity; // TODO: Make it a static constexpr in the future.

		math::Vector3D m_pos;
		math::Quaternion m_rot;
		math::Real m_nearPlane, m_farPlane;
		math::Matrix4D m_projectionMatrix;
		math::Real m_sensitivity;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class CameraBuilder */

	/// <summary>
	/// Orthographic projection camera builder.
	/// </summary>
	class OrthoCameraBuilder : public CameraBuilder
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Orthographic projection camera builder constructor. </summary>
		RENDERING_API OrthoCameraBuilder();

		/// <summary> Orthographic projection camera builder destructor. </summary>
		RENDERING_API virtual ~OrthoCameraBuilder();

		/// <summary> Orthographic projection camera builder copy constructor. </summary>
		/// <param name="orthoCameraBuilder"> The orthographic projection camera builder to copy construct from. </param>
		OrthoCameraBuilder(OrthoCameraBuilder& orthoCameraBuilder) = delete;

		/// <summary> Orthographic projection camera builder move constructor. </summary>
		/// <param name="orthoCameraBuilder"> The orthographic projection camera builder to move construct from. </param>
		OrthoCameraBuilder(OrthoCameraBuilder&& orthoCameraBuilder) = delete;

		/// <summary> Orthographic projection camera builder copy assignment operator. </summary>
		/// <param name="orthoCameraBuilder"> The orthographic projection camera builder to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned orthographic projection camera builder. </returns>
		OrthoCameraBuilder& operator=(const OrthoCameraBuilder& orthoCameraBuilder) = delete;

		/// <summary> Orthographic projection camera builder move assignment operator. </summary>
		/// <param name="orthoCameraBuilder"> The orthographic projection camera builder to move assign from. </param>
		/// <returns> The reference to the newly move-assigned orthographic projection camera builder. </returns>
		OrthoCameraBuilder& operator=(OrthoCameraBuilder&& orthoCameraBuilder) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		RENDERING_API void BuildProjectionMatrix() override;

		RENDERING_API OrthoCameraBuilder& SetLeft(math::Real left)
		{
			m_left = left;
			BuildProjectionMatrix();
			return *this;
		}
		RENDERING_API OrthoCameraBuilder& SetRight(math::Real right)
		{
			m_right = right;
			BuildProjectionMatrix();
			return *this;
		}
		RENDERING_API OrthoCameraBuilder& SetBottom(math::Real bottom)
		{
			m_bottom = bottom;
			BuildProjectionMatrix();
			return *this;
		}
		RENDERING_API OrthoCameraBuilder& SetTop(math::Real top)
		{
			m_top = top;
			BuildProjectionMatrix();
			return *this;
		}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		math::Real m_defaultCameraLeftPlane; // TODO: Make it a static constexpr in the future.
		math::Real m_defaultCameraRightPlane; // TODO: Make it a static constexpr in the future.
		math::Real m_defaultCameraBottomPlane; // TODO: Make it a static constexpr in the future.
		math::Real m_defaultCameraTopPlane; // TODO: Make it a static constexpr in the future.

		math::Real m_left, m_right;
		math::Real m_bottom, m_top;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class OrthoCameraBuilder */

	class PerspectiveCameraBuilder : public CameraBuilder
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		RENDERING_API PerspectiveCameraBuilder();
		RENDERING_API virtual ~PerspectiveCameraBuilder();
		PerspectiveCameraBuilder(PerspectiveCameraBuilder& perspectiveCameraBuilder) = delete;
		PerspectiveCameraBuilder(PerspectiveCameraBuilder&& perspectiveCameraBuilder) = delete;
		PerspectiveCameraBuilder& operator=(const PerspectiveCameraBuilder& perspectiveCameraBuilder) = delete;
		PerspectiveCameraBuilder& operator=(PerspectiveCameraBuilder&& perspectiveCameraBuilder) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		RENDERING_API void BuildProjectionMatrix() override;

		RENDERING_API PerspectiveCameraBuilder& SetFieldOfView(const math::Angle& fieldOfView)
		{
			m_fov = fieldOfView;
			BuildProjectionMatrix();
			return *this;
		}
		RENDERING_API PerspectiveCameraBuilder& SetAspectRatio(math::Real aspectRatio)
		{
			m_aspectRatio = aspectRatio;
			BuildProjectionMatrix();
			return *this;
		}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		const math::Angle m_defaultCameraFieldOfView; // TODO: Make it a static constexpr in the future.
		const math::Real m_defaultCameraAspectRatio; // TODO: Make it a static constexpr in the future.

		math::Angle m_fov;
		math::Real m_aspectRatio;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class PerspectiveCameraBuilder */
} /* end namespace game */

#endif /* __RENDERING_CAMERA_BUILDER_H__ */