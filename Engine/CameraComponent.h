#ifndef __ENGINE_CAMERA_COMPONENT_H__
#define __ENGINE_CAMERA_COMPONENT_H__

#include "Engine.h"
#include "GameComponent.h"
#include "IUpdateable.h"
#include "IInputableMouse.h"

#include "Rendering\Camera.h"

namespace Engine
{
	namespace CameraTypes
	{
		enum CameraBehaviorType
		{
			STATIC_CAMERA = 0,
			ROTATION_ONLY_CAMERA,
			MOVEMENT_ONLY_CAMERA,
			MOVE_FREELY_CAMERA,
			FOLLOW_ENTITY_CAMERA,
			FOLLOW_ENTITY_WITH_ROTATION_CAMERA
		};
	}

	class CameraComponent : public GameComponent, public IUpdateable
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		CameraComponent(Rendering::Camera* camera);
		virtual ~CameraComponent(void);
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void Update(Math::Real deltaTime);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		Rendering::Camera* m_camera;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class CameraComponent */

	class CameraMoveComponent : public CameraComponent, public Input::IInputableMouse
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API CameraMoveComponent(Rendering::Camera* camera);
		ENGINE_API virtual ~CameraMoveComponent(void);
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void MouseButtonEvent(int button, int action, int mods);
		virtual void MousePosEvent(double xPos, double yPos);
		virtual void ScrollEvent(double xOffset, double yOffset);
		virtual void Update(Math::Real deltaTime);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		bool m_forward, m_backward, m_left, m_right, m_up, m_down;
		Math::Vector3D m_velocity;
		Math::Real m_maxSpeed;
		bool m_isLocked;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class CameraMoveComponent */

	class CameraFollowComponent : public CameraComponent, public Input::IInputableMouse
	{
		/* ==================== Static variables and functions begin ==================== */
	private:
		static const Math::Real MINIMUM_DISTANCE_TO_ENTITY;
		static const Math::Real MAXIMUM_DISTANCE_TO_ENTITY;
		static const Math::Angle CameraFollowComponent::MINIMUM_PITCH_ANGLE;
		static const Math::Angle CameraFollowComponent::MAXIMUM_PITCH_ANGLE;
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		CameraFollowComponent(Rendering::Camera* camera, GameNode* entityToFollow, Math::Real initialDistanceFromEntity,
			Math::Real angleAroundEntitySpeed, Math::Real pitchRotationSpeed, const Math::Angle& initialPitchAngle);
		virtual ~CameraFollowComponent(void);
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void Update(Math::Real delta);
		virtual void MouseButtonEvent(int button, int action, int mods);
		virtual void MousePosEvent(double xPos, double yPos);
		virtual void ScrollEvent(double xOffset, double yOffset);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		/// <summary>
		/// Entity to follow.
		/// </summary>
		GameNode* m_gameEntityToFollow;

		/// <summary>
		/// The value representing the zoom. In other words, it defines how close to the entity being followed the camera should be placed.
		/// </summary>
		Math::Real m_distanceFromEntity;

		bool m_changingAngleAroundEntity;
		/// <summary>
		/// The angle around the player.
		/// </summary>
		const Math::Angle m_angleAroundEntitySpeed;
		Math::Angle m_currentAngleAroundEntity;

		/// <summary>
		/// <code>true</code> if the camera is in the "changing pitch" phase (right mouse button pressed).
		/// </summary>
		bool m_changingPitch;
		const Math::Angle m_pitchRotationSpeed;
		Math::Angle m_currentPitchAngle;

		Math::Real m_lastCursorPositionX;
		Math::Real m_lastCursorPositionY;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class CameraFollowComponent */

} /* end namespace Engine */

#endif // __ENGINE_CAMERA_COMPONENT_H__