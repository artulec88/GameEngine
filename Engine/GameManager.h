#ifndef __ENGINE_GAME_MANAGER_H__
#define __ENGINE_GAME_MANAGER_H__

#include "Engine.h"
#include "GameStateManager.h"
//#include "ParticleSystem.h"
#include "IUpdateable.h"
#include "GameNode.h"
#include "GameCommand.h"
#include "InputMapping.h"
#include "IActionHandler.h"
//#include "Observer.h"

#include "Rendering/GuiButtonControl.h"
#include "Rendering/Texture.h"
#include "Rendering/ParticlesSystem.h"

#include "Math/Math.h"
#ifdef CALCULATE_ENGINE_STATS
#include "Math/Statistics.h"
#include "Math/StatisticsStorage.h"
#endif

//#include <amp.h>
#include <vector>

namespace engine
{
	class GameManager : public IUpdateable
	{
		using FontMap = std::map<const rendering::text::Font*, std::vector<rendering::controls::GuiButtonControl>, rendering::text::FontComparator>;
		using ActionsToGameNodesMap = std::map<actions::Action, std::list<GameNode*>>;
		using ActionsToHandlersMap = std::map<actions::Action, std::vector<IActionHandler*>>;
		using ActionsToGameCommandsMap = std::map<actions::Action, const GameCommand*>;
		using StatesToGameNodesMap = std::map<states::State, std::list<GameNode*>>;
		using StatesToGameCommandsMap = std::map<states::State, const GameCommand*>;
		/* ==================== Static variables and functions begin ==================== */
	protected:
		static GameManager* s_gameManager;
	public:
		ENGINE_API static GameManager* GetGameManager();
		ENGINE_API static void LoadGame(GameManager* game);
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
		/// <summary>
		/// Creates a game manager.
		/// </summary>
		/// <remarks>
		/// One must remember to create a game manager after rendering context is properly setup.
		/// Game manager will try to create game fonts which require the graphics context to be initialized.
		/// </remarks>
		ENGINE_API GameManager();

		/// <summary> Destroys the game manager. </summary>
		ENGINE_API virtual ~GameManager();

		/// <summary> Game manager copy constructor. </summary>
		/// <param name="gameManager"> Game manager to copy construct from. </param>
		GameManager(const GameManager& gameManager) = delete;
		
		/// <summary> Game manager move constructor. </summary>
		/// <param name="gameManager"> Game manager to move construct from. </param>
		GameManager(GameManager&& gameManager) = delete;
		
		/// <summary> Game manager copy assignment operator. </summary>
		/// <param name="gameManager"> Game manager to copy assign from. </param>
		/// <returns> The reference to the newly copy-assigned game manager. </returns>
		GameManager& operator=(const GameManager& gameManager) = delete;

		/// <summary> Game manager move assignment operator. </summary>
		/// <param name="gameManager"> The r-value reference to the game manager to move assign from. </param>
		/// <returns> The reference to the newly move-assigned game manager. </returns>
		GameManager& operator=(GameManager&& gameManager) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
		/// <summary> Loads the game. Performs the loading of all necessary models, textures, etc. </summary>
		ENGINE_API virtual void Load() = 0;
		ENGINE_API void Input(actions::Action actionId);
		ENGINE_API virtual void Input(const input::MappedInput& input);
		//ENGINE_API virtual void Notify(GameNode* gameNode, Actions::Action action /*const GameEvent& gameEvent*/) const;
		ENGINE_API void Render(rendering::Renderer* renderer) const;

		ENGINE_API const std::vector<rendering::particles::ParticlesSystem*>& GetParticlesSystems() const { return m_particlesSystems; }
		//ENGINE_API inline const FontMap& GetTexts() const { return m_texts; }

		ENGINE_API virtual math::Real GetLoadingProgress() const = 0;
		ENGINE_API bool IsGameLoaded() const { return m_isGameLoaded; }

		ENGINE_API const rendering::Mesh* AddMesh(int meshId, const std::string& meshFileName) const;
		ENGINE_API inline const rendering::Mesh* GetMesh(int meshId) const;

		ENGINE_API const rendering::Texture* AddTexture(int textureId, const std::string& textureFileName) const;
		ENGINE_API const rendering::Texture* AddCubeTexture(int textureId, const std::string& cubeMapTextureDirectory) const;
		ENGINE_API inline const rendering::Texture* GetTexture(int textureId) const;

