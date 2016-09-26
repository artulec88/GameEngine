#ifndef __GAME_GAME_NODE_BUILDER_H__
#define __GAME_GAME_NODE_BUILDER_H__

#include "Def.h"
#include "Engine\GameNode.h"
#include "Engine\GameManager.h"
#include "Engine\MeshRendererComponent.h"
#include "Engine\ConstantRotationComponent.h"

#include "Rendering\Camera.h"
#include "Rendering\Texture.h"

#include "Utility\Builder.h"

#include <string>

namespace Game
{
	class GameNodeBuilder : public Utility::Builder<Engine::GameNode>
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		GameNodeBuilder(Engine::GameManager* gameManager, Engine::GameNode* gameNode);
		virtual ~GameNodeBuilder(void);
		GameNodeBuilder(GameNodeBuilder& gameNodeBuilder) = delete;
		GameNodeBuilder(GameNodeBuilder&& gameNodeBuilder) = delete;
		GameNodeBuilder& operator=(const GameNodeBuilder& gameNodeBuilder) = delete;
		GameNodeBuilder& operator=(GameNodeBuilder&& gameNodeBuilder) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		Engine::GameManager* m_gameManager;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class GameNodeBuilder */

	class CameraNodeBuilder : public GameNodeBuilder
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		CameraNodeBuilder(Engine::GameManager* gameManager, Engine::GameNode* cameraNode, Rendering::Camera* camera);
		virtual ~CameraNodeBuilder(void);
		CameraNodeBuilder(CameraNodeBuilder& cameraNodeBuilder) = delete;
		CameraNodeBuilder(CameraNodeBuilder&& cameraNodeBuilder) = delete;
		CameraNodeBuilder& operator=(const CameraNodeBuilder& cameraNodeBuilder) = delete;
		CameraNodeBuilder& operator=(CameraNodeBuilder&& cameraNodeBuilder) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void BuildPart1();
		virtual void BuildPart2();
		void SetCameraIndex(unsigned int cameraIndex)
		{
			m_cameraIndex = cameraIndex;
			m_cameraIndexStr = std::to_string(m_cameraIndex);
		}
		void SetGameNodeToFollow(const Engine::GameNode* gameNodeToFollow)
		{
			m_gameNodeToFollow = gameNodeToFollow;
		}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		Rendering::Camera* m_camera;
		unsigned int m_cameraIndex;
		std::string m_cameraIndexStr;
		const Engine::GameNode* m_gameNodeToFollow;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class CameraNodeBuilder */

	class SkyboxBuilder : public GameNodeBuilder
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		explicit SkyboxBuilder(Engine::GameManager* gameManager, Engine::GameNode* skyboxNode);
		virtual ~SkyboxBuilder(void);
		SkyboxBuilder(SkyboxBuilder& skyboxBuilder) = delete;
		SkyboxBuilder(SkyboxBuilder&& skyboxBuilder) = delete;
		SkyboxBuilder& operator=(const SkyboxBuilder& skyboxBuilder) = delete;
		SkyboxBuilder& operator=(SkyboxBuilder&& skyboxBuilder) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void BuildPart1();
		virtual void BuildPart2();
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		Engine::GameManager* m_gameManager;
		Math::Real m_scale;
		Engine::MeshRendererComponent* m_meshRendererComponent;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class SkyboxBuilder */

} /* end namespace Game */

#endif // __GAME_GAME_NODE_BUILDER_H__
