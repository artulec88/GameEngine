#ifndef __GAME_GAME_NODE_BUILDER_H__
#define __GAME_GAME_NODE_BUILDER_H__

#include "Def.h"
#include "Engine/GameNode.h"
#include "Engine/GameManager.h"
#include "Engine\MeshRendererComponent.h"
#include "Engine\ConstantRotationComponent.h"
#include "Engine\CameraBehavior.h"

#include "Rendering\Camera.h"
#include "Rendering/Texture.h"

#include "Utility/Builder.h"
#include "Utility\StringUtility.h"

#include <string>

namespace Game
{
	class GameNodeBuilder : public utility::Builder<engine::GameNode>
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		GameNodeBuilder(engine::GameManager* gameManager);
		virtual ~GameNodeBuilder(void);
		GameNodeBuilder(GameNodeBuilder& gameNodeBuilder) = delete;
		GameNodeBuilder(GameNodeBuilder&& gameNodeBuilder) = delete;
		GameNodeBuilder& operator=(const GameNodeBuilder& gameNodeBuilder) = delete;
		GameNodeBuilder& operator=(GameNodeBuilder&& gameNodeBuilder) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	protected:
		virtual engine::GameNode Get() override
		{
			return m_gameNode.Clone();
			//return std::move(m_gameNode);
		}

		virtual void Build() override
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
		engine::GameManager* m_gameManager;
		engine::GameNode m_gameNode;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class GameNodeBuilder */


	/// <summary> Game node for the cameras. </summary>
	class CameraNodeBuilder : public GameNodeBuilder
	{
		/* ==================== Static variables and functions begin ==================== */
	private:
		static /* constexpr */ engine::camera_behavior_types::CameraBehaviorType ConvertToCameraBehaviorType(const std::string& cameraBehaviorTypeStr)
		{
			const std::string cameraBehaviorTypeLowercaseStr = utility::string_utility::MakeLowercase(cameraBehaviorTypeStr);
			if (cameraBehaviorTypeLowercaseStr == "static")
			{
				return engine::camera_behavior_types::STATIC;
			}
			else if (cameraBehaviorTypeLowercaseStr == "rotation")
			{
				return engine::camera_behavior_types::ROTATION_ONLY;
			}
			else if (cameraBehaviorTypeLowercaseStr == "movement")
			{
				return engine::camera_behavior_types::MOVEMENT_ONLY;
			}
			else if (cameraBehaviorTypeLowercaseStr == "follow_entity")
			{
				return engine::camera_behavior_types::FOLLOW_ENTITY;
			}
			else if (cameraBehaviorTypeLowercaseStr == "follow_entity_with_rotation")
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
		virtual ~CameraNodeBuilder(void);
		CameraNodeBuilder(CameraNodeBuilder& cameraNodeBuilder) = delete;
		CameraNodeBuilder(CameraNodeBuilder&& cameraNodeBuilder) = delete;
		CameraNodeBuilder& operator=(const CameraNodeBuilder& cameraNodeBuilder) = delete;
		CameraNodeBuilder& operator=(CameraNodeBuilder&& cameraNodeBuilder) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void BuildTransform() override;
		virtual void BuildComponents() override;
		void SetGameNodeToFollow(const engine::GameNode* gameNodeToFollow)
		{
			m_gameNodeToFollow = gameNodeToFollow;
		}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		const engine::camera_behavior_types::CameraBehaviorType M_DEFAULT_CAMERA_BEHAVIOR_TYPE;
		const math::Real M_DEFAULT_CAMERA_FOLLOW_INITIAL_DISTANCE_FROM_ENTITY;
		const math::Real M_DEFAULT_CAMERA_FOLLOW_ANGLE_AROUND_ENTITY_SPEED;
		const math::Real M_DEFAULT_CAMERA_FOLLOW_PITCH_ROTATION_SPEED;
		const math::Angle M_DEFAULT_CAMERA_FOLLOW_INITIAL_PITCH_ANGLE;

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
		engine::GameManager* m_gameManager;
		math::Real m_scale;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class SkyboxBuilder */

} /* end namespace Game */

#endif // __GAME_GAME_NODE_BUILDER_H__
