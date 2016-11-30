#ifndef __RENDERING_CAMERA_BUILDER_H__
#define __RENDERING_CAMERA_BUILDER_H__

#include "Rendering.h"
#include "Camera.h"
#include "Texture.h"

#include "Utility\Builder.h"

#include <string>

namespace Rendering
{
	/// <summary>
	/// The camera builder.
	/// </summary>
	class CameraBuilder : public Utility::Builder<Camera>
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Camera builder constructor. </summary>
		RENDERING_API CameraBuilder();

		/// <summary> Camera builder destructor. </summary>
		RENDERING_API virtual ~CameraBuilder(void);

		/// <summary> Camera builder copy constructor. </summary>
		CameraBuilder(CameraBuilder& cameraBuilder) = delete;

		/// <summary> Camera builder move constructor. </summary>
		CameraBuilder(CameraBuilder&& cameraBuilder) = delete;

		/// <summary> Camera builder copy assignment operator. </summary>
		CameraBuilder& operator=(const CameraBuilder& cameraBuilder) = delete;

		/// <summary> Camera builder move assignment operator. </summary>
		CameraBuilder& operator=(CameraBuilder&& cameraBuilder) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		RENDERING_API virtual Camera Get() override
		{
			return Camera(m_pos, m_rot, m_projectionMatrix, m_sensitivity);
		}

		RENDERING_API virtual void Build() override
		{
			BuildTransform();
			BuildProjectionMatrix();
			BuildSensitivity();
		}

		RENDERING_API virtual void SetDefault()
		{
			m_pos.x = M_DEFAULT_CAMERA_POS_X;
			m_pos.y = M_DEFAULT_CAMERA_POS_Y;
			m_pos.z = M_DEFAULT_CAMERA_POS_Z;
			m_rot = Math::Quaternion(Math::Matrix4D(M_DEFAULT_CAMERA_ROTATION_ANGLE_X, M_DEFAULT_CAMERA_ROTATION_ANGLE_Y, M_DEFAULT_CAMERA_ROTATION_ANGLE_Z));
			m_nearPlane = M_DEFAULT_CAMERA_NEAR_PLANE;
			m_farPlane = M_DEFAULT_CAMERA_FAR_PLANE;
			// m_projectionMatrix is built from other member variables so there is no need to set default value to it.
			m_sensitivity = M_DEFAULT_CAMERA_SENSITIVITY;
		}

		RENDERING_API CameraBuilder& SetPos(Math::Real posX, Math::Real posY, Math::Real posZ)
		{
			m_pos.x = posX;
			m_pos.y = posY;
			m_pos.z = posZ;
			return *this;
		}
		RENDERING_API CameraBuilder& SetPos(const Math::Vector3D& position)
		{
			m_pos = position;
			return *this;
		}
		RENDERING_API CameraBuilder& SetPosX(Math::Real posX)
		{
			m_pos.x = posX;
			return *this;
		}
		RENDERING_API CameraBuilder& SetPosY(Math::Real posY)
		{
			m_pos.y = posY;
			return *this;
		}
		RENDERING_API CameraBuilder& SetPosZ(Math::Real posZ)
		{
			m_pos.z = posZ;
			return *this;
		}
		RENDERING_API CameraBuilder& SetRot(const Math::Quaternion& rotation)
		{
			m_rot = rotation;
			return *this;
		}
		RENDERING_API CameraBuilder& SetNearPlane(Math::Real nearPlane) { m_nearPlane = nearPlane; return *this; }
		RENDERING_API CameraBuilder& SetFarPlane(Math::Real farPlane) { m_farPlane = farPlane; return *this; }
		RENDERING_API CameraBuilder& SetSensitivity(Math::Real sensitivity)
		{
			m_sensitivity = sensitivity;
			return *this;
		}
	protected:
		virtual void BuildTransform()
		{
		}
		virtual void BuildProjectionMatrix() = 0;
		virtual void BuildSensitivity()
		{
		}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		const Math::Real M_DEFAULT_CAMERA_POS_X; // TODO: Make it a static constexpr in the future.
		const Math::Real M_DEFAULT_CAMERA_POS_Y; // TODO: Make it a static constexpr in the future.
		const Math::Real M_DEFAULT_CAMERA_POS_Z; // TODO: Make it a static constexpr in the future.
		const Math::Angle M_DEFAULT_CAMERA_ROTATION_ANGLE_X; // TODO: Make it a static constexpr in the future.
		const Math::Angle M_DEFAULT_CAMERA_ROTATION_ANGLE_Y; // TODO: Make it a static constexpr in the future.
		const Math::Angle M_DEFAULT_CAMERA_ROTATION_ANGLE_Z; // TODO: Make it a static constexpr in the future.
		const Math::Real M_DEFAULT_CAMERA_NEAR_PLANE; // TODO: Make it a static constexpr in the future.
		const Math::Real M_DEFAULT_CAMERA_FAR_PLANE; // TODO: Make it a static constexpr in the future.
		const Math::Real M_DEFAULT_CAMERA_SENSITIVITY; // TODO: Make it a static constexpr in the future.

