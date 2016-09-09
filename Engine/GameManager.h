#ifndef __ENGINE_GAME_MANAGER_H__
#define __ENGINE_GAME_MANAGER_H__

#include "Engine.h"
#include "GameStateManager.h"
#include "ParticleGenerator.h"
#include "IUpdateable.h"
#include "IRenderable.h"
#include "GameNode.h"
#include "GameCommand.h"
#include "InputMapping.h"
#include "ShaderFactory.h"
#include "TextureFactory.h"
//#include "Observer.h"

#include "EffectFactory.h"
#include "Rendering\Mesh.h"
#include "Rendering\Shader.h"
#include "Rendering\Material.h"
#include "Rendering\BaseLight.h"
#include "Rendering\Camera.h"
#include "Rendering\FontFactory.h"
#include "Rendering\FogInfo.h"
#include "Rendering\GuiButtonControl.h"
#include "Rendering\Texture.h"

#include "Math\Transform.h"
#include "Math\Math.h"
#ifdef CALCULATE_ENGINE_STATS
#include "Math\Statistics.h"
#include "Math\IStatisticsStorage.h"
#endif

#include <amp.h>
#include <vector>

namespace Engine
{

	class GameManager : public IUpdateable
	{
		typedef std::map<const Rendering::Text::Font*, std::vector<Rendering::Controls::GuiButtonControl>, Rendering::Text::FontComparator> FontMap;
		typedef std::map<Actions::Action, std::list<GameNode*>> ActionsToGameNodesMap;
		typedef std::map<Actions::Action, const GameCommand*> ActionsToGameCommandsMap;
		typedef std::map<States::State, std::list<GameNode*>> StatesToGameNodesMap;
		typedef std::map<States::State, const GameCommand*> StatesToGameCommandsMap;
		/* ==================== Static variables and functions begin ==================== */
	protected:
		static GameManager* s_gameManager;
	public:
		ENGINE_API static GameManager* GetGameManager();
		ENGINE_API static void LoadGame(GameManager* game);
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary>
		/// Creates a game manager.
		/// </summary>
		/// <remarks>
		/// One must remember to create a game manager after rendering context is properly setup.
		/// Game manager will try to create game fonts which require the graphics context to be initialized.
		/// </remarks>
		ENGINE_API GameManager();

		/// <summary>
		/// Destroys the game manager.
		/// </summary>
		ENGINE_API virtual ~GameManager(void);
		GameManager(GameManager& gameManager) = delete;
		void operator=(GameManager& gameManager) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		ENGINE_API virtual void Load() = 0; // Loads the game
		ENGINE_API void Input(Actions::Action actionID);
		ENGINE_API virtual void Input(const Engine::Input::MappedInput& input);
		//ENGINE_API virtual void Notify(GameNode* gameNode, Actions::Action action /*const GameEvent& gameEvent*/) const;
		ENGINE_API void Render(Rendering::Renderer* renderer) const;

		ENGINE_API inline GameNode& GetRootGameNode() { return m_rootGameNode; }
		ENGINE_API inline GameNode* GetWaterNode() { return m_waterNode; }
		ENGINE_API inline GameNode* GetTerrainNode() { return m_terrainNode; }
		ENGINE_API inline GameNode* GetSkyboxNode() { return m_skyboxNode; }
		ENGINE_API inline const std::vector<GameNode*>& GetBillboardNodes() const { return m_billboardsNodes; }
		ENGINE_API inline const std::vector<ParticleGenerator*>& GetParticleGenerators() const { return m_particleGenerators; }
		//ENGINE_API inline const FontMap& GetTexts() const { return m_texts; }

		ENGINE_API virtual Math::Real GetLoadingProgress() const = 0;
		ENGINE_API bool IsGameLoaded() const { return m_isGameLoaded; }

		ENGINE_API void AddShader(ShaderTypes::ShaderType shaderType, const std::string& shaderFileName) { m_shaderFactory.CreateShader(shaderType, shaderFileName); }
		ENGINE_API Rendering::Texture* AddTexture(int textureID, const std::string& textureFileName) { return m_textureFactory.CreateTexture(textureID, textureFileName); }
		ENGINE_API Rendering::Texture* AddCubeTexture(int textureID, const std::string& cubeMapTextureDirectory) { return m_textureFactory.CreateCubeTexture(textureID, cubeMapTextureDirectory); }

#ifdef ANT_TWEAK_BAR_ENABLED
		virtual void InitializeTweakBars();
#endif

		ENGINE_API virtual GameState* GetIntroGameState() = 0;
		ENGINE_API virtual GameState* GetMainMenuGameState() = 0;
		ENGINE_API virtual GameState* GetLoadGameState() = 0;
		ENGINE_API virtual GameState* GetPlayGameState() = 0;
		ENGINE_API virtual GameState* GetPlayMainMenuGameState() = 0;

		ENGINE_API const Rendering::Text::Font* GetFont(Rendering::Text::FontTypes::FontType fontType);

