#ifndef __GAME_GAME_NODE_BUILDER_H__
#define __GAME_GAME_NODE_BUILDER_H__

#include "Def.h"
#include "Engine\GameNode.h"
#include "Engine\GameManager.h"
#include "Engine\MeshRendererComponent.h"
#include "Engine\ConstantRotationComponent.h"
#include "Engine\CameraBehavior.h"

#include "Rendering\Camera.h"
#include "Rendering\Texture.h"

#include "Utility\Builder.h"
#include "Utility\StringUtility.h"

#include <string>

namespace Game
{
	class GameNodeBuilder : public Utility::Builder<Engine::GameNode>
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		GameNodeBuilder(Engine::GameManager* gameManager);
		virtual ~GameNodeBuilder(void);
		GameNodeBuilder(GameNodeBuilder& gameNodeBuilder) = delete;
		GameNodeBuilder(GameNodeBuilder&& gameNodeBuilder) = delete;
		GameNodeBuilder& operator=(const GameNodeBuilder& gameNodeBuilder) = delete;
		GameNodeBuilder& operator=(GameNodeBuilder&& gameNodeBuilder) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	protected:
		virtual Engine::GameNode Get() override
		{
			return m_gameNode.Clone();
			//return std::move(m_gameNode);
		}

		virtual void BuildParts() override
		{
			BuildTransform();
			BuildComponents();
		}

		virtual void BuildTransform()
		{
		}

		virtual void BuildComponents()
		{
		}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		Engine::GameManager* m_gameManager;
		Engine::GameNode m_gameNode;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class GameNodeBuilder */

	class CameraNodeBuilder : public GameNodeBuilder
	{
		/* ==================== Static variables and functions begin ==================== */
	private:
		static /* constexpr */ Engine::CameraBehaviorTypes::CameraBehaviorType ConvertToCameraBehaviorType(const std::string& cameraBehaviorTypeStr)
		{
			const std::string cameraBehaviorTypeLowercaseStr = Utility::StringUtility::MakeLowercase(cameraBehaviorTypeStr);
			if (cameraBehaviorTypeLowercaseStr == "static")
			{
				return Engine::CameraBehaviorTypes::STATIC;
			}
			else if (cameraBehaviorTypeLowercaseStr == "rotation")
			{
				return Engine::CameraBehaviorTypes::ROTATION_ONLY;
			}
			else if (cameraBehaviorTypeLowercaseStr == "movement")
			{
				return Engine::CameraBehaviorTypes::MOVEMENT_ONLY;
			}
			else if (cameraBehaviorTypeLowercaseStr == "follow_entity")
			{
				return Engine::CameraBehaviorTypes::FOLLOW_ENTITY;
			}
			else if (cameraBehaviorTypeLowercaseStr == "follow_entity_with_rotation")
			{
				return Engine::CameraBehaviorTypes::FOLLOW_ENTITY_WITH_ROTATION;
			}
			WARNING_LOG_GAME("Unknown camera behavior type \"", cameraBehaviorTypeStr, "\". Using STATIC as default.");
			return Engine::CameraBehaviorTypes::STATIC;
		}
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		CameraNodeBuilder(Engine::GameManager* gameManager);
		virtual ~CameraNodeBuilder(void);
		CameraNodeBuilder(CameraNodeBuilder& cameraNodeBuilder) = delete;
		CameraNodeBuilder(CameraNodeBuilder&& cameraNodeBuilder) = delete;
		CameraNodeBuilder& operator=(const CameraNodeBuilder& cameraNodeBuilder) = delete;
		CameraNodeBuilder& operator=(CameraNodeBuilder&& cameraNodeBuilder) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void BuildTransform();
		virtual void BuildComponents();
		void SetGameNodeToFollow(const Engine::GameNode* gameNodeToFollow)
		{
			m_gameNodeToFollow = gameNodeToFollow;
		}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		const Math::Vector3D M_DEFAULT_CAMERA_POS;
		const Math::Angle M_DEFAULT_CAMERA_ROTATION_ANGLE_X;
		const Math::Angle M_DEFAULT_CAMERA_ROTATION_ANGLE_Y;
		const Math::Angle M_DEFAULT_CAMERA_ROTATION_ANGLE_Z;
		const Math::Angle M_DEFAULT_CAMERA_FIELD_OF_VIEW;
		const Math::Real M_DEFAULT_CAMERA_ASPECT_RATIO;
		const Math::Real M_DEFAULT_CAMERA_NEAR_PLANE;
		const Math::Real M_DEFAULT_CAMERA_FAR_PLANE;
		const Math::Real M_DEFAULT_CAMERA_SENSITIVITY;
		const Math::Real M_DEFAULT_CAMERA_FOLLOW_INITIAL_DISTANCE_FROM_ENTITY;
		const Math::Real M_DEFAULT_CAMERA_FOLLOW_ANGLE_AROUND_ENTITY_SPEED;
		const Math::Real M_DEFAULT_CAMERA_FOLLOW_PITCH_ROTATION_SPEED;
		const Math::Angle M_DEFAULT_CAMERA_FOLLOW_INITIAL_PITCH_ANGLE;

		std::size_t m_camerasCount;
		std::vector<Math::Vector3D> m_positions;
		std::vector<Math::Quaternion> m_rotations;
		std::vector<Engine::CameraBehavior*> m_cameraBehaviors;
		std::vector<Math::Angle> m_fovs;
		std::vector<Math::Real> m_aspectRatios;
		std::vector<Math::Real> m_nearPlanes;
		std::vector<Math::Real> m_farPlanes;
		std::vector<Math::Real> m_sensitivities;

		const Engine::GameNode* m_gameNodeToFollow;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class CameraNodeBuilder */

	class SkyboxBuilder : public GameNodeBuilder
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		explicit SkyboxBuilder(Engine::GameManager* gameManager);
		virtual ~SkyboxBuilder(void);
		SkyboxBuilder(SkyboxBuilder& skyboxBuilder) = delete;
		SkyboxBuilder(SkyboxBuilder&& skyboxBuilder) = delete;
		SkyboxBuilder& operator=(const SkyboxBuilder& skyboxBuilder) = delete;
		SkyboxBuilder& operator=(SkyboxBuilder&& skyboxBuilder) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	protected:
		virtual void BuildTransform();
		virtual void BuildComponents();
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		Engine::GameManager* m_gameManager;
		Math::Real m_scale;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class SkyboxBuilder */

} /* end namespace Game */

#endif // __GAME_GAME_NODE_BUILDER_H__
