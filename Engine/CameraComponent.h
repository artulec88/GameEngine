#ifndef __ENGINE_CAMERA_COMPONENT_H__
#define __ENGINE_CAMERA_COMPONENT_H__

#include "Engine.h"
#include "GameComponent.h"
#include "IUpdateable.h"
#include "IActionHandler.h"
#include "IStateHandler.h"
#include "IRangeHandler.h"
#include "CameraBehavior.h"

#include "Rendering\BaseCamera.h"

#include <vector>

namespace Engine
{
	class CameraComponent : public GameComponent, public Rendering::BaseCamera, public IActionHandler, public IStateHandler, public IRangeHandler
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary>
		/// The constructor of the camera component object.
		/// </summary>
		/// <param name="projectionMatrix">The projection matrix of the camera.</param>
		/// <param name="cameraBehavior"> The behavior of the camera. </param>
		ENGINE_API CameraComponent(const Math::Matrix4D& projectionMatrix, CameraBehavior* cameraBehavior = NULL);
		/// <summary>
		/// The constructor of the camera component object.
		/// </summary>
		/// <param name="projectionMatrix">The projection matrix of the camera.</param>
		/// <param name="sensitivity">The value representing how fast the camera reacts to player input.</param>
		/// <param name="cameraBehavior"> The behavior of the camera. </param>
		ENGINE_API CameraComponent(const Math::Matrix4D& projectionMatrix, Math::Real sensitivity, CameraBehavior* cameraBehavior = NULL);
		/// <summary>
		/// The constructor of the base camera object.
		/// </summary>
		/// <param name="fov">The field of view of the camera.</param>
		/// <param name="aspectRatio">The aspect ratio of the camera.</param>
		/// <param name="zNearPlane">The near plane of the camera.</param>
		/// <param name="zFarPlane">The far plane of the camera.</param>
		/// <param name="sensitivity">The value representing how fast the camera reacts to player input.</param>
		/// <param name="cameraBehavior"> The behavior of the camera. </param>
		ENGINE_API CameraComponent(const Math::Angle& FoV, Math::Real aspectRatio, Math::Real zNearPlane, Math::Real zFarPlane, CameraBehavior* cameraBehavior = NULL);
		/// <summary>
		/// The constructor of the base camera object.
		/// </summary>
		/// <param name="fov">The field of view of the camera.</param>
		/// <param name="aspectRatio">The aspect ratio of the camera.</param>
		/// <param name="zNearPlane">The near plane of the camera.</param>
		/// <param name="zFarPlane">The far plane of the camera.</param>
		/// <param name="sensitivity">The value representing how fast the camera reacts to player input.</param>
		/// <param name="cameraBehavior"> The behavior of the camera. </param>
		ENGINE_API CameraComponent(const Math::Angle& FoV, Math::Real aspectRatio, Math::Real zNearPlane, Math::Real zFarPlane, Math::Real sensitivity, CameraBehavior* cameraBehavior = NULL);
		ENGINE_API virtual ~CameraComponent(void);
		
		/// <summary> The camera component copy constructor. </summary>
		ENGINE_API CameraComponent(const CameraComponent& cameraComponent) = delete;

		/// <summary> The camera component move constructor. </summary>
		ENGINE_API CameraComponent(CameraComponent&& cameraComponent);

		/// <summary> The camera component copy assignment operator. </summary>
		CameraComponent& operator=(const CameraComponent& cameraComponent) = delete;

		/// <summary> The camera component move assignment operator. </summary>
		CameraComponent& operator=(CameraComponent&& cameraComponent);
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		ENGINE_API virtual const Math::Vector3D& GetPos() const { m_pos = GetTransform().GetTransformedPos(); return m_pos; }
		ENGINE_API virtual const Math::Quaternion& GetRot() const { m_rot = GetTransform().GetTransformedRot(); return m_rot; }
		ENGINE_API virtual void SetPos(const Math::Vector3D& position) { GetTransform().SetPos(position); }
		ENGINE_API virtual void SetPos(Math::Real x, Math::Real y, Math::Real z) { GetTransform().SetPos(x, y, z); }
		ENGINE_API virtual void SetPosX(Math::Real x) { GetTransform().SetPosX(x); }
		ENGINE_API virtual void SetPosY(Math::Real y) { GetTransform().SetPosY(y); }
		ENGINE_API virtual void SetPosZ(Math::Real z) { GetTransform().SetPosZ(z); }
		ENGINE_API virtual void IncreasePos(Math::Real x, Math::Real y, Math::Real z) { GetTransform().IncreasePos(x, y, z); }
		ENGINE_API virtual void IncreasePos(const Math::Vector3D& translation) { GetTransform().IncreasePos(translation); }
		ENGINE_API virtual void IncreasePosX(Math::Real x) { GetTransform().IncreasePosX(x); }
		ENGINE_API virtual void IncreasePosY(Math::Real y) { GetTransform().IncreasePosY(y); }
		ENGINE_API virtual void IncreasePosZ(Math::Real z) { GetTransform().IncreasePosZ(z); }
		ENGINE_API virtual void SetRot(const Math::Quaternion& rotation) { GetTransform().SetRot(rotation); }
		ENGINE_API void SetCameraBehavior(CameraBehavior* cameraBehavior) { m_cameraBehavior = cameraBehavior; }
		ENGINE_API void Handle(Actions::Action action)
		{
			if (m_cameraBehavior != NULL && IsActive())
			{
				m_cameraBehavior->Handle(this, action);
			}
		}
		ENGINE_API void Handle(States::State state)
		{
			if (m_cameraBehavior != NULL && IsActive())
			{
				m_cameraBehavior->Handle(this, state);
			}
		}
		ENGINE_API void Handle(Ranges::Range range, Math::Real value)
		{
			if (m_cameraBehavior != NULL && IsActive())
			{
				m_cameraBehavior->Handle(this, range, value);
			}
		}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		/// <summary>
		/// The type of behavior of the camera. By modifying this member variable one can change the way the camera reacts to player's input.
		/// </summary>
		CameraBehavior* m_cameraBehavior;

