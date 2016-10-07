#ifndef __ENGINE_GAME_MANAGER_H__
#define __ENGINE_GAME_MANAGER_H__

#include "Engine.h"
#include "GameStateManager.h"
//#include "ParticleSystem.h"
#include "IUpdateable.h"
#include "IRenderable.h"
#include "GameNode.h"
#include "GameCommand.h"
#include "InputMapping.h"
//#include "Observer.h"

#include "Rendering\TextureFactory.h"
#include "Rendering\ShaderFactory.h"
#include "Rendering\Mesh.h"
#include "Rendering\Shader.h"
#include "Rendering\Material.h"
#include "Rendering\BaseLight.h"
#include "Rendering\Camera.h"
#include "Rendering\FontFactory.h"
#include "Rendering\FogInfo.h"
#include "Rendering\GuiButtonControl.h"
#include "Rendering\Texture.h"
#include "Rendering\ParticlesSystem.h"

#include "Math\EffectFactory.h"
#include "Math\Transform.h"
#include "Math\Math.h"
#ifdef CALCULATE_ENGINE_STATS
#include "Math\Statistics.h"
#include "Math\StatisticsStorage.h"
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

		/// <summary>
		/// Game manager copy constructor.
		/// </summary>
		GameManager(const GameManager& gameManager) = delete;
		/// <summary>
		/// Game manager move constructor.
		/// </summary>
		GameManager(GameManager&& gameManager) = delete;
		/// <summary>
		/// Game manager copy assignment operator.
		/// </summary>
		GameManager& operator=(const GameManager& gameManager) = delete;
		/// <summary>
		/// Game manager move assignment operator.
		/// </summary>
		GameManager& operator=(GameManager&& gameManager) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		ENGINE_API virtual void Load() = 0; // Loads the game
		ENGINE_API void Input(Actions::Action actionID);
		ENGINE_API virtual void Input(const Engine::Input::MappedInput& input);
		//ENGINE_API virtual void Notify(GameNode* gameNode, Actions::Action action /*const GameEvent& gameEvent*/) const;
		ENGINE_API void Render(Rendering::Renderer* renderer) const;

		ENGINE_API inline const std::vector<Rendering::Particles::ParticlesSystem*>& GetParticlesSystems() const { return m_particlesSystems; }
		//ENGINE_API inline const FontMap& GetTexts() const { return m_texts; }

		ENGINE_API virtual Math::Real GetLoadingProgress() const = 0;
		ENGINE_API bool IsGameLoaded() const { return m_isGameLoaded; }

		ENGINE_API void AddShader(int shaderID, const std::string& shaderFileName) { m_shaderFactory.CreateShader(shaderID, shaderFileName); }
		ENGINE_API const Rendering::Texture* AddTexture(int textureID, const std::string& textureFileName) { return m_textureFactory.CreateTexture(textureID, textureFileName); }
		ENGINE_API const Rendering::Texture* AddCubeTexture(int textureID, const std::string& cubeMapTextureDirectory) { return m_textureFactory.CreateCubeTexture(textureID, cubeMapTextureDirectory); }
		ENGINE_API const Rendering::Particles::ParticleTexture* AddParticleTexture(int textureID, const std::string& particleTextureFileName, int rowsCount, bool isAdditive)
		{
			return m_textureFactory.CreateParticleTexture(textureID, particleTextureFileName, rowsCount, isAdditive);
		}

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

		ENGINE_API void LoadSoundEffect(const std::string& soundEffectFileName) const;
		ENGINE_API void PlaySoundEffect(const std::string& soundEffectFileName, Math::Real volume, Math::Real pitch) const;

		ENGINE_API inline const Rendering::ShaderFactory& GetShaderFactory() const { return m_shaderFactory; }
		ENGINE_API inline const Rendering::TextureFactory& GetTextureFactory() const { return m_textureFactory; }
	public:
		ENGINE_API void AddGuiControl(const Rendering::Controls::GuiControl& guiControl);
		ENGINE_API void AddParticlesSystem(Rendering::Particles::ParticlesSystem* particlesSystem);
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	protected:
		std::vector<Rendering::Particles::ParticlesSystem*> m_particlesSystems;
		Rendering::ShaderFactory m_shaderFactory;
		Rendering::TextureFactory m_textureFactory;
		Rendering::Text::FontFactory m_fontFactory;
		std::unique_ptr<GameStateManager> m_gameStateManager;
		bool m_isGameLoaded;

		const EmptyGameCommand m_emptyGameCommand;
		ActionsToGameCommandsMap m_actionsToGameCommandsMap;
		//Rendering::Effects::EffectFactory m_effectFactory;

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
		Math::Statistics::ClassStats& m_classStats;
#endif
		/* ==================== Non-static member variables end ==================== */
	}; /* end class GameManager */

} /* end namespace Engine */

#endif // __ENGINE_GAME_MANAGER_H__
