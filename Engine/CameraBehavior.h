#ifndef __ENGINE_CAMERA_BEHAVIOR_H__
#define __ENGINE_CAMERA_BEHAVIOR_H__

#include "Engine.h"
#include "ActionConstants.h"
#include "Rendering\BaseCamera.h"

namespace Engine
{
	namespace CameraBehaviorTypes
	{
		enum CameraBehaviorType
		{
			STATIC = 0,
			ROTATION_ONLY,
			MOVEMENT_ONLY,
			FOLLOW_ENTITY,
			FOLLOW_ENTITY_WITH_ROTATION
		}; /* end enum CameraBehaviorType */
	} /* end namespace CameraBehaviorTypes */

	class CameraBehavior
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API CameraBehavior();
		ENGINE_API virtual ~CameraBehavior(void);
		CameraBehavior(const CameraBehavior& cameraBehavior) = delete;
		CameraBehavior(CameraBehavior&& cameraBehavior) = default;
		CameraBehavior& operator=(const CameraBehavior& cameraBehavior) = delete;
		CameraBehavior& operator=(CameraBehavior&& cameraBehavior) = default;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void Handle(Rendering::BaseCamera* camera, Actions::Action action) = 0;
		virtual void Handle(Rendering::BaseCamera* camera, States::State state) = 0;
		virtual void Handle(Rendering::BaseCamera* camera, Ranges::Range range, Math::Real value) = 0;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		/* ==================== Non-static member variables end ==================== */
	}; /* end class CameraBehavior */

	class StaticCameraBehavior : public CameraBehavior
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API StaticCameraBehavior();
		ENGINE_API virtual ~StaticCameraBehavior(void);
		StaticCameraBehavior(const StaticCameraBehavior& staticCameraBehavior) = delete;
		StaticCameraBehavior(StaticCameraBehavior&& staticCameraBehavior) = default;
		StaticCameraBehavior& operator=(const StaticCameraBehavior& staticCameraBehavior) = delete;
		StaticCameraBehavior& operator=(StaticCameraBehavior&& staticCameraBehavior) = default;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void Handle(Rendering::BaseCamera* camera, Actions::Action action);
		virtual void Handle(Rendering::BaseCamera* camera, States::State state);
		virtual void Handle(Rendering::BaseCamera* camera, Ranges::Range range, Math::Real value);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		/* ==================== Non-static member variables end ==================== */
	}; /* end class StaticCameraBehavior */

	class RotationCameraBehavior : public CameraBehavior
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API RotationCameraBehavior();
		ENGINE_API virtual ~RotationCameraBehavior(void);
		RotationCameraBehavior(const RotationCameraBehavior& rotationCameraBehavior) = delete;
		RotationCameraBehavior(RotationCameraBehavior&& rotationCameraBehavior) = default;
		RotationCameraBehavior& operator=(const RotationCameraBehavior& rotationCameraBehavior) = delete;
		RotationCameraBehavior& operator=(RotationCameraBehavior&& rotationCameraBehavior) = default;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void Handle(Rendering::BaseCamera* camera, Actions::Action action);
		virtual void Handle(Rendering::BaseCamera* camera, States::State state);
		virtual void Handle(Rendering::BaseCamera* camera, Ranges::Range range, Math::Real value);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		/* ==================== Non-static member variables end ==================== */
	}; /* end class RotationCameraBehavior */

	class MovementCameraBehavior : public CameraBehavior
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		ENGINE_API MovementCameraBehavior();
		ENGINE_API virtual ~MovementCameraBehavior(void);
		MovementCameraBehavior(const MovementCameraBehavior& movementCameraBehavior) = delete;
		MovementCameraBehavior(MovementCameraBehavior&& movementCameraBehavior) = default;
		MovementCameraBehavior& operator=(const MovementCameraBehavior& movementCameraBehavior) = delete;
		MovementCameraBehavior& operator=(MovementCameraBehavior&& moveMovementCameraBehaviormentCameraBehavior) = default;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void Handle(Rendering::BaseCamera* camera, Actions::Action action);
		virtual void Handle(Rendering::BaseCamera* camera, States::State state);
		virtual void Handle(Rendering::BaseCamera* camera, Ranges::Range range, Math::Real value);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		/* ==================== Non-static member variables end ==================== */
	}; /* end class MovementCameraBehavior */
} /* end namespace Engine */

#endif /* __ENGINE_CAMERA_BEHAVIOR_H__ */