		mutable Math::Vector3D m_pos;
		mutable Math::Quaternion m_rot;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class CameraComponent */

	//class CameraMoveComponent : public CameraComponent, public Input::IInputableMouse
	//{
	//	/* ==================== Static variables and functions begin ==================== */
	//	/* ==================== Static variables and functions end ==================== */

	//	/* ==================== Constructors and destructors begin ==================== */
	//public:
	//	ENGINE_API CameraMoveComponent(Rendering::Camera* camera);
	//	ENGINE_API virtual ~CameraMoveComponent(void);
	//	CameraMoveComponent(const CameraMoveComponent& cameraMoveComponent) = delete;
	//	ENGINE_API CameraMoveComponent(CameraMoveComponent&& cameraMoveComponent);
	//	CameraMoveComponent& operator=(const CameraMoveComponent& cameraMoveComponent) = delete;
	//	ENGINE_API CameraMoveComponent& operator=(CameraMoveComponent&& cameraMoveComponent);
	//	/* ==================== Constructors and destructors end ==================== */

	//	/* ==================== Non-static member functions begin ==================== */
	//public:
	//	ENGINE_API virtual void MouseButtonEvent(int button, int action, int mods);
	//	ENGINE_API virtual void MousePosEvent(double xPos, double yPos);
	//	ENGINE_API virtual void ScrollEvent(double xOffset, double yOffset);
	//	ENGINE_API virtual void Update(Math::Real deltaTime);
	//	/* ==================== Non-static member functions end ==================== */

	//	/* ==================== Non-static member variables begin ==================== */
	//private:
	//	bool m_forward, m_backward, m_left, m_right, m_up, m_down;
	//	Math::Vector3D m_velocity;
	//	Math::Real m_maxSpeed;
	//	bool m_isLocked;
	//	/* ==================== Non-static member variables end ==================== */
	//}; /* end class CameraMoveComponent */

	//class CameraFollowComponent : public CameraComponent, public IStateHandler, public IRangeHandler
	//{
	//	/* ==================== Static variables and functions begin ==================== */
	//private:
	//	static const Math::Real MINIMUM_DISTANCE_TO_ENTITY;
	//	static const Math::Real MAXIMUM_DISTANCE_TO_ENTITY;
	//	static const Math::Angle CameraFollowComponent::MINIMUM_PITCH_ANGLE;
	//	static const Math::Angle CameraFollowComponent::MAXIMUM_PITCH_ANGLE;
	//	/* ==================== Static variables and functions end ==================== */

	//	/* ==================== Constructors and destructors begin ==================== */
	//public:
	//	ENGINE_API CameraFollowComponent(Rendering::Camera* camera, const GameNode* entityToFollow, Math::Real initialDistanceFromEntity,
	//		Math::Real angleAroundEntitySpeed, Math::Real pitchRotationSpeed, const Math::Angle& initialPitchAngle);
	//	ENGINE_API virtual ~CameraFollowComponent(void);
	//	CameraFollowComponent(const CameraFollowComponent& cameraFollowComponent) = delete;
	//	ENGINE_API CameraFollowComponent(CameraFollowComponent&& cameraFollowComponent);
	//	CameraFollowComponent& operator=(const CameraFollowComponent& cameraFollowComponent) = delete;
	//	ENGINE_API CameraFollowComponent& operator=(CameraFollowComponent&& cameraFollowComponent);
	//	/* ==================== Constructors and destructors end ==================== */

	//	/* ==================== Non-static member functions begin ==================== */
	//public:
	//	ENGINE_API virtual void Update(Math::Real delta);
	//	ENGINE_API virtual void Handle(States::State state);
	//	ENGINE_API virtual void Handle(Ranges::Range range, Math::Real value);
	//	/* ==================== Non-static member functions end ==================== */

	//	/* ==================== Non-static member variables begin ==================== */
	//private:
	//	/// <summary>
	//	/// Entity to follow.
	//	/// </summary>
	//	const GameNode* m_gameEntityToFollow;

	//	/// <summary>
	//	/// The value representing the zoom. In other words, it defines how close to the entity being followed the camera should be placed.
	//	/// </summary>
	//	Math::Real m_distanceFromEntity;

	//	bool m_changingAngleAroundEntity;
	//	/// <summary>
	//	/// The angle around the player.
	//	/// </summary>
	//	Math::Angle m_angleAroundEntitySpeed;
	//	Math::Angle m_currentAngleAroundEntity;

	//	/// <summary>
	//	/// <code>true</code> if the camera is in the "changing pitch" phase (right mouse button pressed).
	//	/// </summary>
	//	bool m_changingPitch;
	//	Math::Angle m_pitchRotationSpeed;
	//	Math::Angle m_currentPitchAngle;

	//	Math::Vector2D m_mousePos;
	//	Math::Vector2D m_prevMousePos;
	//	bool m_mousePosChanged;
	//	/* ==================== Non-static member variables end ==================== */
	//}; /* end class CameraFollowComponent */

} /* end namespace Engine */

#endif // __ENGINE_CAMERA_COMPONENT_H__