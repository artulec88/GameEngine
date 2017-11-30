#ifndef __GAME_GAME_NODE_BUILDER_H__
#define __GAME_GAME_NODE_BUILDER_H__

#include "Def.h"
#include "Engine/GameNode.h"
#include "Engine/GameManager.h"
#include "Engine/ConstantRotationComponent.h"
#include "Engine/CameraBehavior.h"

#include "Utility/Builder.h"
#include "Utility/StringUtility.h"

#include <string>

namespace game
{
	class GameNodeBuilder : public utility::Builder<engine::GameNode>
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		explicit GameNodeBuilder(engine::GameManager* gameManager);
		virtual ~GameNodeBuilder();
		GameNodeBuilder(GameNodeBuilder& gameNodeBuilder) = delete;
		GameNodeBuilder(GameNodeBuilder&& gameNodeBuilder) = delete;
		GameNodeBuilder& operator=(const GameNodeBuilder& gameNodeBuilder) = delete;
		GameNodeBuilder& operator=(GameNodeBuilder&& gameNodeBuilder) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	protected:
		engine::GameNode Get() override
		{
			return m_gameNode.Clone();
			//return std::move(m_gameNode);
		}

		void Build() override
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
		engine::GameManager* m_gameManager;
		engine::GameNode m_gameNode;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class GameNodeBuilder */


	/// <summary> Game node for the cameras. </summary>
	class CameraNodeBuilder : public GameNodeBuilder
	{
		/* ==================== Static variables and functions begin ==================== */
		static /* constexpr */ engine::camera_behavior_types::CameraBehaviorType ConvertToCameraBehaviorType(const std::string& cameraBehaviorTypeStr)
		{
			const auto cameraBehaviorTypeLowercaseStr = utility::string_utility::MakeLowercase(cameraBehaviorTypeStr);
			if (cameraBehaviorTypeLowercaseStr == "static")
			{
				return engine::camera_behavior_types::STATIC;
			}
			if (cameraBehaviorTypeLowercaseStr == "rotation")
			{
				return engine::camera_behavior_types::ROTATION_ONLY;
			}
			if (cameraBehaviorTypeLowercaseStr == "movement")
			{
				return engine::camera_behavior_types::MOVEMENT_ONLY;
			}
			if (cameraBehaviorTypeLowercaseStr == "follow_entity")
			{
				return engine::camera_behavior_types::FOLLOW_ENTITY;
			}
			if (cameraBehaviorTypeLowercaseStr == "follow_entity_with_rotation")
			{
				return engine::camera_behavior_types::FOLLOW_ENTITY_WITH_ROTATION;
			}
			WARNING_LOG_GAME("Unknown camera behavior type \"", cameraBehaviorTypeStr, "\". Using STATIC as default.");
			return engine::camera_behavior_types::STATIC;
		}
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		CameraNodeBuilder(engine::GameManager* gameManager, rendering::BaseCamera* camera);
		virtual ~CameraNodeBuilder();
		CameraNodeBuilder(CameraNodeBuilder& cameraNodeBuilder) = delete;
		CameraNodeBuilder(CameraNodeBuilder&& cameraNodeBuilder) = delete;
		CameraNodeBuilder& operator=(const CameraNodeBuilder& cameraNodeBuilder) = delete;
		CameraNodeBuilder& operator=(CameraNodeBuilder&& cameraNodeBuilder) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		void BuildTransform() override;
		void BuildComponents() override;
		void SetGameNodeToFollow(const engine::GameNode* gameNodeToFollow)
		{
			m_gameNodeToFollow = gameNodeToFollow;
		}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		const engine::camera_behavior_types::CameraBehaviorType m_defaultCameraBehaviorType;
		const math::Real m_defaultCameraFollowInitialDistanceFromEntity;
		const math::Real m_defaultCameraFollowAngleAroundEntitySpeed;
		const math::Real m_defaultCameraFollowPitchRotationSpeed;
		const math::Angle m_defaultCameraFollowInitialPitchAngle;

		rendering::BaseCamera* m_camera;
		engine::camera_behavior_types::CameraBehaviorType m_cameraBehaviorType;

		const engine::GameNode* m_gameNodeToFollow;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class CameraNodeBuilder */

	class SkyboxBuilder : public GameNodeBuilder
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		explicit SkyboxBuilder(engine::GameManager* gameManager);
		virtual ~SkyboxBuilder();
		SkyboxBuilder(SkyboxBuilder& skyboxBuilder) = delete;
		SkyboxBuilder(SkyboxBuilder&& skyboxBuilder) = delete;
		SkyboxBuilder& operator=(const SkyboxBuilder& skyboxBuilder) = delete;
		SkyboxBuilder& operator=(SkyboxBuilder&& skyboxBuilder) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	protected:
		void BuildTransform() override;
		void BuildComponents() override;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		engine::GameManager* m_gameManager;
		math::Real m_scale;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class SkyboxBuilder */

} /* end namespace game */

#endif // __GAME_GAME_NODE_BUILDER_H__
