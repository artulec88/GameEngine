#ifndef __GAME_TEST_GAME_MANAGER_H__
#define __GAME_TEST_GAME_MANAGER_H__

#include "Def.h"
#include "StartGameCommand.h"
#include "ShowIntroCommand.h"
#include "ResumeGameCommand.h"
#include "SaveGameCommand.h"
#include "LoadGameCommand.h"

#include "Engine\QuitGameCommand.h"
#include "Engine\GameManager.h"
#include "Engine\GameNode.h"
#include "Rendering\DirectionalLight.h"
#include "Rendering\PointLight.h"
#include "Rendering\SpotLight.h"

#ifdef PROFILING_GAME_MODULE_ENABLED
#include "Math\IStatisticsStorage.h"
#include "Math\Statistics.h"
#endif

#include <vector>
#include <map>

namespace Game
{
	namespace TextureTypes
	{
		enum TextureType
		{
			DEFAULT = 0,
			DEFAULT_NORMAL_MAP,
			DEFAULT_DISPLACEMENT_MAP,
			TERRAIN_DIFFUSE_1,
			TERRAIN_DIFFUSE_2,
			TERRAIN_DIFFUSE_3,
			TERRAIN_DIFFUSE_4,
			TERRAIN_NORMAL_MAP,
			TERRAIN_DISPLACEMENT_MAP,
			TERRAIN_BLEND_MAP,
			// TODO: Add more texture types
			COUNT
		}; /* end enum TextureType */
	} /* end namespace TextureTypes */

	class TestGameManager : public Engine::GameManager
	{
		/* ==================== Constructors and destructors begin ==================== */
	public:
		TestGameManager();
		virtual ~TestGameManager(void);
	private:
		TestGameManager(TestGameManager& gameManager);
		void operator=(TestGameManager& gameManager);
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void Load();
		virtual void Update(Math::Real delta);

		virtual Math::Real GetLoadingProgress() const;

		virtual Engine::GameState* GetIntroGameState();
		virtual Engine::GameState* GetMainMenuGameState();
		virtual Engine::GameState* GetLoadGameState();
		virtual Engine::GameState* GetPlayGameState();
		virtual Engine::GameState* GetPlayMainMenuGameState();

		virtual void WindowResizeEvent(int width, int height);
		virtual void ScrollEvent(double xOffset, double yOffset);
		virtual void MousePosEvent(double xPos, double yPos);
#ifdef ANT_TWEAK_BAR_ENABLED
		virtual void InitializeTweakBars();
#endif
	private:
		void AddBillboards(unsigned int billboardsCount, Rendering::Material* billboardsMaterial);
		void AddCameras(Engine::GameNode* entityToFollow);
		void AddSkybox() const;
		//void AddStaticEffects();
		//void AddSmoothEffects();
		//void AddBlinkEffects();
	/* ==================== Non-static member functions end ==================== */

	/* ==================== Non-static member variables begin ==================== */
	protected:
		const int RESOURCES_TO_LOAD;
		const Math::Real CAMERA_HEIGHT_UPDATE_INTERVAL;
		int m_resourcesLoaded;

		std::unique_ptr<Engine::GameState> m_introGameState;
		std::unique_ptr<Engine::GameState> m_menuGameState;
		std::unique_ptr<Engine::GameState> m_loadGameState;
		std::unique_ptr<Engine::GameState> m_playGameState;
		std::unique_ptr<Engine::GameState> m_playMainMenuGameState;

		StartGameCommand m_startGameCommand;
		Engine::QuitGameCommand m_quitGameCommand;
		ShowIntroCommand m_showIntroGameCommand;
		ResumeGameCommand m_resumeGameCommand;
		SaveGameCommand m_saveGameCommand;
		LoadGameCommand m_loadGameCommand;

		//std::vector<Rendering::Effects::Effect<Math::Real>> m_singleValueStaticEffects;
		//std::vector<Rendering::Effects::Effect<Math::Vector2D>> m_vec2DStaticEffects;
		//std::vector<Rendering::Effects::Effect<Math::Vector3D>> m_vec3DStaticEffects;
		//std::vector<Rendering::Effects::SmoothTransitionEffect<Math::Real>> m_singleValueSmoothEffects;
		//std::vector<Rendering::Effects::SmoothTransitionEffect<Math::Vector2D>> m_vec2DSmoothEffects;
		//std::vector<Rendering::Effects::SmoothTransitionEffect<Math::Vector3D>> m_vec3DSmoothEffects;
		//std::vector<Rendering::Effects::BlinkEffect<Math::Real>> m_singleValueBlinkEffects;
		//std::vector<Rendering::Effects::BlinkEffect<Math::Vector2D>> m_vec2DBlinkEffects;
		//std::vector<Rendering::Effects::BlinkEffect<Math::Vector3D>> m_vec3DBlinkEffects;

		Rendering::TerrainMesh* m_terrainMesh;
		Math::Real m_timeToUpdateCameraHeight;

		Engine::GameNode* m_boxNode;
#ifdef ANT_TWEAK_BAR_ENABLED
		Rendering::Material* terrainMaterial;
		Rendering::Material* boxMaterial;

		Math::Real terrainSpecularIntensity, terrainSpecularPower;
		Math::Real terrainDisplacementScale, terrainDisplacementOffset;
#endif

		const int HUMAN_NODES_COUNT;
		Engine::GameNode** humanNodes;
		const int cameraCount;
		Engine::GameNode** cameraNodes;
		bool m_heightMapCalculationEnabled;
		//Engine::GameNode* castleNode;

#ifdef PROFILING_GAME_MODULE_ENABLED
		Math::Statistics::ClassStats& m_classStats;
#endif

		/* ==================== Non-static member variables end ==================== */
	}; /* end class TestGameManager */

} /* end namespace Game */

#endif /* __GAME_TEST_GAME_MANAGER_H__ */