		Math::Vector3D m_pos;
		Math::Quaternion m_rot;
		Math::Real m_nearPlane, m_farPlane;
		Math::Matrix4D m_projectionMatrix;
		Math::Real m_sensitivity;
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
		RENDERING_API virtual ~OrthoCameraBuilder(void);

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
	public:
		RENDERING_API virtual void BuildProjectionMatrix() override;

		RENDERING_API OrthoCameraBuilder& SetLeft(Math::Real left) { m_left = left; return *this; }
		RENDERING_API OrthoCameraBuilder& SetRight(Math::Real right) { m_right = right; return *this; }
		RENDERING_API OrthoCameraBuilder& SetBottom(Math::Real bottom) { m_bottom = bottom; return *this; }
		RENDERING_API OrthoCameraBuilder& SetTop(Math::Real top) { m_top = top; return *this; }
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		Math::Real M_DEFAULT_CAMERA_LEFT_PLANE; // TODO: Make it a static constexpr in the future.
		Math::Real M_DEFAULT_CAMERA_RIGHT_PLANE; // TODO: Make it a static constexpr in the future.
		Math::Real M_DEFAULT_CAMERA_BOTTOM_PLANE; // TODO: Make it a static constexpr in the future.
		Math::Real M_DEFAULT_CAMERA_TOP_PLANE; // TODO: Make it a static constexpr in the future.

		Math::Real m_left, m_right;
		Math::Real m_bottom, m_top;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class OrthoCameraBuilder */

	class PerspectiveCameraBuilder : public CameraBuilder
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		RENDERING_API PerspectiveCameraBuilder();
		RENDERING_API virtual ~PerspectiveCameraBuilder(void);
		PerspectiveCameraBuilder(PerspectiveCameraBuilder& perspectiveCameraBuilder) = delete;
		PerspectiveCameraBuilder(PerspectiveCameraBuilder&& perspectiveCameraBuilder) = delete;
		PerspectiveCameraBuilder& operator=(const PerspectiveCameraBuilder& perspectiveCameraBuilder) = delete;
		PerspectiveCameraBuilder& operator=(PerspectiveCameraBuilder&& perspectiveCameraBuilder) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		RENDERING_API virtual void BuildProjectionMatrix() override;

		RENDERING_API PerspectiveCameraBuilder& SetFieldOfView(const Math::Angle& fieldOfView) { m_fov = fieldOfView; return *this; }
		RENDERING_API PerspectiveCameraBuilder& SetAspectRatio(Math::Real aspectRatio) { m_aspectRatio = aspectRatio; return *this; }
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		const Math::Angle M_DEFAULT_CAMERA_FIELD_OF_VIEW; // TODO: Make it a static constexpr in the future.
		const Math::Real M_DEFAULT_CAMERA_ASPECT_RATIO; // TODO: Make it a static constexpr in the future.

		Math::Angle m_fov;
		Math::Real m_aspectRatio;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class PerspectiveCameraBuilder */
} /* end namespace Game */

#endif /* __RENDERING_CAMERA_BUILDER_H__ */