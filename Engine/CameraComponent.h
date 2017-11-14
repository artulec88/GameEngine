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

namespace engine
{
	class CameraComponent : public GameComponent, Rendering::BaseCamera, public IActionHandler, public IStateHandler, public IRangeHandler
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary>
		/// The constructor of the camera component object.
		/// </summary>
		/// <param name="projectionMatrix">The projection matrix of the camera.</param>
		/// <param name="sensitivity"> The sensitivity of the camera. The value representing how fast the camera reacts to player input. </param>
		/// <param name="cameraBehavior"> The behavior of the camera. </param>
		ENGINE_API CameraComponent(const Math::Matrix4D& projectionMatrix, Math::Real sensitivity, CameraBehavior* cameraBehavior = NULL);
		/// <summary>
		/// The constructor of the base camera object.
		/// </summary>
		/// <param name="fov">The field of view of the camera.</param>
		/// <param name="aspectRatio">The aspect ratio of the camera.</param>
		/// <param name="zNearPlane">The near plane of the camera.</param>
		/// <param name="zFarPlane">The far plane of the camera.</param>
		/// <param name="sensitivity">The sensitivity of the camera. The value representing how fast the camera reacts to player input. </param>
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
		ENGINE_API virtual Math::Transform& GetTransform() { return GameComponent::GetTransform(); }
		ENGINE_API virtual const Math::Transform& GetTransform() const { return GameComponent::GetTransform(); }
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
	//	static constexpr Math::Real MINIMUM_DISTANCE_TO_ENTITY = 0.1f;
	//	static constexpr Math::Real MAXIMUM_DISTANCE_TO_ENTITY = 0.5f;
	//	static constexpr Math::Angle CameraFollowComponent::MINIMUM_PITCH_ANGLE{ 2.0f };
	//	static constexpr Math::Angle CameraFollowComponent::MAXIMUM_PITCH_ANGLE{ 70.0f };
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