		ENGINE_API const rendering::Shader* AddShader(int shaderId, const std::string& shaderFileName) const;
		ENGINE_API const rendering::Shader* GetShader(int shaderId) const;

		ENGINE_API const rendering::text::Font* CreateFont(int fontId, const std::string& fontTextureFileName, const std::string& fontMetaDataFileName);
		ENGINE_API const rendering::text::Font* GetFont(int fontId) const;

#ifdef ANT_TWEAK_BAR_ENABLED
		virtual void InitializeTweakBars();
#endif

		ENGINE_API virtual GameState* GetIntroGameState() = 0;
		ENGINE_API virtual GameState* GetMainMenuGameState() = 0;
		ENGINE_API virtual GameState* GetLoadGameState() = 0;
		ENGINE_API virtual GameState* GetPlayGameState() = 0;
		ENGINE_API virtual GameState* GetPlayMainMenuGameState() = 0;

		// TODO: Think about removing the window parameter from the event handling functions below.
		ENGINE_API virtual void WindowResizeEvent(int width, int height);
		ENGINE_API virtual void CloseWindowEvent();
		virtual void MousePosEvent(double xPos, double yPos);
		ENGINE_API virtual void ScrollEvent(double xOffset, double yOffset);

		/// <summary> Sets the game state transition object. The transition itself is not performed.
		/// The transition itself is performed in the <code>PerformStateTransition</code> method.</summary>
		/// <see cref="PerformStateTransition"/>
		ENGINE_API void SetTransition(game_state_transitioning::GameStateTransition* gameStateTransition) const;
		void PerformStateTransition() const;
		ENGINE_API void PopState() const;
		ENGINE_API void RequestGameQuit() const;
		ENGINE_API const GameCommand& GetCommand(actions::Action action) const
		{
			const auto actionToGameCommandItr = m_actionsToGameCommandsMap.find(action);
			CHECK_CONDITION_RETURN_ALWAYS_ENGINE(actionToGameCommandItr != m_actionsToGameCommandsMap.end(), m_emptyGameCommand, utility::logging::WARNING, "No game command registered for the action ID : ", action);
			return *actionToGameCommandItr->second;
		}
		//ENGINE_API Rendering::effects::Effect<math::Real>* GetSingleValueEffect(Rendering::effects::EffectType effectType, unsigned int variant)
		//{
		//	return m_effectFactory.GetSingleValueEffect(effectType, variant);
		//}
		//ENGINE_API Rendering::effects::Effect<math::Vector2D>* GetVec2DEffect(Rendering::effects::EffectType effectType, unsigned int variant)
		//{
		//	return m_effectFactory.GetVec2DEffect(effectType, variant);
		//}
		//ENGINE_API Rendering::effects::Effect<math::Vector3D>* GetVec3DEffect(Rendering::effects::EffectType effectType, unsigned int variant)
		//{
		//	return m_effectFactory.GetVec3DEffect(effectType, variant);
		//}

		ENGINE_API void CentralizeCursor() const;

		ENGINE_API void LoadSoundEffect(const std::string& soundEffectFileName) const;
		ENGINE_API void PlaySoundEffect(const std::string& soundEffectFileName, math::Real volume, math::Real pitch) const;
		ENGINE_API void AddGuiControl(const rendering::controls::GuiControl& guiControl);
		ENGINE_API void AddParticlesSystem(rendering::particles::ParticlesSystem* particlesSystem);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		std::vector<rendering::particles::ParticlesSystem*> m_particlesSystems;
		std::unique_ptr<GameStateManager> m_gameStateManager;
		bool m_isGameLoaded;

		const EmptyGameCommand m_emptyGameCommand;
		ActionsToGameCommandsMap m_actionsToGameCommandsMap;
		//ActionsToHandlersMap m_actionsToHandlersMap;

		//Rendering::effects::EffectFactory m_effectFactory;

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
#endif

#ifdef PROFILING_ENGINE_MODULE_ENABLED
		math::statistics::ClassStats& m_classStats;
#endif
		/* ==================== Non-static member variables end ==================== */
	}; /* end class GameManager */

} /* end namespace engine */

#endif // __ENGINE_GAME_MANAGER_H__