		// TODO: Think about removing the window parameter from the event handling functions below.
		ENGINE_API virtual void WindowResizeEvent(int width, int height);
		ENGINE_API virtual void CloseWindowEvent();
		virtual void MousePosEvent(double xPos, double yPos);
		ENGINE_API virtual void ScrollEvent(double xOffset, double yOffset);

		/// <summary> Sets the game state transition object. The transition itself is not performed.
		/// The transition itself is performed in the <code>PerformStateTransition</code> method.</summary>
		/// <see cref="PerformStateTransition">
		ENGINE_API void SetTransition(GameStateTransitioning::GameStateTransition* gameStateTransition);
		void PerformStateTransition();
		ENGINE_API void PopState();
		ENGINE_API void RequestGameQuit() const;
		ENGINE_API const GameCommand& GetCommand(Actions::Action action) const
		{
			ActionsToGameCommandsMap::const_iterator actionToGameCommandItr = m_actionsToGameCommandsMap.find(action);
			if (actionToGameCommandItr == m_actionsToGameCommandsMap.end())
			{
				WARNING_LOG_ENGINE("No game command registered for the action ID: ", action);
				return m_emptyGameCommand;
			}
			return *actionToGameCommandItr->second;
		}
		//ENGINE_API Rendering::Effects::Effect<Math::Real>* GetSingleValueEffect(Rendering::Effects::EffectType effectType, unsigned int variant)
		//{
		//	return m_effectFactory.GetSingleValueEffect(effectType, variant);
		//}
		//ENGINE_API Rendering::Effects::Effect<Math::Vector2D>* GetVec2DEffect(Rendering::Effects::EffectType effectType, unsigned int variant)
		//{
		//	return m_effectFactory.GetVec2DEffect(effectType, variant);
		//}
		//ENGINE_API Rendering::Effects::Effect<Math::Vector3D>* GetVec3DEffect(Rendering::Effects::EffectType effectType, unsigned int variant)
		//{
		//	return m_effectFactory.GetVec3DEffect(effectType, variant);
		//}

		ENGINE_API inline const Rendering::Texture* GetTexture(int textureID) const
		{
			return m_textureFactory.GetTexture(textureID);
		}

		ENGINE_API inline const ShaderFactory& GetShaderFactory() const { return m_shaderFactory; }
		ENGINE_API inline const TextureFactory& GetTextureFactory() const { return m_textureFactory; }

		Rendering::Camera* GetCurrentCamera() { return m_cameras[m_currentCameraIndex]; }
		unsigned int GetCurrentCameraIndex() const { return m_currentCameraIndex; }
		ENGINE_API void AddCamera(Rendering::Camera* camera);
		unsigned int SetCurrentCamera(unsigned int cameraIndex);
		unsigned int NextCamera();
		unsigned int PrevCamera();
	public:
		ENGINE_API void AddTerrainNode(GameNode* terrainNode);
		ENGINE_API void AddWaterNode(GameNode* waterNode);
		ENGINE_API void AddBillboardsNode(GameNode* billboardsNode);
		ENGINE_API void AddGuiControl(const Rendering::Controls::GuiControl& guiControl);
		ENGINE_API void AddSkyboxNode(GameNode* skyboxNode);
		ENGINE_API void AddParticleGenerator(ParticleGenerator* particleGenerator);
	protected:
		ENGINE_API void AddToSceneRoot(GameNode* child);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		GameNode m_rootGameNode;
		GameNode* m_terrainNode;
		GameNode* m_skyboxNode;
		GameNode* m_waterNode;
		std::vector<GameNode*> m_billboardsNodes;
		std::vector<ParticleGenerator*> m_particleGenerators;
		ShaderFactory m_shaderFactory;
		TextureFactory m_textureFactory;
		Rendering::Text::FontFactory m_fontFactory;
		GameStateManager* m_gameStateManager;
		bool m_isGameLoaded;

		const EmptyGameCommand m_emptyGameCommand;
		ActionsToGameCommandsMap m_actionsToGameCommandsMap;
		//Rendering::Effects::EffectFactory m_effectFactory;

		std::vector<Rendering::Camera*> m_cameras;
		unsigned int m_currentCameraIndex;

		/// <summary>
		/// The list of game nodes that are interested in handling the particular action event that occurred in the game.
		/// </summary>
		ActionsToGameNodesMap m_actionsToGameNodesMap;

		/// <summary>
		/// The map of game commands identified by a state.
		/// </summary>
		StatesToGameCommandsMap m_statesToGameCommandsMap;

		/// <summary>
		/// The list of game nodes that are interested in handling the particular state that occurred in the game.
		/// </summary>
		StatesToGameNodesMap m_statesToGameNodesMap;

#ifdef ANT_TWEAK_BAR_ENABLED
		TwBar* m_gameBar;
		unsigned int m_cameraCountMinusOne;
#endif

#ifdef PROFILING_ENGINE_MODULE_ENABLED
		Math::Statistics::ClassStats& m_classStats;
#endif
		/* ==================== Non-static member variables end ==================== */
	}; /* end class GameManager */

} /* end namespace Engine */

#endif // __ENGINE_GAME_MANAGER_H__
