#ifndef __GAME_TEST_GAME_MANAGER_H__
#define __GAME_TEST_GAME_MANAGER_H__

#include "Def.h"
#include "StartGameCommand.h"
#include "ShowIntroCommand.h"
#include "ResumeGameCommand.h"
#include "SaveGameCommand.h"
#include "LoadGameCommand.h"

#include "Engine\QuitGameCommand.h"
#include "Engine/GameManager.h"
#include "Engine/GameNode.h"
#include "Rendering/DirectionalLight.h"
#include "Rendering/PointLight.h"
#include "Rendering/SpotLight.h"

#ifdef PROFILING_GAME_MODULE_ENABLED
#include "Math/StatisticsStorage.h"
#include "Math/Statistics.h"
#endif

#include <vector>
#include <map>

namespace game
{
	class TestGameManager : public engine::GameManager
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		TestGameManager();
		virtual ~TestGameManager();
		TestGameManager(const TestGameManager& gameManager) = delete;
		TestGameManager(TestGameManager&& testGameManager) = delete;
		TestGameManager& operator=(const TestGameManager& testGameManager) = delete;
		TestGameManager& operator=(TestGameManager&& testGameManager) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		void Load() override;
		void Update(math::Real delta) override;

		math::Real GetLoadingProgress() const override;

		engine::GameState* GetIntroGameState() override;
		engine::GameState* GetMainMenuGameState() override;
		engine::GameState* GetLoadGameState() override;
		engine::GameState* GetPlayGameState() override;
		engine::GameState* GetPlayMainMenuGameState() override;

		void WindowResizeEvent(int width, int height) override;
		void ScrollEvent(double xOffset, double yOffset) override;
		void MousePosEvent(double xPos, double yPos) override;
#ifdef ANT_TWEAK_BAR_ENABLED
		void InitializeTweakBars() override;
#endif
		//Rendering::Particles::ParticlesSystem CreateParticlesSystem(Particleeffects::ParticleEffect particleEffect); // TODO: temporary code. Remove in the future.
		//void AddCameras(Engine::GameNode* entityToFollow);
		//void AddStaticEffects();
		//void AddSmoothEffects();
		//void AddBlinkEffects();
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	protected:
		const int RESOURCES_TO_LOAD;
		const math::Real CAMERA_HEIGHT_UPDATE_INTERVAL;
		int m_resourcesLoaded;

		std::unique_ptr<engine::GameState> m_introGameState;
		std::unique_ptr<engine::GameState> m_menuGameState;
		std::unique_ptr<engine::GameState> m_loadGameState;
		std::unique_ptr<engine::GameState> m_playGameState;
		std::unique_ptr<engine::GameState> m_playMainMenuGameState;

		StartGameCommand m_startGameCommand;
		engine::QuitGameCommand m_quitGameCommand;
		ShowIntroCommand m_showIntroGameCommand;
		ResumeGameCommand m_resumeGameCommand;
		SaveGameCommand m_saveGameCommand;
		LoadGameCommand m_loadGameCommand;

		//std::vector<Rendering::effects::Effect<math::Real>> m_singleValueStaticEffects;
		//std::vector<Rendering::effects::Effect<math::Vector2D>> m_vec2DStaticEffects;
		//std::vector<Rendering::effects::Effect<math::Vector3D>> m_vec3DStaticEffects;
		//std::vector<Rendering::effects::SmoothTransitionEffect<math::Real>> m_singleValueSmoothEffects;
		//std::vector<Rendering::effects::SmoothTransitionEffect<math::Vector2D>> m_vec2DSmoothEffects;
		//std::vector<Rendering::effects::SmoothTransitionEffect<math::Vector3D>> m_vec3DSmoothEffects;
		//std::vector<Rendering::effects::BlinkEffect<math::Real>> m_singleValueBlinkEffects;
		//std::vector<Rendering::effects::BlinkEffect<math::Vector2D>> m_vec2DBlinkEffects;
		//std::vector<Rendering::effects::BlinkEffect<math::Vector3D>> m_vec3DBlinkEffects;

		math::Real m_timeToUpdateCameraHeight;

		engine::GameNode* m_boxNode;

		const int HUMAN_NODES_COUNT;
		engine::GameNode** humanNodes;
		bool m_heightMapCalculationEnabled;

#ifdef PROFILING_GAME_MODULE_ENABLED
		math::statistics::ClassStats& m_classStats;
#endif

		/* ==================== Non-static member variables end ==================== */
	}; /* end class TestGameManager */

} /* end namespace Game */

#endif /* __GAME_TEST_GAME_MANAGER_H__ */