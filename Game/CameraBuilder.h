#ifndef __GAME_CAMERA_BUILDER_H__
#define __GAME_CAMERA_BUILDER_H__

#include "Def.h"
#include "Engine\GameNode.h"
#include "Engine\GameManager.h"
#include "Engine\CameraComponent.h"

#include "Rendering\Texture.h"

#include "Utility\Builder.h"

#include <string>

namespace Game
{

	class CameraBuilder : public Utility::Builder<Rendering::BaseCamera>
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		CameraBuilder(Engine::GameManager* gameManager, Rendering::BaseCamera* camera);
		virtual ~CameraBuilder(void);
		CameraBuilder(CameraBuilder& cameraBuilder) = delete;
		CameraBuilder(CameraBuilder&& cameraBuilder) = delete;
		CameraBuilder& operator=(const CameraBuilder& cameraBuilder) = delete;
		CameraBuilder& operator=(CameraBuilder&& cameraBuilder) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void BuildPart1();
		virtual void BuildPart2();
		virtual void BuildPart3();
		void SetCameraIndex(int cameraIndex);
		//void SetEntityToFollow(Engine::GameNode* gameNodeToFollow);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		Engine::GameManager* m_gameManager;
		const Math::Vector3D M_DEFAULT_CAMERA_POS;
		const Math::Angle M_DEFAULT_CAMERA_ROTATION_ANGLE_X;
		const Math::Angle M_DEFAULT_CAMERA_ROTATION_ANGLE_Y;
		const Math::Angle M_DEFAULT_CAMERA_ROTATION_ANGLE_Z;
		const Math::Angle M_DEFAULT_CAMERA_FIELD_OF_VIEW;
		const Math::Real M_DEFAULT_CAMERA_ASPECT_RATIO;
		const Math::Real M_DEFAULT_CAMERA_NEAR_PLANE;
		const Math::Real M_DEFAULT_CAMERA_FAR_PLANE;
		const Math::Real M_DEFAULT_CAMERA_SENSITIVITY;
		//const Math::Real M_DEFAULT_CAMERA_FOLLOW_INITIAL_DISTANCE_FROM_ENTITY;
		//const Math::Real M_DEFAULT_CAMERA_FOLLOW_ANGLE_AROUND_ENTITY_SPEED;
		//const Math::Real M_DEFAULT_CAMERA_FOLLOW_PITCH_ROTATION_SPEED;
		//const Math::Angle M_DEFAULT_CAMERA_FOLLOW_INITIAL_PITCH_ANGLE;
		int m_cameraIndex;
		std::string m_cameraIndexStr;
		//Engine::GameNode* m_gameNodeToFollow;

		Math::Vector3D m_pos;
		Math::Quaternion m_rot;
		Math::Angle m_fov;
		Math::Real m_aspectRatio;
		Math::Real m_nearPlane;
		Math::Real m_farPlane;
		Math::Real m_sensitivity;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class CameraBuilder */
} /* end namespace Game */

#endif /* __GAME_CAMERA_BUILDER_H__ */